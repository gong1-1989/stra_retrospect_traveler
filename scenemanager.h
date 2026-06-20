/**
 * @file SceneManager.h
 * @brief 场景管理器类
 * 
 * 场景管理器负责游戏中所有场景的管理、切换和渲染，包括：
 * - 场景的创建和销毁
 * - 场景的切换和过渡
 * - 当前场景的管理
 * - 场景的更新
 * 
 * 主要特性：
 * 1. 场景管理：支持多个场景并存，通过名称标识
 * 2. 场景切换：支持快速切换场景，发出切换信号
 * 3. 场景更新：每帧更新当前场景
 * 4. 默认场景：自动创建默认场景，确保始终有可用场景
 * 
 * 使用示例：
 * @code
 * SceneManager sceneManager;
 * sceneManager.initialize();
 * 
 * // 创建场景
 * QGraphicsScene* menuScene = sceneManager.createScene("menu");
 * QGraphicsScene* gameScene = sceneManager.createScene("game");
 * 
 * // 切换场景
 * sceneManager.switchToScene("game");
 * 
 * // 在游戏循环中更新
 * sceneManager.update(deltaTime);
 * @endcode
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <QObject>
#include <QGraphicsScene>
#include <QMap>
#include <QString>

/**
 * @brief 场景管理器类
 * 
 * 负责游戏场景的管理、切换和渲染。
 * 基于 Qt Graphics View Framework，使用 QGraphicsScene 作为场景容器。
 * 
 * 场景架构：
 * - 每个场景是一个 QGraphicsScene，包含所有游戏对象
 * - 场景通过名称标识，支持多个场景并存
 * - 同一时间只有一个场景是活动的（当前场景）
 * - 场景切换时发出 sceneChanged 信号
 * 
 * 注意：
 * - 场景对象由 SceneManager 管理，不需要手动删除
 * - 场景切换是即时的，没有过渡动画
 * - 如果需要过渡效果，可以在 sceneChanged 信号中处理
 */
class SceneManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * 
     * 创建场景管理器实例，但不进行初始化。
     * 必须调用 initialize() 完成初始化后才能使用。
     * 
     * @param parent 父对象，用于 Qt 对象树内存管理
     */
    explicit SceneManager(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     * 
     * 销毁场景管理器，自动调用 shutdown() 清理所有场景。
     */
    ~SceneManager() override;

    // ============================================================
    // 生命周期管理
    // ============================================================
    
    /**
     * @brief 初始化场景管理器
     * 
     * 执行以下初始化操作：
     * 1. 检查是否已经初始化（防止重复初始化）
     * 2. 创建默认场景
     * 3. 切换到默认场景
     * 
     * @return true 初始化成功
     * @return false 初始化失败
     */
    bool initialize();
    
    /**
     * @brief 关闭场景管理器
     * 
     * 清理所有场景，释放资源。
     * 所有场景对象会被自动删除。
     */
    void shutdown();

    // ============================================================
    // 场景管理
    // ============================================================
    
    /**
     * @brief 创建新场景
     * 
     * 创建一个新的 QGraphicsScene 并添加到场景映射中。
     * 如果场景名称已存在，则返回已有的场景。
     * 
     * 新场景的默认大小为 1920×1080。
     * 
     * @param sceneName 场景名称（唯一标识）
     * @return QGraphicsScene* 场景指针
     */
    QGraphicsScene* createScene(const QString& sceneName);
    
    /**
     * @brief 获取场景
     * 
     * 根据场景名称获取场景指针。
     * 如果场景不存在，返回 nullptr。
     * 
     * @param sceneName 场景名称
     * @return QGraphicsScene* 场景指针，不存在则返回 nullptr
     */
    QGraphicsScene* getScene(const QString& sceneName) const;
    
    /**
     * @brief 切换场景
     * 
     * 切换到指定名称的场景。
     * 如果场景不存在，返回 false。
     * 
     * 切换成功后会发出 sceneChanged 信号。
     * 
     * @param sceneName 场景名称
     * @return true 切换成功
     * @return false 场景不存在
     */
    bool switchToScene(const QString& sceneName);
    
    /**
     * @brief 获取当前场景
     * @return QGraphicsScene* 当前场景指针
     */
    QGraphicsScene* getCurrentScene() const { return m_currentScene; }
    
    /**
     * @brief 获取当前场景名称
     * @return QString 当前场景名称
     */
    QString getCurrentSceneName() const { return m_currentSceneName; }

    // ============================================================
    // 帧更新
    // ============================================================
    
    /**
     * @brief 更新场景
     * 
     * 每帧调用一次，更新当前场景。
     * 可以在这里更新场景中的所有可更新对象。
     * 
     * @param deltaTime 时间增量（秒）
     */
    void update(float deltaTime);

signals:
    /**
     * @brief 场景切换信号
     * 
     * 当场景切换完成后发出，携带新场景名称。
     * 可以连接此信号来处理场景切换逻辑（如播放过渡动画、加载资源等）。
     * 
     * @param sceneName 新场景名称
     */
    void sceneChanged(const QString& sceneName);

private:
    // ============================================================
    // 成员变量
    // ============================================================
    
    // 场景管理
    QMap<QString, QGraphicsScene*> m_scenes;  ///< 场景映射（名称 -> 场景指针）
    
    // 当前场景
    QGraphicsScene* m_currentScene = nullptr;  ///< 当前活动场景
    QString m_currentSceneName;                ///< 当前活动场景名称
    
    // 状态
    bool m_initialized = false;  ///< 是否已初始化
};

#endif // SCENE_MANAGER_H
