/**
 * @file GameManager.h
 * @brief 游戏管理器类
 * 
 * 游戏管理器是游戏的核心协调器，负责：
 * - 游戏状态管理（主菜单、加载中、游戏中、暂停、游戏结束、胜利）
 * - 游戏系统的初始化和管理（玩家、时间系统、共鸣系统、生态系统）
 * - 游戏时间和天数管理
 * - 场景切换管理
 * - 存档和读档管理
 * 
 * 主要特性：
 * 1. 状态机：清晰的游戏状态管理，支持状态切换和信号通知
 * 2. 系统协调：统一管理所有游戏系统，确保它们正确初始化和更新
 * 3. 时间管理：游戏内时间和天数的追踪
 * 4. 场景管理：场景切换和当前场景追踪
 * 5. 存档系统：支持多槽位存档和快速存档
 * 
 * 游戏状态：
 * - MainMenu：主菜单
 * - Loading：加载中
 * - Playing：游戏中
 * - Paused：暂停
 * - GameOver：游戏结束
 * - Victory：胜利
 * 
 * 使用示例：
 * @code
 * GameManager gameManager;
 * gameManager.initialize();
 * 
 * // 开始新游戏
 * gameManager.startNewGame();
 * 
 * // 暂停游戏
 * gameManager.pauseGame();
 * 
 * // 恢复游戏
 * gameManager.resumeGame();
 * 
 * // 保存游戏
 * gameManager.saveGame(1);
 * 
 * // 获取玩家
 * Player* player = gameManager.getPlayer();
 * @endcode
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <QObject>
#include <QString>
#include <QPointF>

#include "Player.h"
#include "TimeSystem.h"
#include "ResonanceSystem.h"
#include "EcosystemSystem.h"

/**
 * @brief 游戏管理器类
 * 
 * 负责协调游戏各个系统，管理游戏状态。
 * 游戏管理器是游戏的核心，统一管理所有游戏系统和状态。
 * 
 * 架构说明：
 * - 游戏管理器作为游戏系统的容器和协调者
 * - 所有游戏系统（玩家、时间系统、共鸣系统、生态系统）都由游戏管理器创建和管理
 * - 游戏状态通过状态机管理，状态变化时发出相应信号
 * - 游戏时间和天数由游戏管理器统一追踪
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
 * 注意：
 * - 游戏管理器是单例吗？不，目前不是单例，可以有多个实例
 * - 游戏系统由游戏管理器管理，不需要手动删除
 * - 状态变化会发出信号，可以连接信号来响应状态变化
 */
class GameManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 游戏状态枚举
     * 
     * 定义游戏的所有可能状态。
     * 游戏状态通过状态机管理，状态变化时发出相应信号。
     */
    enum GameState {
        MainMenu = 0,      ///< 主菜单
        Loading = 1,       ///< 加载中
        Playing = 2,       ///< 游戏中
        Paused = 3,        ///< 暂停
        GameOver = 4,      ///< 游戏结束
        Victory = 5        ///< 胜利
    };

    /**
     * @brief 构造函数
     * 
     * 创建游戏管理器实例，但不进行初始化。
     * 必须调用 initialize() 完成初始化后才能使用。
     * 
     * @param parent 父对象，用于 Qt 对象树内存管理
     */
    explicit GameManager(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     * 
     * 销毁游戏管理器，自动调用 shutdown() 清理所有游戏系统。
     */
    ~GameManager() override;

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
    bool initialize();
    
    /**
     * @brief 关闭游戏
     * 
     * 清理所有游戏系统，释放资源。
     * 所有游戏系统对象会被自动删除。
     * 
     * 此函数是幂等的，可以安全地多次调用。
     */
    void shutdown();
    
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
     * @param deltaTime 时间增量（秒）
     */
    void update(float deltaTime);

    // ============================================================
    // 游戏状态管理
    // ============================================================
    
    /**
     * @brief 获取当前游戏状态
     * @return 游戏状态
     */
    GameState getGameState() const { return m_gameState; }
    
    /**
     * @brief 设置游戏状态
     * 
     * 设置新的游戏状态。
     * 如果状态没有变化，不做任何操作。
     * 
     * 状态变化时会：
     * 1. 发出 gameStateChanged 信号
     * 2. 根据新状态发出相应信号（gameStarted、gamePaused、gameOver 等）
     * 
     * @param state 新的游戏状态
     */
    void setGameState(GameState state);
    
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
    bool startNewGame();
    
    /**
     * @brief 暂停游戏
     * 
     * 暂停游戏。
     * 只有在游戏状态为 Playing 时才会暂停。
     * 暂停后游戏状态变为 Paused。
     */
    void pauseGame();
    
    /**
     * @brief 恢复游戏
     * 
     * 恢复游戏。
     * 只有在游戏状态为 Paused 时才会恢复。
     * 恢复后游戏状态变为 Playing。
     */
    void resumeGame();
    
    /**
     * @brief 返回主菜单
     * 
     * 返回主菜单。
     * 执行以下操作：
     * 1. 设置游戏状态为主菜单
     * 2. 设置当前场景为主菜单
     * 3. 发出 sceneChanged 信号
     */
    void returnToMainMenu();

    // ============================================================
    // 系统访问
    // ============================================================
    
    /**
     * @brief 获取玩家
     * @return 玩家指针
     */
    Player* getPlayer() const { return m_player; }
    
    /**
     * @brief 获取时间系统
     * @return 时间系统指针
     */
    TimeSystem* getTimeSystem() const { return m_timeSystem; }
    
    /**
     * @brief 获取星核共鸣系统
     * @return 共鸣系统指针
     */
    ResonanceSystem* getResonanceSystem() const { return m_resonanceSystem; }
    
    /**
     * @brief 获取生态共生系统
     * @return 生态系统指针
     */
    EcosystemSystem* getEcosystemSystem() const { return m_ecosystemSystem; }

    // ============================================================
    // 游戏信息
    // ============================================================
    
    /**
     * @brief 获取游戏时间
     * @return 游戏时间（秒）
     */
    float getGameTime() const { return m_gameTime; }
    
    /**
     * @brief 获取游戏天数
     * @return 天数
     */
    int getGameDay() const { return m_gameDay; }
    
    /**
     * @brief 获取当前场景名称
     * @return 场景名称
     */
    QString getCurrentSceneName() const { return m_currentSceneName; }
    
    /**
     * @brief 切换场景
     * 
     * 切换到指定场景。
     * 切换成功后会发出 sceneChanged 信号。
     * 
     * @param sceneName 场景名称
     * @return true 切换成功
     * @return false 场景名称为空
     */
    bool switchScene(const QString& sceneName);

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
     * 
     * @param slot 存档槽位
     * @return true 保存成功
     * @return false 保存失败
     */
    bool saveGame(int slot);
    
    /**
     * @brief 加载游戏
     * 
     * 从指定存档槽位加载游戏。
     * 
     * 注意：
     * - 目前是占位实现，完整的读档系统需要 SaveSystem
     * - 加载成功后会发出 gameLoaded 信号
     * 
     * @param slot 存档槽位
     * @return true 加载成功
     * @return false 加载失败
     */
    bool loadGame(int slot);
    
    /**
     * @brief 快速保存
     * 
     * 快速保存游戏到第 0 个存档槽位。
     * 这是一个便捷方法，相当于 saveGame(0)。
     * 
     * @return true 保存成功
     * @return false 保存失败
     */
    bool quickSave();
    
    /**
     * @brief 快速加载
     * 
     * 从第 0 个存档槽位快速加载游戏。
     * 这是一个便捷方法，相当于 loadGame(0)。
     * 
     * @return true 加载成功
     * @return false 加载失败
     */
    bool quickLoad();

signals:
    /**
     * @brief 游戏状态变化信号
     * 
     * 当游戏状态变化时发出，携带新状态。
     * 可以连接此信号来响应游戏状态变化。
     * 
     * @param newState 新状态
     */
    void gameStateChanged(GameState newState);
    
    /**
     * @brief 游戏开始信号
     * 
     * 当游戏开始（状态变为 Playing）时发出。
     */
    void gameStarted();
    
    /**
     * @brief 游戏暂停信号
     * 
     * 当游戏暂停（状态变为 Paused）时发出。
     */
    void gamePaused();
    
    /**
     * @brief 游戏恢复信号
     * 
     * 当游戏恢复（状态从 Paused 变为 Playing）时发出。
     */
    void gameResumed();
    
    /**
     * @brief 游戏结束信号
     * 
     * 当游戏结束（状态变为 GameOver）时发出。
     */
    void gameOver();
    
    /**
     * @brief 场景切换信号
     * 
     * 当场景切换时发出，携带新场景名称。
     * 
     * @param sceneName 新场景名称
     */
    void sceneChanged(const QString& sceneName);
    
    /**
     * @brief 游戏保存信号
     * 
     * 当游戏保存成功后发出，携带存档槽位。
     * 
     * @param slot 存档槽位
     */
    void gameSaved(int slot);
    
    /**
     * @brief 游戏加载信号
     * 
     * 当游戏加载成功后发出，携带存档槽位。
     * 
     * @param slot 存档槽位
     */
    void gameLoaded(int slot);

private:
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
     * @return true 所有系统初始化成功
     * @return false 某个系统初始化失败
     */
    bool initializeSystems();
    
    /**
     * @brief 初始化游戏世界
     * 
     * 初始化游戏世界，为新游戏做准备：
     * 1. 重置游戏时间和天数
     * 2. 重置玩家状态（名称、等级、生命值、灵能值、位置等）
     * 3. 重置时间系统（重置时间熵）
     * 
     * @return true 初始化成功
     * @return false 初始化失败
     */
    bool initializeWorld();

    // ============================================================
    // 成员变量
    // ============================================================
    
    // 游戏状态
    GameState m_gameState = MainMenu;  ///< 当前游戏状态
    
    // 游戏时间
    float m_gameTime = 0.0f;  ///< 游戏时间（秒）
    int m_gameDay = 1;        ///< 游戏天数
    
    // 当前场景
    QString m_currentSceneName = "main_menu";  ///< 当前场景名称
    
    // 游戏系统
    Player* m_player = nullptr;                 ///< 玩家系统
    TimeSystem* m_timeSystem = nullptr;         ///< 时间系统
    ResonanceSystem* m_resonanceSystem = nullptr;  ///< 星核共鸣系统
    EcosystemSystem* m_ecosystemSystem = nullptr;  ///< 生态共生系统
    
    // 状态
    bool m_initialized = false;  ///< 是否已初始化
};

#endif // GAME_MANAGER_H
