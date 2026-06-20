/**
 * @file RenderManager.h
 * @brief 渲染管理器类
 * 
 * 渲染管理器负责游戏中所有渲染相关的管理，包括：
 * - 2.5D 箱庭场景的渲染
 * - 场景的创建、切换和管理
 * - 渲染设置（抗锯齿、垂直同步、背景颜色等）
 * - 相机控制（位置、移动、缩放）
 * - 等距视角变换
 * 
 * 主要特性：
 * 1. 2.5D 渲染：基于 Qt Graphics View Framework，支持等距视角
 * 2. 场景管理：支持多个场景并存，通过名称标识
 * 3. 相机控制：支持相机位置、缩放、移动
 * 4. 渲染设置：抗锯齿、平滑变换、背景颜色等
 * 5. 等距视角：模拟 2.5D 等距视角效果
 * 
 * 使用示例：
 * @code
 * RenderManager renderManager;
 * renderManager.initialize(graphicsView);
 * 
 * // 创建场景
 * QGraphicsScene* scene = renderManager.createScene("game");
 * 
 * // 设置相机位置
 * renderManager.setCameraPosition(0, 0);
 * 
 * // 设置缩放
 * renderManager.setScale(2.0f);
 * 
 * // 启用等距视角
 * renderManager.setIsometricView(true);
 * @endcode
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMap>
#include <QString>
#include <QPointF>
#include <QColor>

/**
 * @brief 渲染管理器类
 * 
 * 负责 2.5D 箱庭场景的渲染管理。
 * 基于 Qt Graphics View Framework，提供场景管理、相机控制、渲染设置等功能。
 * 
 * 渲染架构：
 * - 使用 QGraphicsView 作为渲染视图
 * - 使用 QGraphicsScene 作为场景容器
 * - 支持等距视角变换（模拟 2.5D 效果）
 * - 支持相机控制（位置、缩放、移动）
 * 
 * 等距视角说明：
 * - 通过压缩 Y 轴和旋转来模拟等距视角
 * - 等距视角可以让 2D 图形呈现 3D 效果
 * - 适用于箱庭式探索游戏
 * 
 * 注意：
 * - 渲染管理器不拥有 QGraphicsView 的所有权
 * - 场景对象由渲染管理器管理，不需要手动删除
 * - 相机位置是场景坐标，不是屏幕坐标
 */
class RenderManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * 
     * 创建渲染管理器实例，但不进行初始化。
     * 必须调用 initialize() 完成初始化后才能使用。
     * 
     * @param parent 父对象，用于 Qt 对象树内存管理
     */
    explicit RenderManager(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     * 
     * 销毁渲染管理器，自动调用 shutdown() 清理资源。
     */
    ~RenderManager() override;

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
    bool initialize(QGraphicsView* view);
    
    /**
     * @brief 关闭渲染系统
     * 
     * 清理所有场景，释放资源。
     * 注意：不会删除 QGraphicsView，因为它不是渲染管理器创建的。
     */
    void shutdown();
    
    /**
     * @brief 更新渲染
     * 
     * 每帧调用一次，更新视图变换（相机位置、缩放等）。
     * 
     * @param deltaTime 时间增量（秒）
     */
    void update(float deltaTime);

    // ============================================================
    // 场景管理
    // ============================================================
    
    /**
     * @brief 获取当前场景
     * @return QGraphicsScene* 当前场景指针
     */
    QGraphicsScene* getCurrentScene() const { return m_currentScene; }
    
    /**
     * @brief 创建新场景
     * 
     * 创建一个新的 QGraphicsScene 并添加到场景映射中。
     * 如果场景名称已存在，则返回已有的场景。
     * 
     * 新场景的默认大小：4000×4000 像素，中心在原点。
     * 
     * @param name 场景名称（唯一标识）
     * @return QGraphicsScene* 场景指针
     */
    QGraphicsScene* createScene(const QString& name);
    
    /**
     * @brief 获取场景
     * 
     * 根据场景名称获取场景指针。
     * 如果场景不存在，返回 nullptr。
     * 
     * @param name 场景名称
     * @return QGraphicsScene* 场景指针，不存在则返回 nullptr
     */
    QGraphicsScene* getScene(const QString& name) const;
    
    /**
     * @brief 切换场景
     * 
     * 切换到指定名称的场景。
     * 如果场景不存在，返回 false。
     * 
     * 切换成功后会发出 sceneChanged 信号。
     * 
     * @param name 场景名称
     * @return true 切换成功
     * @return false 场景不存在
     */
    bool switchScene(const QString& name);
    
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
    bool removeScene(const QString& name);

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
    void setBackgroundColor(const QColor& color);
    
    /**
     * @brief 设置缩放
     * 
     * 设置视图的缩放比例。
     * 缩放范围限制在 0.1 到 5.0 之间。
     * 
     * @param scale 缩放比例
     */
    void setScale(float scale);
    
    /**
     * @brief 获取缩放
     * @return float 缩放比例
     */
    float getScale() const { return m_scale; }
    
    /**
     * @brief 设置抗锯齿
     * 
     * 启用或禁用抗锯齿渲染。
     * 抗锯齿可以让图形边缘更平滑，但会降低性能。
     * 
     * @param enabled true 启用，false 禁用
     */
    void setAntialiasing(bool enabled);
    
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
    void setVSync(bool enabled);

    // ============================================================
    // 相机控制
    // ============================================================
    
    /**
     * @brief 设置相机位置
     * 
     * 设置相机在场景中的位置（场景坐标）。
     * 相机会自动居中到该位置。
     * 
     * @param x X 坐标
     * @param y Y 坐标
     */
    void setCameraPosition(float x, float y);
    
    /**
     * @brief 获取相机位置
     * @return QPointF 相机位置（场景坐标）
     */
    QPointF getCameraPosition() const;
    
    /**
     * @brief 移动相机
     * 
     * 相对移动相机位置。
     * 
     * @param dx X 偏移
     * @param dy Y 偏移
     */
    void moveCamera(float dx, float dy);

    // ============================================================
    // 等距视角
    // ============================================================
    
    /**
     * @brief 设置等距视角
     * 
     * 启用或禁用等距视角变换。
     * 等距视角通过压缩 Y 轴来模拟 2.5D 效果。
     * 
     * @param enabled true 启用，false 禁用
     */
    void setIsometricView(bool enabled);
    
    /**
     * @brief 是否为等距视角
     * @return true 等距视角已启用
     * @return false 等距视角已禁用
     */
    bool isIsometricView() const { return m_isometricView; }

signals:
    /**
     * @brief 场景切换信号
     * 
     * 当场景切换完成后发出，携带新场景名称。
     * 
     * @param sceneName 新场景名称
     */
    void sceneChanged(const QString& sceneName);
    
    /**
     * @brief 渲染错误信号
     * 
     * 当发生渲染错误时发出，携带错误信息。
     * 
     * @param error 错误信息
     */
    void renderError(const QString& error);

private:
    /**
     * @brief 更新视图变换
     * 
     * 根据当前的相机位置、缩放、等距视角设置，更新视图的变换矩阵。
     * 在相机位置、缩放或等距视角设置改变时调用。
     * 
     * 变换顺序：
     * 1. 重置变换
     * 2. 应用缩放
     * 3. 应用等距视角变换（如果启用）
     * 4. 居中相机
     */
    void updateViewTransform();

    // ============================================================
    // 成员变量
    // ============================================================
    
    // 视图和场景
    QGraphicsView* m_view = nullptr;           ///< 图形视图指针（不拥有所有权）
    QGraphicsScene* m_currentScene = nullptr;  ///< 当前活动场景
    QMap<QString, QGraphicsScene*> m_scenes;   ///< 场景映射（名称 -> 场景指针）
    
    // 渲染设置
    float m_scale = 1.0f;        ///< 缩放比例
    bool m_antialiasing = true;  ///< 是否启用抗锯齿
    bool m_vSync = true;         ///< 是否启用垂直同步
    
    // 相机
    QPointF m_cameraPosition;  ///< 相机位置（场景坐标）
    
    // 等距视角
    bool m_isometricView = true;  ///< 是否启用等距视角
    
    // 状态
    bool m_initialized = false;  ///< 是否已初始化
};

#endif // RENDER_MANAGER_H
