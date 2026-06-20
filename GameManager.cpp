/**
 * @file GameManager.cpp
 * @brief 游戏管理器类实现
 * 
 * 本文件实现了游戏管理器的所有功能，包括：
 * - 游戏状态管理（主菜单、加载中、游戏中、暂停、游戏结束、胜利）
 * - 游戏系统的初始化和管理（玩家、时间系统、共鸣系统、生态系统）
 * - 游戏时间和天数管理
 * - 场景切换管理
 * - 存档和读档管理
 * 
 * 游戏管理器是游戏的核心协调器，
 * 统一管理所有游戏系统和状态。
 * 
 * 游戏状态机：
 * - MainMenu：主菜单状态，游戏未开始
 * - Loading：加载中状态，正在加载游戏资源
 * - Playing：游戏中状态，正常游戏流程
 * - Paused：暂停状态，游戏暂停
 * - GameOver：游戏结束状态，玩家失败
 * - Victory：胜利状态，玩家胜利
 * 
 * 系统管理：
 * - 玩家系统（Player）：管理玩家状态和属性
 * - 时间系统（TimeSystem）：管理时间褶皱和时间熵
 * - 共鸣系统（ResonanceSystem）：管理星核共鸣
 * - 生态系统（EcosystemSystem）：管理生态共生
 * 
 * 游戏时间：
 * - 游戏时间以秒为单位
 * - 每 1440 秒（24 分钟）为一天
 * - 天数从第 1 天开始计数
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#include "GameManager.h"

#include <QDebug>
#include <QPointF>

// ============================================================
// 构造函数与析构函数
// ============================================================

/**
 * @brief 构造函数
 * 
 * 创建游戏管理器实例，但不进行任何初始化操作。
 * 所有的初始化工作都在 initialize() 中完成。
 * 
 * @param parent 父对象，用于 Qt 对象树内存管理
 */
GameManager::GameManager(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[GameManager] 创建游戏管理器";
}

/**
 * @brief 析构函数
 * 
 * 销毁游戏管理器，自动调用 shutdown() 清理所有游戏系统。
 */
GameManager::~GameManager()
{
    qDebug() << "[GameManager] 销毁游戏管理器";
    shutdown();
}

// ============================================================
// 生命周期管理
// ============================================================

/**
 * @brief 初始化游戏
 * 
 * 执行以下初始化操作：
 * 1. 检查是否已经初始化（防止重复初始化）
 * 2. 初始化所有游戏系统（玩家、时间系统、共鸣系统、生态系统）
 * 3. 设置初始游戏状态（主菜单）
 * 4. 重置游戏时间和天数
 * 
 * @return true 初始化成功
 * @return false 初始化失败（游戏系统初始化失败）
 */
bool GameManager::initialize()
{
    // 防止重复初始化
    if (m_initialized) {
        qWarning() << "[GameManager] 游戏管理器已经初始化，跳过重复初始化";
        return true;
    }

    qDebug() << "[GameManager] 初始化游戏管理器...";

    // 1. 初始化所有游戏系统
    qDebug() << "[GameManager] 初始化游戏系统";
    if (!initializeSystems()) {
        qCritical() << "[GameManager] 游戏系统初始化失败";
        return false;
    }

    // 2. 设置初始游戏状态
    m_gameState = MainMenu;
    
    // 3. 重置游戏时间和天数
    m_gameTime = 0.0f;
    m_gameDay = 1;
    
    // 4. 设置初始场景
    m_currentSceneName = "main_menu";

    // 初始化完成
    m_initialized = true;
    
    qDebug() << "[GameManager] 游戏管理器初始化完成";
    return true;
}

/**
 * @brief 关闭游戏
 * 
 * 执行以下清理操作：
 * 1. 清理生态系统
 * 2. 清理共鸣系统
 * 3. 清理时间系统
 * 4. 清理玩家系统
 * 5. 重置状态
 * 
 * 注意：
 * - 系统按创建的逆序删除（先创建的后删除）
 * - 每个系统都先调用 shutdown() 再删除
 * - 删除后将指针置为 nullptr，避免悬空指针
 * 
 * 此函数是幂等的，可以安全地多次调用。
 */
void GameManager::shutdown()
{
    // 如果没有初始化，直接返回
    if (!m_initialized) {
        return;
    }

    qDebug() << "[GameManager] 关闭游戏管理器...";

    // 1. 清理生态系统
    if (m_ecosystemSystem) {
        qDebug() << "[GameManager] 关闭生态系统";
        m_ecosystemSystem->shutdown();
        delete m_ecosystemSystem;
        m_ecosystemSystem = nullptr;
    }
    
    // 2. 清理共鸣系统
    if (m_resonanceSystem) {
        qDebug() << "[GameManager] 关闭共鸣系统";
        m_resonanceSystem->shutdown();
        delete m_resonanceSystem;
        m_resonanceSystem = nullptr;
    }
    
    // 3. 清理时间系统
    if (m_timeSystem) {
        qDebug() << "[GameManager] 关闭时间系统";
        m_timeSystem->shutdown();
        delete m_timeSystem;
        m_timeSystem = nullptr;
    }
    
    // 4. 清理玩家系统
    if (m_player) {
        qDebug() << "[GameManager] 关闭玩家系统";
        m_player->shutdown();
        delete m_player;
        m_player = nullptr;
    }

    // 5. 重置状态
    m_initialized = false;
    
    qDebug() << "[GameManager] 游戏管理器已关闭";
}

/**
 * @brief 更新游戏
 * 
 * 每帧调用一次，更新游戏状态。
 * 只有在游戏状态为 Playing 时才会更新。
 * 
 * 更新内容：
 * 1. 更新游戏时间
 * 2. 检查是否进入新的一天
 * 3. 更新玩家
 * 4. 更新时间系统
 * 5. 更新共鸣系统
 * 6. 更新生态系统
 * 
 * 游戏时间计算：
 * - 游戏时间以秒为单位累积
 * - 每 1440 秒（24 分钟）为一天
 * - 超过 1440 秒后，天数加 1，游戏时间减去 1440 秒
 * 
 * @param deltaTime 时间增量（秒）
 */
void GameManager::update(float deltaTime)
{
    // 只有在游戏状态为 Playing 时才更新
    if (!m_initialized || m_gameState != Playing) {
        return;
    }

    // 1. 更新游戏时间
    m_gameTime += deltaTime;
    
    // 2. 检查是否进入新的一天（每 1440 秒 = 24 分钟为一天）
    const float SECONDS_PER_DAY = 1440.0f;  // 24 分钟 = 1440 秒
    if (m_gameTime >= SECONDS_PER_DAY) {
        m_gameTime -= SECONDS_PER_DAY;
        m_gameDay++;
        qDebug() << "[GameManager] 第" << m_gameDay << "天开始";
    }

    // 3. 更新玩家
    if (m_player) {
        m_player->update(deltaTime);
    }
    
    // 4. 更新时间系统
    if (m_timeSystem) {
        m_timeSystem->update(deltaTime);
    }
    
    // 5. 更新共鸣系统
    if (m_resonanceSystem) {
        m_resonanceSystem->update(deltaTime);
    }
    
    // 6. 更新生态系统
    if (m_ecosystemSystem) {
        m_ecosystemSystem->update(deltaTime);
    }
}

// ============================================================
// 游戏状态管理
// ============================================================

/**
 * @brief 设置游戏状态
 * 
 * 设置新的游戏状态。
 * 如果状态没有变化，不做任何操作。
 * 
 * 状态变化时会：
 * 1. 保存旧状态
 * 2. 设置新状态
 * 3. 发出 gameStateChanged 信号
 * 4. 根据新状态发出相应信号（gameStarted、gamePaused、gameOver 等）
 * 
 * 注意：
 * - gameResumed 信号没有在 switch 中处理
 * - 因为从 Paused 变为 Playing 时，gameStarted 信号已经发出了
 * - 后续可以优化，区分"开始游戏"和"恢复游戏"
 * 
 * @param state 新的游戏状态
 */
void GameManager::setGameState(GameState state)
{
    // 如果状态没有变化，不做任何操作
    if (m_gameState == state) {
        return;
    }

    // 保存旧状态
    GameState oldState = m_gameState;
    
    // 设置新状态
    m_gameState = state;
    
    // 发出状态变化信号
    emit gameStateChanged(state);
    
    qDebug() << "[GameManager] 游戏状态从" << oldState << "变为" << state;
    
    // 根据新状态发出相应信号
    switch (state) {
        case Playing:
            emit gameStarted();
            break;
        case Paused:
            emit gamePaused();
            break;
        case GameOver:
            emit gameOver();
            break;
        default:
            break;
    }
}

/**
 * @brief 开始新游戏
 * 
 * 开始一个新游戏。
 * 执行以下操作：
 * 1. 初始化游戏世界（重置玩家、时间系统等）
 * 2. 设置游戏状态为 Playing
 * 3. 切换到初始场景（新伊甸）
 * 
 * @return true 新游戏开始成功
 * @return false 初始化游戏世界失败
 */
bool GameManager::startNewGame()
{
    qDebug() << "[GameManager] 开始新游戏...";

    // 1. 初始化游戏世界
    if (!initializeWorld()) {
        qCritical() << "[GameManager] 游戏世界初始化失败";
        return false;
    }

    // 2. 设置游戏状态为 Playing
    setGameState(Playing);
    
    // 3. 切换到初始场景（新伊甸）
    switchScene("new_eden");

    qDebug() << "[GameManager] 新游戏开始成功";
    return true;
}

/**
 * @brief 暂停游戏
 * 
 * 暂停游戏。
 * 只有在游戏状态为 Playing 时才会暂停。
 * 暂停后游戏状态变为 Paused。
 */
void GameManager::pauseGame()
{
    if (m_gameState == Playing) {
        qDebug() << "[GameManager] 暂停游戏";
        setGameState(Paused);
    }
}

/**
 * @brief 恢复游戏
 * 
 * 恢复游戏。
 * 只有在游戏状态为 Paused 时才会恢复。
 * 恢复后游戏状态变为 Playing。
 */
void GameManager::resumeGame()
{
    if (m_gameState == Paused) {
        qDebug() << "[GameManager] 恢复游戏";
        setGameState(Playing);
    }
}

/**
 * @brief 返回主菜单
 * 
 * 返回主菜单。
 * 执行以下操作：
 * 1. 设置游戏状态为主菜单
 * 2. 设置当前场景为主菜单
 * 3. 发出 sceneChanged 信号
 */
void GameManager::returnToMainMenu()
{
    qDebug() << "[GameManager] 返回主菜单";

    // 1. 设置游戏状态为主菜单
    setGameState(MainMenu);
    
    // 2. 设置当前场景为主菜单
    m_currentSceneName = "main_menu";
    
    // 3. 发出场景变化信号
    emit sceneChanged(m_currentSceneName);
}

// ============================================================
// 系统初始化
// ============================================================

/**
 * @brief 初始化游戏系统
 * 
 * 创建并初始化所有游戏系统：
 * 1. 创建并初始化玩家系统
 * 2. 创建并初始化时间系统
 * 3. 创建并初始化共鸣系统
 * 4. 创建并初始化生态系统
 * 
 * 如果任何一个系统初始化失败，返回 false。
 * 
 * 注意：
 * - 系统按顺序创建（玩家先创建，因为其他系统可能依赖玩家）
 * - 每个系统创建后立即调用 initialize()
 * - 如果初始化失败，直接返回 false，不清理已创建的系统
 * - 后续可以优化，添加失败时的清理逻辑
 * 
 * @return true 所有系统初始化成功
 * @return false 某个系统初始化失败
 */
bool GameManager::initializeSystems()
{
    qDebug() << "[GameManager] 初始化游戏系统...";

    // 1. 创建并初始化玩家系统
    qDebug() << "[GameManager] 创建玩家系统";
    m_player = new Player(this);
    if (!m_player->initialize()) {
        qCritical() << "[GameManager] 玩家系统初始化失败";
        return false;
    }

    // 2. 创建并初始化时间系统
    qDebug() << "[GameManager] 创建时间系统";
    m_timeSystem = new TimeSystem(this);
    if (!m_timeSystem->initialize()) {
        qCritical() << "[GameManager] 时间系统初始化失败";
        return false;
    }

    // 3. 创建并初始化共鸣系统
    qDebug() << "[GameManager] 创建共鸣系统";
    m_resonanceSystem = new ResonanceSystem(this);
    if (!m_resonanceSystem->initialize()) {
        qCritical() << "[GameManager] 共鸣系统初始化失败";
        return false;
    }

    // 4. 创建并初始化生态系统
    qDebug() << "[GameManager] 创建生态系统";
    m_ecosystemSystem = new EcosystemSystem(this);
    if (!m_ecosystemSystem->initialize()) {
        qCritical() << "[GameManager] 生态系统初始化失败";
        return false;
    }

    qDebug() << "[GameManager] 所有游戏系统初始化成功";
    return true;
}

/**
 * @brief 初始化游戏世界
 * 
 * 初始化游戏世界，为新游戏做准备：
 * 1. 重置游戏时间和天数
 * 2. 重置玩家状态（名称、等级、生命值、灵能值、位置等）
 * 3. 重置时间系统（重置时间熵）
 * 
 * 注意：
 * - 玩家初始位置是 (400, 300)，这是场景的中心位置
 * - 玩家初始名称是"旅者"
 * - 玩家初始等级是 1 级
 * - 玩家初始生命值是 100，满值
 * - 玩家初始灵能值是 50，满值
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
bool GameManager::initializeWorld()
{
    qDebug() << "[GameManager] 初始化游戏世界...";

    // 1. 重置游戏时间和天数
    m_gameTime = 0.0f;
    m_gameDay = 1;

    // 2. 重置玩家状态
    if (m_player) {
        qDebug() << "[GameManager] 重置玩家状态";
        m_player->setName("旅者");
        m_player->setLevel(1);
        m_player->setMaxHealth(100.0f);
        m_player->heal(100.0f);
        m_player->setMaxMana(50.0f);
        m_player->restoreMana(50.0f);
        m_player->setPosition(QPointF(400, 300));
    }

    // 3. 重置时间系统
    if (m_timeSystem) {
        qDebug() << "[GameManager] 重置时间系统";
        m_timeSystem->resetEntropy();
    }

    qDebug() << "[GameManager] 游戏世界初始化成功";
    return true;
}

// ============================================================
// 场景切换
// ============================================================

/**
 * @brief 切换场景
 * 
 * 切换到指定场景。
 * 切换成功后会发出 sceneChanged 信号。
 * 
 * 注意：
 * - 目前只是设置场景名称和发出信号
 * - 实际的场景加载和切换需要 SceneManager 配合
 * - 后续可以完善，添加场景加载和卸载逻辑
 * 
 * @param sceneName 场景名称
 * @return true 切换成功
 * @return false 场景名称为空
 */
bool GameManager::switchScene(const QString& sceneName)
{
    // 检查场景名称是否为空
    if (sceneName.isEmpty()) {
        qWarning() << "[GameManager] 场景名称为空，无法切换";
        return false;
    }

    // 设置当前场景名称
    m_currentSceneName = sceneName;
    
    // 发出场景变化信号
    emit sceneChanged(sceneName);

    qDebug() << "[GameManager] 切换到场景:" << sceneName;
    return true;
}

// ============================================================
// 存档系统
// ============================================================

/**
 * @brief 保存游戏
 * 
 * 保存游戏到指定存档槽位。
 * 
 * 注意：
 * - 目前是占位实现，完整的存档系统需要 SaveSystem
 * - 保存成功后会发出 gameSaved 信号
 * - 后续需要实现完整的存档逻辑
 * 
 * @param slot 存档槽位
 * @return true 保存成功
 * @return false 保存失败
 */
bool GameManager::saveGame(int slot)
{
    Q_UNUSED(slot);
    
    qDebug() << "[GameManager] 保存游戏到槽位" << slot;
    
    // TODO: 实现完整的存档系统
    // 完整的存档系统需要 SaveSystem，这里只做占位
    
    // 发出保存成功信号
    emit gameSaved(slot);
    
    return true;
}

/**
 * @brief 加载游戏
 * 
 * 从指定存档槽位加载游戏。
 * 
 * 注意：
 * - 目前是占位实现，完整的读档系统需要 SaveSystem
 * - 加载成功后会发出 gameLoaded 信号
 * - 后续需要实现完整的读档逻辑
 * 
 * @param slot 存档槽位
 * @return true 加载成功
 * @return false 加载失败
 */
bool GameManager::loadGame(int slot)
{
    Q_UNUSED(slot);
    
    qDebug() << "[GameManager] 从槽位" << slot << "加载游戏";
    
    // TODO: 实现完整的读档系统
    // 完整的读档系统需要 SaveSystem，这里只做占位
    
    // 发出加载成功信号
    emit gameLoaded(slot);
    
    return true;
}

/**
 * @brief 快速保存
 * 
 * 快速保存游戏到第 0 个存档槽位。
 * 这是一个便捷方法，相当于 saveGame(0)。
 * 
 * @return true 保存成功
 * @return false 保存失败
 */
bool GameManager::quickSave()
{
    qDebug() << "[GameManager] 快速保存";
    return saveGame(0);
}

/**
 * @brief 快速加载
 * 
 * 从第 0 个存档槽位快速加载游戏。
 * 这是一个便捷方法，相当于 loadGame(0)。
 * 
 * @return true 加载成功
 * @return false 加载失败
 */
bool GameManager::quickLoad()
{
    qDebug() << "[GameManager] 快速加载";
    return loadGame(0);
}
