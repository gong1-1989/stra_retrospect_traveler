/**
 * @file ExampleScene.h
 * @brief 示例场景 - 展示如何使用游戏系统
 * 
 * 本文件定义了示例场景类，展示如何创建一个游戏场景。
 * 示例场景包含玩家、地形、UI 等元素。
 * 
 * 主要功能：
 * 1. 地形创建：等距视角地形网格
 * 2. 玩家创建：玩家图形项
 * 3. 装饰创建：星星、标题等装饰元素
 * 4. 场景更新：每帧更新场景状态
 * 5. 玩家位置：设置和获取玩家位置
 * 
 * 场景特点：
 * - 2.5D 等距视角
 * - 深空蓝背景
 * - 青色玩家
 * - 星星装饰
 * - 场景标题
 * 
 * 使用示例：
 * @code
 * ExampleScene scene;
 * scene.initialize();
 * 
 * // 设置玩家位置
 * scene.setPlayerPosition(100, 50);
 * 
 * // 获取玩家位置
 * QPointF pos = scene.getPlayerPosition();
 * 
 * // 更新场景
 * scene.update(deltaTime);
 * @endcode
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#ifndef EXAMPLE_SCENE_H
#define EXAMPLE_SCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QPointF>
#include <QList>

/**
 * @brief 示例场景类
 * 
 * 展示如何创建一个游戏场景，包含玩家、地形、UI 等元素。
 * 
 * 架构说明：
 * - 继承 QGraphicsScene，使用 Qt Graphics View Framework
 * - 场景大小：2000x2000
 * - 背景颜色：深空蓝
 * - 玩家：青色矩形
 * - 地形：等距视角网格
 * - 装饰：星星和标题
 * 
 * 地形系统：
 * - 等距视角（Isometric）
 * - 网格大小：64像素
 * - 地图大小：20x15 格
 * - 交替颜色：深浅蓝色交替
 * - Z 值排序：确保正确的深度顺序
 * 
 * 玩家系统：
 * - 图形：矩形（32x32）
 * - 颜色：青色（代表星溯旅人）
 * - 位置：QPointF 存储
 * - Z 值：100（在地形上方）
 * 
 * 装饰系统：
 * - 星星：50个随机位置的白色星星
 * - 标题：场景名称文本
 * - Z 值：-100（在背景上方）
 * 
 * 使用示例：
 * @code
 * ExampleScene scene;
 * scene.initialize();
 * 
 * // 设置玩家位置
 * scene.setPlayerPosition(100, 50);
 * 
 * // 获取玩家位置
 * QPointF pos = scene.getPlayerPosition();
 * 
 * // 更新场景
 * scene.update(deltaTime);
 * @endcode
 */
class ExampleScene : public QGraphicsScene
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * 
     * 创建示例场景实例，但不进行初始化。
     * 必须调用 initialize() 完成初始化后才能使用。
     * 
     * @param parent 父对象，用于 Qt 对象树内存管理
     */
    explicit ExampleScene(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     * 
     * 销毁示例场景。
     * Qt 对象树会自动管理子对象的内存。
     */
    ~ExampleScene() override;

    // ============================================================
    // 生命周期管理
    // ============================================================
    
    /**
     * @brief 初始化场景
     * 
     * 初始化场景的所有元素。
     * 
     * 执行以下初始化操作：
     * 1. 检查是否已经初始化（防止重复初始化）
     * 2. 设置场景大小（2000x2000）
     * 3. 设置背景颜色（深空蓝）
     * 4. 创建地形
     * 5. 创建玩家
     * 6. 创建装饰
     * 7. 设置初始化标志
     * 
     * @return true 初始化成功
     * @return false 初始化失败
     */
    bool initialize();
    
    /**
     * @brief 更新场景
     * 
     * 每帧调用一次，更新场景状态。
     * 
     * 更新内容：
     * 1. 更新玩家位置
     * 
     * @param deltaTime 时间增量（秒）
     */
    void update(float deltaTime);

    // ============================================================
    // 玩家控制
    // ============================================================
    
    /**
     * @brief 设置玩家位置
     * 
     * 设置玩家在场景中的位置。
     * 
     * 执行以下操作：
     * 1. 更新玩家位置变量
     * 2. 更新玩家图形项的位置
     * 
     * @param x X坐标
     * @param y Y坐标
     */
    void setPlayerPosition(float x, float y);
    
    /**
     * @brief 获取玩家位置
     * 
     * 获取玩家在场景中的位置。
     * 
     * @return 位置
     */
    QPointF getPlayerPosition() const;

private:
    // ============================================================
    // 私有方法
    // ============================================================
    
    /**
     * @brief 创建地形
     * 
     * 创建等距视角的地形网格。
     * 
     * 地形参数：
     * - 网格大小：64像素
     * - 地图大小：20x15 格
     * - 交替颜色：深浅蓝色交替
     * - Z 值排序：确保正确的深度顺序
     * 
     * 执行以下操作：
     * 1. 循环创建地形格子
     * 2. 计算等距坐标
     * 3. 设置格子颜色
     * 4. 设置 Z 值
     * 5. 添加到场景
     */
    void createTerrain();
    
    /**
     * @brief 创建玩家
     * 
     * 创建玩家图形项。
     * 
     * 玩家参数：
     * - 图形：矩形（32x32）
     * - 颜色：青色（代表星溯旅人）
     * - 初始位置：(0, 0)
     * - Z 值：100（在地形上方）
     * 
     * 执行以下操作：
     * 1. 创建玩家矩形项
     * 2. 设置颜色和边框
     * 3. 设置初始位置
     * 4. 设置 Z 值
     * 5. 添加到场景
     */
    void createPlayer();
    
    /**
     * @brief 创建装饰
     * 
     * 创建场景装饰元素。
     * 
     * 装饰元素：
     * - 星星：50个随机位置的白色星星
     * - 标题：场景名称文本
     * 
     * 执行以下操作：
     * 1. 创建50个星星
     * 2. 创建场景标题
     * 3. 添加到场景
     */
    void createDecorations();

    // ============================================================
    // 成员变量
    // ============================================================
    
    // 玩家图形项
    QGraphicsRectItem* m_playerItem = nullptr;  ///< 玩家图形项指针
    
    // 地形项
    QList<QGraphicsRectItem*> m_terrainItems;  ///< 地形项列表
    
    // 装饰项
    QList<QGraphicsItem*> m_decorationItems;  ///< 装饰项列表
    
    // 玩家位置
    QPointF m_playerPosition;  ///< 玩家位置
    
    // 初始化状态
    bool m_initialized = false;  ///< 是否已初始化
};

#endif // EXAMPLE_SCENE_H
