/**
 * @file GameEngine.h
 * @brief 游戏引擎核心类
 * 
 * 这是游戏的核心引擎类，负责：
 * - 游戏的初始化和关闭
 * - 主循环管理（更新、渲染）
 * - 所有子系统的管理和协调
 * - FPS 统计和游戏时间管理
 * 
 * 游戏引擎采用单例模式，确保全局只有一个引擎实例。
 * 所有子系统通过引擎进行访问和协调。
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <QMainWindow>
#include <QString>

#include "GameConstants.h"

// ============================================================
// 前向声明
// ============================================================
// 使用前向声明减少头文件依赖，加快编译速度
class SceneManager;
class ResourceManager;
class InputManager;
class AudioManager;
class PluginManager;
class RenderManager;
class UIManager;
class GameManager;
class GameConfig;

/**
 * @brief 游戏引擎核心类
 * 
 * GameEngine 是整个游戏的核心控制器，负责管理所有子系统
 * 和游戏主循环。它采用模块化设计，各个子系统相对独立，
 * 通过引擎进行协调和通信。
 * 
 * 主要功能：
 * 1. 系统初始化：按顺序初始化所有子系统
 * 2. 主循环：定时触发更新和渲染
 * 3. 系统管理：提供各子系统的访问接口
 * 4. 时间管理：游戏时间、deltaTime、FPS统计
 * 5. 状态管理：游戏运行状态、暂停状态等
 * 
 * 使用示例：
 * @code
 * GameEngine engine;
 * if (engine.initialize()) {
 *     engine.run();
 * }
 * @endcode
 */
class GameEngine : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * 
     * 创建游戏引擎实例，但不进行初始化。
     * 必须调用 initialize() 完成初始化后才能使用。
     * 
     * @param parent 父对象，用于 Qt 对象树管理
     */
    explicit GameEngine(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     * 
     * 销毁游戏引擎，自动调用 shutdown() 清理资源。
     */
    ~GameEngine() override;

    // ============================================================
    // 生命周期管理
    // ============================================================
    
    /**
     * @brief 初始化游戏引擎
     * 
     * 按顺序执行以下初始化：
     * 1. 初始化主窗口
     * 2. 初始化所有子系统（资源、输入、音频、场景、插件、渲染、UI、游戏逻辑）
     * 3. 创建游戏主循环定时器
     * 
     * @return 是否初始化成功，失败时输出错误日志
     */
    bool initialize();
    
    /**
     * @brief 运行游戏主循环
     * 
     * 启动游戏定时器，显示主窗口，进入 Qt 事件循环。
     * 直到窗口关闭或调用 shutdown() 才会返回。
     * 
     * @return 程序退出码，0表示正常退出
     */
    int run();
    
    /**
     * @brief 关闭游戏引擎
     * 
     * 按初始化的逆序关闭所有子系统，释放资源。
     * 析构函数会自动调用此方法。
     */
    void shutdown();

    // ============================================================
    // 子系统访问接口
    // ============================================================
    
    /**
     * @brief 获取游戏配置
     * @return 游戏配置引用
     */
    GameConfig& getConfig() const;
    
    /**
     * @brief 获取场景管理器
     * @return 场景管理器指针，未初始化时返回 nullptr
     */
    SceneManager* getSceneManager() const { return m_sceneManager; }
    
    /**
     * @brief 获取资源管理器
     * @return 资源管理器指针，未初始化时返回 nullptr
     */
    ResourceManager* getResourceManager() const { return m_resourceManager; }
    
    /**
     * @brief 获取输入管理器
     * @return 输入管理器指针，未初始化时返回 nullptr
     */
    InputManager* getInputManager() const { return m_inputManager; }
    
    /**
     * @brief 获取音频管理器
     * @return 音频管理器指针，未初始化时返回 nullptr
     */
    AudioManager* getAudioManager() const { return m_audioManager; }
    
    /**
     * @brief 获取插件管理器
     * @return 插件管理器指针，未初始化时返回 nullptr
     */
    PluginManager* getPluginManager() const { return m_pluginManager; }
    
    /**
     * @brief 获取渲染管理器
     * @return 渲染管理器指针，未初始化时返回 nullptr
     */
    RenderManager* getRenderManager() const { return m_renderManager; }
    
    /**
     * @brief 获取 UI 管理器
     * @return UI 管理器指针，未初始化时返回 nullptr
     */
    UIManager* getUIManager() const { return m_uiManager; }
    
    /**
     * @brief 获取游戏管理器
     * @return 游戏管理器指针，未初始化时返回 nullptr
     */
    GameManager* getGameManager() const { return m_gameManager; }
    
    /**
     * @brief 获取主窗口
     * @return 主窗口指针，未初始化时返回 nullptr
     */
    QMainWindow* getMainWindow() const { return m_mainWindow; }

    // ============================================================
    // 时间和性能统计
    // ============================================================
    
    /**
     * @brief 获取当前 FPS
     * @return 每秒帧数，每秒更新一次
     */
    float getCurrentFPS() const { return m_currentFPS; }
    
    /**
     * @brief 获取游戏运行时间
     * @return 游戏运行时间（秒），不包含暂停时间
     */
    float getGameTime() const { return m_gameTime; }
    
    /**
     * @brief 获取当前帧的时间增量
     * @return 上一帧到当前帧的时间间隔（秒）
     */
    float getDeltaTime() const { return m_deltaTime; }

    // ============================================================
    // 游戏状态控制
    // ============================================================
    
    /**
     * @brief 检查游戏是否正在运行
     * @return true 表示游戏正在运行
     */
    bool isRunning() const { return m_running; }
    
    /**
     * @brief 检查游戏是否已暂停
     * @return true 表示游戏已暂停
     */
    bool isPaused() const { return m_paused; }
    
    /**
     * @brief 设置游戏暂停状态
     * @param paused true 暂停，false 继续
     */
    void setPaused(bool paused) { m_paused = paused; }
    
    /**
     * @brief 切换暂停状态
     */
    void togglePause() { m_paused = !m_paused; }

signals:
    /**
     * @brief 游戏初始化完成信号
     * 
     * 当所有子系统初始化完成后发出此信号。
     * 可以连接此信号来执行初始化后的操作。
     */
    void initialized();
    
    /**
     * @brief 游戏即将关闭信号
     * 
     * 在开始关闭子系统之前发出此信号。
     * 可以连接此信号来执行清理操作。
     */
    void shuttingDown();
    
    /**
     * @brief 游戏暂停状态改变信号
     * @param paused 新的暂停状态
     */
    void pauseStateChanged(bool paused);

private slots:
    /**
     * @brief 游戏主循环更新
     * 
     * 每帧调用一次，按顺序更新所有子系统。
     * 如果游戏处于暂停状态，则跳过逻辑更新。
     * 
     * @param deltaTime 时间增量（秒）
     */
    void update(float deltaTime);
    
    /**
     * @brief 渲染帧
     * 
     * 每帧调用一次，触发渲染系统渲染当前帧。
     * 注意：实际渲染由 Qt Graphics View 框架自动处理，
     * 这里主要用于统计和触发自定义渲染逻辑。
     */
    void render();

private:
    // ============================================================
    // 私有初始化方法
    // ============================================================
    
    /**
     * @brief 初始化所有子系统
     * 
     * 按依赖顺序创建并初始化子系统：
     * 1. 资源管理器（最先，其他系统可能需要资源）
     * 2. 输入管理器
     * 3. 音频管理器
     * 4. 场景管理器
     * 5. 插件管理器
     * 6. 渲染管理器（依赖场景和资源）
     * 7. UI 管理器（依赖渲染和主窗口）
     * 8. 游戏管理器（最后，依赖所有其他系统）
     * 
     * @return 是否所有子系统都初始化成功
     */
    bool initializeSubsystems();
    
    /**
     * @brief 初始化主窗口
     * 
     * 创建主窗口，设置标题和大小，创建中央视图。
     * 
     * @return 是否初始化成功
     */
    bool initializeMainWindow();
    
    /**
     * @brief 计算 FPS
     * 
     * 每秒更新一次 FPS 统计值。
     * 在调试模式下会输出 FPS 信息。
     */
    void calculateFPS();

    // ============================================================
    // 成员变量
    // ============================================================
    
    // 主窗口
    QMainWindow* m_mainWindow = nullptr;  ///< 主窗口指针
    
    // 子系统管理器（按初始化顺序排列）
    ResourceManager* m_resourceManager = nullptr;  ///< 资源管理器
    InputManager* m_inputManager = nullptr;        ///< 输入管理器
    AudioManager* m_audioManager = nullptr;        ///< 音频管理器
    SceneManager* m_sceneManager = nullptr;        ///< 场景管理器
    PluginManager* m_pluginManager = nullptr;      ///< 插件管理器
    RenderManager* m_renderManager = nullptr;      ///< 渲染管理器
    UIManager* m_uiManager = nullptr;              ///< UI 管理器
    GameManager* m_gameManager = nullptr;          ///< 游戏管理器
    
    // 主循环相关
    QTimer* m_gameTimer = nullptr;       ///< 游戏主循环定时器
    QElapsedTimer m_frameTimer;          ///< 帧时间计时器，用于计算 deltaTime
    
    // 时间相关
    float m_gameTime = 0.0f;             ///< 游戏总运行时间（秒）
    float m_deltaTime = 0.0f;            ///< 当前帧的时间增量（秒）
    
    // FPS 统计
    float m_currentFPS = 0.0f;           ///< 当前 FPS 值
    int m_frameCount = 0;                ///< FPS 统计区间内的帧数
    float m_fpsTimer = 0.0f;             ///< FPS 统计计时器
    
    // 游戏状态
    bool m_initialized = false;          ///< 是否已初始化
    bool m_running = false;              ///< 是否正在运行
    bool m_paused = false;               ///< 是否暂停
};

#endif // GAME_ENGINE_H
