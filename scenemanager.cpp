/**
 * @file SceneManager.cpp
 * @brief 场景管理器类实现
 * 
 * 本文件实现了场景管理器的所有功能，包括：
 * - 场景的创建和销毁
 * - 场景的切换和管理
 * - 当前场景的更新
 * 
 * 场景管理器基于 Qt Graphics View Framework，
 * 使用 QGraphicsScene 作为场景容器，管理游戏中的所有场景。
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#include "SceneManager.h"

#include <QDebug>

// ============================================================
// 构造函数与析构函数
// ============================================================

/**
 * @brief 构造函数
 * 
 * 创建场景管理器实例，但不进行任何初始化操作。
 * 所有的初始化工作都在 initialize() 中完成。
 * 
 * @param parent 父对象，用于 Qt 对象树内存管理
 */
SceneManager::SceneManager(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[SceneManager] 创建场景管理器";
}

/**
 * @brief 析构函数
 * 
 * 销毁场景管理器，自动调用 shutdown() 清理所有场景。
 */
SceneManager::~SceneManager()
{
    qDebug() << "[SceneManager] 销毁场景管理器";
    shutdown();
}

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
 * 默认场景确保场景管理器始终有一个可用的场景，
 * 避免在场景切换间隙出现空场景的情况。
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
bool SceneManager::initialize()
{
    // 防止重复初始化
    if (m_initialized) {
        qWarning() << "[SceneManager] 场景管理器已经初始化，跳过重复初始化";
        return true;
    }

    qDebug() << "[SceneManager] 初始化场景管理器...";

    // 1. 创建默认场景
    qDebug() << "[SceneManager] 创建默认场景";
    createScene("default");
    
    // 2. 切换到默认场景
    switchToScene("default");

    // 初始化完成
    m_initialized = true;
    
    qDebug() << "[SceneManager] 场景管理器初始化完成";
    return true;
}

/**
 * @brief 关闭场景管理器
 * 
 * 执行以下清理操作：
 * 1. 清理所有场景（自动删除 QGraphicsScene 对象）
 * 2. 重置当前场景指针和名称
 * 3. 重置状态
 * 
 * 注意：QGraphicsScene 对象会通过 qDeleteAll 自动删除，
 * 场景中的 QGraphicsItem 也会被自动删除（因为它们的父对象是场景）。
 * 
 * 此函数是幂等的，可以安全地多次调用。
 */
void SceneManager::shutdown()
{
    // 如果没有初始化，直接返回
    if (!m_initialized) {
        return;
    }

    qDebug() << "[SceneManager] 关闭场景管理器...";

    // 1. 清理所有场景
    qDebug() << "[SceneManager] 清理所有场景，数量:" << m_scenes.size();
    qDeleteAll(m_scenes);
    m_scenes.clear();

    // 2. 重置当前场景
    m_currentScene = nullptr;
    m_currentSceneName.clear();

    // 3. 重置状态
    m_initialized = false;
    
    qDebug() << "[SceneManager] 场景管理器已关闭";
}

// ============================================================
// 场景管理
// ============================================================

/**
 * @brief 创建新场景
 * 
 * 创建一个新的 QGraphicsScene 并添加到场景映射中。
 * 如果场景名称已存在，则返回已有的场景（不会创建新场景）。
 * 
 * 新场景的默认设置：
 * - 场景大小：1920 × 1080 像素
 * - 背景：默认（透明）
 * - 父对象：this（场景管理器）
 * 
 * @param sceneName 场景名称（唯一标识）
 * @return QGraphicsScene* 场景指针
 */
QGraphicsScene* SceneManager::createScene(const QString& sceneName)
{
    // 检查场景是否已存在
    if (m_scenes.contains(sceneName)) {
        qWarning() << "[SceneManager] 场景已存在:" << sceneName << "，返回已有场景";
        return m_scenes[sceneName];
    }

    qDebug() << "[SceneManager] 创建新场景:" << sceneName;

    // 1. 创建新场景
    QGraphicsScene* scene = new QGraphicsScene(this);
    
    // 2. 设置场景大小（默认 1920×1080）
    scene->setSceneRect(0, 0, 1920, 1080);
    
    // 3. 添加到场景映射
    m_scenes[sceneName] = scene;

    qDebug() << "[SceneManager] 场景创建成功，大小: 1920x1080";
    return scene;
}

/**
 * @brief 获取场景
 * 
 * 根据场景名称获取场景指针。
 * 如果场景不存在，返回 nullptr。
 * 
 * @param sceneName 场景名称
 * @return QGraphicsScene* 场景指针，不存在则返回 nullptr
 */
QGraphicsScene* SceneManager::getScene(const QString& sceneName) const
{
    return m_scenes.value(sceneName, nullptr);
}

/**
 * @brief 切换场景
 * 
 * 切换到指定名称的场景。
 * 如果场景不存在，返回 false。
 * 
 * 切换场景的操作：
 * 1. 检查场景是否存在
 * 2. 更新当前场景指针
 * 3. 更新当前场景名称
 * 4. 发出 sceneChanged 信号
 * 
 * 注意：
 * - 场景切换是即时的，没有过渡动画
 * - 如果需要过渡效果，可以在 sceneChanged 信号中处理
 * - 旧场景不会被删除，仍然保留在内存中
 * 
 * @param sceneName 场景名称
 * @return true 切换成功
 * @return false 场景不存在
 */
bool SceneManager::switchToScene(const QString& sceneName)
{
    // 检查场景是否存在
    if (!m_scenes.contains(sceneName)) {
        qWarning() << "[SceneManager] 场景不存在:" << sceneName;
        return false;
    }

    qDebug() << "[SceneManager] 切换到场景:" << sceneName;

    // 1. 更新当前场景指针
    m_currentScene = m_scenes[sceneName];
    
    // 2. 更新当前场景名称
    m_currentSceneName = sceneName;

    // 3. 发出场景切换信号
    emit sceneChanged(sceneName);

    qDebug() << "[SceneManager] 场景切换完成";
    return true;
}

// ============================================================
// 帧更新
// ============================================================

/**
 * @brief 更新场景
 * 
 * 每帧调用一次，更新当前场景。
 * 
 * 当前实现是空的，因为 QGraphicsScene 本身不支持帧更新。
 * 如果需要更新场景中的对象，可以：
 * 1. 遍历场景中的所有可更新对象
 * 2. 调用每个对象的 update 方法
 * 3. 或者使用 QGraphicsItem 的 advance() 机制
 * 
 * 注意：
 * - QGraphicsView 会自动渲染场景，不需要手动渲染
 * - 这里主要用于逻辑更新，不是渲染更新
 * 
 * @param deltaTime 时间增量（秒）
 */
void SceneManager::update(float deltaTime)
{
    Q_UNUSED(deltaTime);
    
    // 场景更新逻辑
    // 可以在这里更新场景中的所有可更新对象
    
    // 示例：使用 QGraphicsScene 的 advance() 机制
    // if (m_currentScene) {
    //     m_currentScene->advance();
    // }
}
