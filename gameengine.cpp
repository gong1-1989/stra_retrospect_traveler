/**
 * @file GameEngine.cpp
 * @brief 游戏引擎核心类实现
 * 
 * 本文件实现了游戏引擎的核心功能，包括：
 * - 游戏引擎的初始化和关闭
 * - 主循环的管理（更新、渲染）
 * - 所有子系统的创建、初始化和销毁
 * - FPS 统计和游戏时间管理
 * 
 * 游戏引擎采用模块化设计，各个子系统相对独立，
 * 通过引擎进行协调和通信。初始化顺序很重要，
 * 必须确保依赖的系统先初始化。
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#include "GameEngine.h"
#include "GameConfig.h"

#include <QDebug>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSize>

// ============================================================
// 子系统头文件
// ============================================================
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "PluginManager.h"
#include "RenderManager.h"
#include "UIManager.h"
#include "GameManager.h"

// ============================================================
// 构造函数与析构函数
// ============================================================

/**
 * @brief 构造函数
 * 
 * 创建游戏引擎实例，但不进行任何初始化操作。
 * 所有的初始化工作都在 initialize() 中完成，
 * 这样可以更好地处理初始化失败的情况。
 * 
 * @param parent 父对象，用于 Qt 对象树内存管理
 */
GameEngine::GameEngine(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[GameEngine] 创建游戏引擎实例";
}

/**
 * @brief 析构函数
 * 
 * 销毁游戏引擎，自动调用 shutdown() 确保所有资源
 * 都被正确释放。即使 initialize() 没有被调用，
 * 析构函数也是安全的。
 */
GameEngine::~GameEngine()
{
    qDebug() << "[GameEngine] 销毁游戏引擎实例";
    shutdown();
}

// ============================================================
// 生命周期管理
// ============================================================

/**
 * @brief 初始化游戏引擎
 * 
 * 按顺序执行以下初始化步骤：
 * 1. 检查是否已经初始化（防止重复初始化）
 * 2. 初始化主窗口
 * 3. 初始化所有子系统
 * 4. 创建游戏主循环定时器
 * 5. 设置定时器间隔（根据目标 FPS）
 * 
 * 如果任何一步失败，函数会立即返回 false。
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
bool GameEngine::initialize()
{
    // 防止重复初始化
    if (m_initialized) {
        qWarning() << "[GameEngine] 游戏引擎已经初始化，跳过重复初始化";
        return true;
    }

    qDebug() << "[GameEngine] ========================================";
    qDebug() << "[GameEngine] 开始初始化游戏引擎...";
    qDebug() << "[GameEngine] ========================================";

    // 1. 初始化主窗口
    if (!initializeMainWindow()) {
        qCritical() << "[GameEngine] 主窗口初始化失败！";
        return false;
    }

    // 2. 初始化所有子系统
    if (!initializeSubsystems()) {
        qCritical() << "[GameEngine] 子系统初始化失败！";
        return false;
    }

    // 3. 创建游戏主循环定时器
    m_gameTimer = new QTimer(this);
    
    // 连接定时器超时信号到主循环
    connect(m_gameTimer, &QTimer::timeout, this, [this]() {
        // 计算从上一帧到现在的时间间隔
        qint64 elapsed = m_frameTimer.restart();
        m_deltaTime = elapsed / 1000.0f;  // 转换为秒
        
        // 限制最大 deltaTime，防止卡顿后出现大跳跃
        const float MAX_DELTA_TIME = 0.1f;  // 最大 100ms
        if (m_deltaTime > MAX_DELTA_TIME) {
            m_deltaTime = MAX_DELTA_TIME;
        }
        
        // 更新游戏逻辑
        update(m_deltaTime);
        
        // 渲染当前帧
        render();
        
        // 计算 FPS 统计
        calculateFPS();
    });

    // 4. 设置定时器间隔（目标 FPS）
    GameConfig& config = GameConfig::instance();
    int interval = 1000 / config.targetFPS;
    m_gameTimer->setInterval(interval);
    
    qDebug() << "[GameEngine] 目标 FPS:" << config.targetFPS 
             << "，帧间隔:" << interval << "ms";

    // 初始化完成
    m_initialized = true;
    
    qDebug() << "[GameEngine] ========================================";
    qDebug() << "[GameEngine] 游戏引擎初始化完成！";
    qDebug() << "[GameEngine] ========================================";
    
    // 发出初始化完成信号
    emit initialized();
    
    return true;
}

/**
 * @brief 运行游戏主循环
 * 
 * 启动游戏定时器，显示主窗口，然后进入 Qt 事件循环。
 * 直到主窗口关闭或调用 shutdown() 才会返回。
 * 
 * @return int 程序退出码，0 表示正常退出
 */
int GameEngine::run()
{
    // 检查是否已初始化
    if (!m_initialized) {
        qCritical() << "[GameEngine] 游戏引擎未初始化，无法运行！";
        return -1;
    }

    qDebug() << "[GameEngine] 启动游戏主循环...";

    // 设置运行状态
    m_running = true;
    
    // 启动帧计时器
    m_frameTimer.start();
    
    // 启动游戏主循环定时器
    m_gameTimer->start();

    // 显示主窗口
    if (m_mainWindow) {
        GameConfig& config = GameConfig::instance();
        
        if (config.fullscreen) {
            qDebug() << "[GameEngine] 以全屏模式显示主窗口";
            m_mainWindow->showFullScreen();
        } else {
            qDebug() << "[GameEngine] 以窗口模式显示主窗口";
            m_mainWindow->show();
        }
    }

    // 进入 Qt 事件循环（阻塞调用）
    int result = QCoreApplication::exec();

    // 事件循环退出，游戏结束
    m_running = false;
    
    qDebug() << "[GameEngine] 游戏主循环结束，退出码:" << result;
    
    return result;
}

/**
 * @brief 关闭游戏引擎
 * 
 * 按初始化的逆序关闭所有子系统，释放所有资源。
 * 此函数是幂等的，可以安全地多次调用。
 */
void GameEngine::shutdown()
{
    // 如果没有初始化，直接返回
    if (!m_initialized) {
        return;
    }

    qDebug() << "[GameEngine] ========================================";
    qDebug() << "[GameEngine] 关闭游戏引擎...";
    qDebug() << "[GameEngine] ========================================";

    // 发出即将关闭信号
    emit shuttingDown();

    // 1. 停止游戏定时器
    if (m_gameTimer) {
        m_gameTimer->stop();
        qDebug() << "[GameEngine] 游戏定时器已停止";
    }

    // 2. 关闭游戏管理器（最后创建的最先关闭）
    if (m_gameManager) {
        qDebug() << "[GameEngine] 关闭游戏管理器...";
        m_gameManager->shutdown();
        delete m_gameManager;
        m_gameManager = nullptr;
    }
    
    // 3. 关闭 UI 管理器
    if (m_uiManager) {
        qDebug() << "[GameEngine] 关闭 UI 管理器...";
        m_uiManager->shutdown();
        delete m_uiManager;
        m_uiManager = nullptr;
    }
    
    // 4. 关闭渲染管理器
    if (m_renderManager) {
        qDebug() << "[GameEngine] 关闭渲染管理器...";
        m_renderManager->shutdown();
        delete m_renderManager;
        m_renderManager = nullptr;
    }
    
    // 5. 关闭插件管理器
    if (m_pluginManager) {
        qDebug() << "[GameEngine] 关闭插件管理器...";
        delete m_pluginManager;
        m_pluginManager = nullptr;
    }
    
    // 6. 关闭音频管理器
    if (m_audioManager) {
        qDebug() << "[GameEngine] 关闭音频管理器...";
        delete m_audioManager;
        m_audioManager = nullptr;
    }
    
    // 7. 关闭输入管理器
    if (m_inputManager) {
        qDebug() << "[GameEngine] 关闭输入管理器...";
        delete m_inputManager;
        m_inputManager = nullptr;
    }
    
    // 8. 关闭资源管理器
    if (m_resourceManager) {
        qDebug() << "[GameEngine] 关闭资源管理器...";
        delete m_resourceManager;
        m_resourceManager = nullptr;
    }
    
    // 9. 关闭场景管理器
    if (m_sceneManager) {
        qDebug() << "[GameEngine] 关闭场景管理器...";
        delete m_sceneManager;
        m_sceneManager = nullptr;
    }

    // 10. 关闭主窗口
    if (m_mainWindow) {
        qDebug() << "[GameEngine] 关闭主窗口...";
        m_mainWindow->close();
        delete m_mainWindow;
        m_mainWindow = nullptr;
    }

    // 重置状态
    m_initialized = false;
    m_running = false;
    m_gameTime = 0.0f;
    m_deltaTime = 0.0f;
    m_currentFPS = 0.0f;
    m_frameCount = 0;
    m_fpsTimer = 0.0f;
    
    qDebug() << "[GameEngine] ========================================";
    qDebug() << "[GameEngine] 游戏引擎已完全关闭";
    qDebug() << "[GameEngine] ========================================";
}

// ============================================================
// 子系统访问
// ============================================================

/**
 * @brief 获取游戏配置
 * 
 * 返回游戏配置单例的引用，用于读取和修改配置。
 * 
 * @return GameConfig& 游戏配置引用
 */
GameConfig& GameEngine::getConfig() const
{
    return GameConfig::instance();
}

// ============================================================
// 主循环
// ============================================================

/**
 * @brief 游戏主循环更新
 * 
 * 每帧调用一次，按顺序更新所有子系统。
 * 如果游戏处于暂停状态，则只更新 UI，不更新游戏逻辑。
 * 
 * @param deltaTime 从上一帧到现在的时间间隔（秒）
 */
void GameEngine::update(float deltaTime)
{
    // 累加游戏时间（仅在未暂停时）
    if (!m_paused) {
        m_gameTime += deltaTime;
    }

    // 1. 更新输入系统（总是更新）
    if (m_inputManager) {
        // 输入系统的更新由事件驱动
    }

    // 2. 更新游戏逻辑（仅在未暂停时）
    if (!m_paused && m_gameManager) {
        m_gameManager->update(deltaTime);
    }

    // 3. 更新渲染系统
    if (m_renderManager) {
        m_renderManager->update(deltaTime);
    }

    // 4. 更新 UI 系统（总是更新）
    if (m_uiManager) {
        m_uiManager->update(deltaTime);
    }

    // 5. 更新场景
    if (m_sceneManager) {
        // 场景更新由游戏管理器驱动
    }
}

/**
 * @brief 渲染帧
 * 
 * 每帧调用一次，主要用于帧计数。
 * 实际渲染由 Qt Graphics View 自动处理。
 */
void GameEngine::render()
{
    // 帧计数（用于 FPS 统计）
    m_frameCount++;
}

// ============================================================
// 私有初始化方法
// ============================================================

/**
 * @brief 初始化所有子系统
 * 
 * 按依赖顺序创建并初始化所有子系统。
 * 
 * @return true 所有子系统都初始化成功
 * @return false 某个子系统初始化失败
 */
bool GameEngine::initializeSubsystems()
{
    qDebug() << "[GameEngine] 初始化子系统...";

    // 1. 资源管理器
    qDebug() << "[GameEngine] [1/8] 创建资源管理器...";
    m_resourceManager = new ResourceManager();
    qDebug() << "[GameEngine] [1/8] 资源管理器创建完成";

    // 2. 输入管理器
    qDebug() << "[GameEngine] [2/8] 创建输入管理器...";
    m_inputManager = new InputManager();
    qDebug() << "[GameEngine] [2/8] 输入管理器创建完成";

    // 3. 音频管理器
    qDebug() << "[GameEngine] [3/8] 创建音频管理器...";
    m_audioManager = new AudioManager();
    qDebug() << "[GameEngine] [3/8] 音频管理器创建完成";

    // 4. 场景管理器
    qDebug() << "[GameEngine] [4/8] 创建场景管理器...";
    m_sceneManager = new SceneManager();
    qDebug() << "[GameEngine] [4/8] 场景管理器创建完成";

    // 5. 插件管理器
    qDebug() << "[GameEngine] [5/8] 创建插件管理器...";
    m_pluginManager = new PluginManager();
    qDebug() << "[GameEngine] [5/8] 插件管理器创建完成";

    // 6. 渲染管理器
    qDebug() << "[GameEngine] [6/8] 初始化渲染管理器...";
    m_renderManager = new RenderManager(this);
    
    QGraphicsView* view = m_mainWindow->findChild<QGraphicsView*>();
    if (view && !m_renderManager->initialize(view)) {
        qCritical() << "[GameEngine] [6/8] 渲染管理器初始化失败！";
        return false;
    }
    qDebug() << "[GameEngine] [6/8] 渲染管理器初始化完成";

    // 7. UI 管理器
    qDebug() << "[GameEngine] [7/8] 初始化 UI 管理器...";
    m_uiManager = new UIManager(this);
    if (!m_uiManager->initialize(m_mainWindow)) {
        qCritical() << "[GameEngine] [7/8] UI 管理器初始化失败！";
        return false;
    }
    qDebug() << "[GameEngine] [7/8] UI 管理器初始化完成";

    // 8. 游戏管理器
    qDebug() << "[GameEngine] [8/8] 初始化游戏管理器...";
    m_gameManager = new GameManager(this);
    if (!m_gameManager->initialize()) {
        qCritical() << "[GameEngine] [8/8] 游戏管理器初始化失败！";
        return false;
    }
    qDebug() << "[GameEngine] [8/8] 游戏管理器初始化完成";

    qDebug() << "[GameEngine] 所有子系统初始化完成！";
    return true;
}

/**
 * @brief 初始化主窗口
 * 
 * 创建主窗口并进行基本设置，包括：
 * - 窗口标题和大小
 * - 最小窗口大小
 * - 中央 QGraphicsView 视图
 * - 渲染提示设置
 * 
 * @return true 主窗口初始化成功
 * @return false 主窗口初始化失败
 */
bool GameEngine::initializeMainWindow()
{
    qDebug() << "[GameEngine] 初始化主窗口...";

    GameConfig& config = GameConfig::instance();

    // 创建主窗口
    m_mainWindow = new QMainWindow();
    
    // 设置窗口标题
    m_mainWindow->setWindowTitle(QString("%1 v%2")
        .arg(GameConstants::GAME_NAME)
        .arg(GameConstants::GAME_VERSION));
    
    // 设置窗口大小
    m_mainWindow->resize(config.windowSize);
    
    // 设置最小窗口大小
    m_mainWindow->setMinimumSize(QSize(
        GameConstants::MIN_WINDOW_WIDTH, 
        GameConstants::MIN_WINDOW_HEIGHT
    ));

    // 创建中央视图（QGraphicsView）
    QGraphicsView* view = new QGraphicsView(m_mainWindow);
    
    // 设置渲染提示
    view->setRenderHint(QPainter::Antialiasing, true);
    view->setRenderHint(QPainter::SmoothPixmapTransform, true);
    
    // 设置视口更新模式
    view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    
    // 设置滚动条策略（隐藏滚动条）
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    // 将视图设置为中央部件
    m_mainWindow->setCentralWidget(view);

    qDebug() << "[GameEngine] 主窗口初始化完成";
    qDebug() << "[GameEngine] 窗口大小:" << config.windowSize.width() 
             << "x" << config.windowSize.height();
    
    return true;
}

// ============================================================
// FPS 统计
// ============================================================

/**
 * @brief 计算 FPS（每秒帧数）
 * 
 * 每秒更新一次 FPS 统计值。
 * 在调试模式下会输出 FPS 信息到日志。
 */
void GameEngine::calculateFPS()
{
    // 累加时间
    m_fpsTimer += m_deltaTime;
    // m_frameCount 在 render() 中累加

    // 每秒更新一次 FPS
    const float FPS_UPDATE_INTERVAL = 1.0f;
    
    if (m_fpsTimer >= FPS_UPDATE_INTERVAL) {
        // 计算 FPS
        m_currentFPS = m_frameCount / m_fpsTimer;
        
        // 重置计数器
        m_frameCount = 0;
        m_fpsTimer = 0.0f;
        
        // 调试模式下输出 FPS 信息
        GameConfig& config = GameConfig::instance();
        if (config.debugMode && config.showFPS) {
            qDebug() << "[GameEngine] FPS:" << QString::number(m_currentFPS, 'f', 1)
                     << " 游戏时间:" << QString::number(m_gameTime, 'f', 1) << "s";
        }
    }
}
