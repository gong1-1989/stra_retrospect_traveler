/**
 * @file RenderManager.cpp
 * @brief 渲染管理器类实现
 * 
 * 本文件实现了渲染管理器的所有功能，包括：
 * - 2.5D 箱庭场景的渲染
 * - 场景的创建、切换和管理
 * - 渲染设置（抗锯齿、垂直同步、背景颜色等）
 * - 相机控制（位置、移动、缩放）
 * - 等距视角变换
 * 
 * 渲染管理器基于 Qt Graphics View Framework，
 * 使用 QGraphicsView 作为渲染视图，QGraphicsScene 作为场景容器。
 * 
 * 等距视角实现原理：
 * - 通过压缩 Y 轴（缩放 0.5 倍）来模拟等距视角
 * - 配合相机控制，可以实现 2.5D 效果
 * - 适用于箱庭式探索游戏
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#include "RenderManager.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QColor>
#include <algorithm>

// ============================================================
// 构造函数与析构函数
// ============================================================

/**
 * @brief 构造函数
 * 
 * 创建渲染管理器实例，但不进行任何初始化操作。
 * 所有的初始化工作都在 initialize() 中完成。
 * 
 * @param parent 父对象，用于 Qt 对象树内存管理
 */
RenderManager::RenderManager(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[RenderManager] 创建渲染管理器";
}

/**
 * @brief 析构函数
 * 
 * 销毁渲染管理器，自动调用 shutdown() 清理资源。
 */
RenderManager::~RenderManager()
{
    qDebug() << "[RenderManager] 销毁渲染管理器";
    shutdown();
}

// ============================================================
// 生命周期管理
// ============================================================

/**
 * @brief 初始化渲染系统
 * 
 * 执行以下初始化操作：
 * 1. 检查是否已经初始化（防止重复初始化）
 * 2. 验证视图指针是否有效
 * 3. 设置渲染提示（抗锯齿、平滑变换等）
 * 4. 设置视口更新模式
 * 5. 设置背景颜色
 * 6. 创建默认场景
 * 7. 切换到默认场景
 * 
 * @param view 图形视图指针（QGraphicsView）
 * @return true 初始化成功
 * @return false 初始化失败（视图指针无效）
 */
bool RenderManager::initialize(QGraphicsView* view)
{
    // 防止重复初始化
    if (m_initialized) {
        qWarning() << "[RenderManager] 渲染管理器已经初始化，跳过重复初始化";
        return true;
    }

    // 验证视图指针
    if (!view) {
        qCritical() << "[RenderManager] 无效的图形视图指针";
        return false;
    }

    qDebug() << "[RenderManager] 初始化渲染管理器...";

    // 1. 保存视图指针
    m_view = view;

    // 2. 设置渲染提示
    qDebug() << "[RenderManager] 设置渲染提示";
    m_view->setRenderHint(QPainter::Antialiasing, m_antialiasing);
    m_view->setRenderHint(QPainter::SmoothPixmapTransform, true);

    // 3. 设置视口更新模式（智能更新，只更新变化的区域）
    m_view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    // 4. 设置拖拽模式（无拖拽，由相机控制）
    m_view->setDragMode(QGraphicsView::NoDrag);

    // 5. 设置背景颜色（深空蓝）
    qDebug() << "[RenderManager] 设置背景颜色";
    m_view->setBackgroundBrush(QColor(20, 20, 40));

    // 6. 创建默认场景
    qDebug() << "[RenderManager] 创建默认场景";
    createScene("default");

    // 7. 切换到默认场景
    switchScene("default");

    // 初始化完成
    m_initialized = true;
    
    qDebug() << "[RenderManager] 渲染管理器初始化完成";
    return true;
}

/**
 * @brief 关闭渲染系统
 * 
 * 执行以下清理操作：
 * 1. 清理所有场景（自动删除 QGraphicsScene 对象）
 * 2. 重置当前场景指针
 * 3. 重置视图指针（不删除视图，因为不是我们创建的）
 * 4. 重置状态
 * 
 * 注意：不会删除 QGraphicsView，因为它不是渲染管理器创建的。
 * 
 * 此函数是幂等的，可以安全地多次调用。
 */
void RenderManager::shutdown()
{
    // 如果没有初始化，直接返回
    if (!m_initialized) {
        return;
    }

    qDebug() << "[RenderManager] 关闭渲染管理器...";

    // 1. 清理所有场景
    qDebug() << "[RenderManager] 清理所有场景，数量:" << m_scenes.size();
    qDeleteAll(m_scenes);
    m_scenes.clear();

    // 2. 重置当前场景
    m_currentScene = nullptr;

    // 3. 重置视图指针（不删除视图）
    m_view = nullptr;

    // 4. 重置状态
    m_initialized = false;
    
    qDebug() << "[RenderManager] 渲染管理器已关闭";
}

/**
 * @brief 更新渲染
 * 
 * 每帧调用一次，更新视图变换（相机位置、缩放等）。
 * 
 * 注意：
 * - QGraphicsView 会自动渲染场景，不需要手动渲染
 * - 这里主要用于更新视图变换（相机、缩放等）
 * 
 * @param deltaTime 时间增量（秒）
 */
void RenderManager::update(float deltaTime)
{
    Q_UNUSED(deltaTime);
    
    // 如果没有初始化或视图无效，直接返回
    if (!m_initialized || !m_view) {
        return;
    }

    // 更新视图变换（相机位置、缩放、等距视角等）
    updateViewTransform();
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
 * - 场景大小：4000 × 4000 像素
 * - 场景中心：原点 (0, 0)
 * - 父对象：this（渲染管理器）
 * 
 * @param name 场景名称（唯一标识）
 * @return QGraphicsScene* 场景指针
 */
QGraphicsScene* RenderManager::createScene(const QString& name)
{
    // 检查场景是否已存在
    if (m_scenes.contains(name)) {
        qWarning() << "[RenderManager] 场景已存在:" << name << "，返回已有场景";
        return m_scenes[name];
    }

    qDebug() << "[RenderManager] 创建新场景:" << name;

    // 1. 创建新场景
    QGraphicsScene* scene = new QGraphicsScene(this);
    
    // 2. 设置场景大小（4000×4000，中心在原点）
    scene->setSceneRect(-2000, -2000, 4000, 4000);
    
    // 3. 添加到场景映射
    m_scenes[name] = scene;

    qDebug() << "[RenderManager] 场景创建成功，大小: 4000x4000";
    return scene;
}

/**
 * @brief 获取场景
 * 
 * 根据场景名称获取场景指针。
 * 如果场景不存在，返回 nullptr。
 * 
 * @param name 场景名称
 * @return QGraphicsScene* 场景指针，不存在则返回 nullptr
 */
QGraphicsScene* RenderManager::getScene(const QString& name) const
{
    return m_scenes.value(name, nullptr);
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
 * 3. 设置视图的场景
 * 4. 发出 sceneChanged 信号
 * 
 * @param name 场景名称
 * @return true 切换成功
 * @return false 场景不存在
 */
bool RenderManager::switchScene(const QString& name)
{
    // 检查场景是否存在
    if (!m_scenes.contains(name)) {
        qWarning() << "[RenderManager] 场景不存在:" << name;
        return false;
    }

    qDebug() << "[RenderManager] 切换到场景:" << name;

    // 1. 更新当前场景指针
    m_currentScene = m_scenes[name];

    // 2. 设置视图的场景
    if (m_view) {
        m_view->setScene(m_currentScene);
    }

    // 3. 发出场景切换信号
    emit sceneChanged(name);

    qDebug() << "[RenderManager] 场景切换完成";
    return true;
}

/**
 * @brief 删除场景
 * 
 * 删除指定名称的场景。
 * 如果场景是当前场景，则不能删除，返回 false。
 * 
 * @param name 场景名称
 * @return true 删除成功
 * @return false 场景不存在或为当前场景
 */
bool RenderManager::removeScene(const QString& name)
{
    // 检查场景是否存在
    if (!m_scenes.contains(name)) {
        qWarning() << "[RenderManager] 场景不存在:" << name;
        return false;
    }

    // 检查是否为当前场景
    if (m_scenes[name] == m_currentScene) {
        qWarning() << "[RenderManager] 不能删除当前场景:" << name;
        return false;
    }

    qDebug() << "[RenderManager] 删除场景:" << name;

    // 1. 删除场景对象
    delete m_scenes[name];
    
    // 2. 从映射中移除
    m_scenes.remove(name);

    qDebug() << "[RenderManager] 场景删除成功";
    return true;
}

// ============================================================
// 渲染设置
// ============================================================

/**
 * @brief 设置背景颜色
 * 
 * 设置视图的背景刷颜色。
 * 
 * @param color 背景颜色
 */
void RenderManager::setBackgroundColor(const QColor& color)
{
    if (m_view) {
        m_view->setBackgroundBrush(color);
    }
}

/**
 * @brief 设置缩放
 * 
 * 设置视图的缩放比例。
 * 缩放范围限制在 0.1 到 5.0 之间，防止过度缩放。
 * 
 * 设置后会自动更新视图变换。
 * 
 * @param scale 缩放比例
 */
void RenderManager::setScale(float scale)
{
    // 限制缩放范围
    m_scale = std::max(0.1f, std::min(5.0f, scale));
    
    // 更新视图变换
    updateViewTransform();
}

/**
 * @brief 设置抗锯齿
 * 
 * 启用或禁用抗锯齿渲染。
 * 抗锯齿可以让图形边缘更平滑，但会降低性能。
 * 
 * @param enabled true 启用，false 禁用
 */
void RenderManager::setAntialiasing(bool enabled)
{
    m_antialiasing = enabled;
    
    if (m_view) {
        m_view->setRenderHint(QPainter::Antialiasing, enabled);
    }
}

/**
 * @brief 设置垂直同步
 * 
 * 启用或禁用垂直同步。
 * 垂直同步可以防止画面撕裂，但可能会增加输入延迟。
 * 
 * 注意：Qt 中 VSync 设置依赖于平台和视图配置，
 * 这里只是记录设置，实际效果取决于平台。
 * 
 * @param enabled true 启用，false 禁用
 */
void RenderManager::setVSync(bool enabled)
{
    m_vSync = enabled;
    
    // Qt 中 VSync 设置依赖于平台和视图配置
    qDebug() << "[RenderManager] 垂直同步设置为:" << enabled;
}

// ============================================================
// 相机控制
// ============================================================

/**
 * @brief 设置相机位置
 * 
 * 设置相机在场景中的位置（场景坐标）。
 * 相机会自动居中到该位置。
 * 
 * 设置后会自动更新视图变换。
 * 
 * @param x X 坐标
 * @param y Y 坐标
 */
void RenderManager::setCameraPosition(float x, float y)
{
    m_cameraPosition = QPointF(x, y);
    updateViewTransform();
}

/**
 * @brief 获取相机位置
 * @return QPointF 相机位置（场景坐标）
 */
QPointF RenderManager::getCameraPosition() const
{
    return m_cameraPosition;
}

/**
 * @brief 移动相机
 * 
 * 相对移动相机位置。
 * 
 * 移动后会自动更新视图变换。
 * 
 * @param dx X 偏移
 * @param dy Y 偏移
 */
void RenderManager::moveCamera(float dx, float dy)
{
    m_cameraPosition.setX(m_cameraPosition.x() + dx);
    m_cameraPosition.setY(m_cameraPosition.y() + dy);
    updateViewTransform();
}

// ============================================================
// 等距视角
// ============================================================

/**
 * @brief 设置等距视角
 * 
 * 启用或禁用等距视角变换。
 * 等距视角通过压缩 Y 轴来模拟 2.5D 效果。
 * 
 * 设置后会自动更新视图变换。
 * 
 * @param enabled true 启用，false 禁用
 */
void RenderManager::setIsometricView(bool enabled)
{
    m_isometricView = enabled;
    updateViewTransform();
}

// ============================================================
// 私有方法
// ============================================================

/**
 * @brief 更新视图变换
 * 
 * 根据当前的相机位置、缩放、等距视角设置，更新视图的变换矩阵。
 * 在相机位置、缩放或等距视角设置改变时调用。
 * 
 * 变换顺序（重要！顺序会影响最终效果）：
 * 1. 重置变换（清除所有变换）
 * 2. 应用缩放（等比缩放）
 * 3. 应用等距视角变换（如果启用）
 *    - 压缩 Y 轴（缩放 0.5 倍）
 *    - 旋转 0 度（可调整）
 * 4. 居中相机（将相机位置移动到视图中心）
 * 
 * 注意：
 * - 变换顺序很重要，不同的顺序会产生不同的效果
 * - 等距视角的 Y 轴压缩比例可以根据需要调整
 * - 相机居中是通过 centerOn() 实现的，会自动考虑视图大小
 */
void RenderManager::updateViewTransform()
{
    // 如果视图无效，直接返回
    if (!m_view) {
        return;
    }

    // 1. 重置变换（清除所有之前的变换）
    m_view->resetTransform();

    // 2. 应用缩放（等比缩放 X 和 Y）
    m_view->scale(m_scale, m_scale);

    // 3. 应用等距视角变换（如果启用）
    if (m_isometricView) {
        // 简单的等距视角模拟：压缩 Y 轴
        // Y 轴压缩 0.5 倍，让图形看起来有高度感
        m_view->scale(1.0f, 0.5f);
        
        // 可选：旋转视角（标准等距视角通常旋转 45 度）
        // 这里不旋转，保持正视角
        m_view->rotate(0);
    }

    // 4. 居中相机（将相机位置移动到视图中心）
    // centerOn() 会自动考虑视图的大小和当前变换
    m_view->centerOn(m_cameraPosition);
}
