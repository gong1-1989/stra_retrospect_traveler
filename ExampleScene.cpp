/**
 * @file ExampleScene.cpp
 * @brief 示例场景实现
 * 
 * 本文件实现了示例场景的所有功能。
 * 示例场景展示了如何创建一个游戏场景。
 * 
 * 主要功能：
 * 1. 地形创建：等距视角地形网格
 * 2. 玩家创建：玩家图形项
 * 3. 装饰创建：星星、标题等装饰元素
 * 4. 场景更新：每帧更新场景状态
 * 5. 玩家位置：设置和获取玩家位置
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#include "ExampleScene.h"
#include <QDebug>
#include <QBrush>
#include <QPen>
#include <QGraphicsEllipseItem>
#include <QFont>

// ============================================================
// 构造函数和析构函数
// ============================================================

/**
 * @brief 构造函数
 * 
 * 创建示例场景实例。
 * 初始化所有成员变量为默认值。
 * 
 * 注意：
 * - 构造函数不进行实际的初始化工作
 * - 必须调用 initialize() 完成初始化后才能使用
 * - 父对象会自动管理内存
 * 
 * @param parent 父对象
 */
ExampleScene::ExampleScene(QObject* parent)
    : QGraphicsScene(parent)
{
    // 成员变量在头文件中已初始化
    // 这里不需要额外操作
}

/**
 * @brief 析构函数
 * 
 * 销毁示例场景。
 * Qt 对象树会自动管理子对象的内存。
 * 
 * 注意：
 * - Qt 的 QGraphicsScene 会自动删除所有子项
 * - 不需要手动删除图形项
 */
ExampleScene::~ExampleScene()
{
    // Qt 对象树会自动管理子对象的内存
    // 不需要手动删除图形项
}

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
 * 注意：
 * - 此函数是幂等的，可以安全地多次调用
 * - 如果已经初始化，会直接返回 true
 * - 初始化失败会返回 false
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
bool ExampleScene::initialize()
{
    // 检查是否已经初始化
    if (m_initialized) {
        qWarning() << "ExampleScene already initialized";
        return true;
    }

    qDebug() << "Initializing ExampleScene...";
    
    // 设置场景大小（2000x2000）
    setSceneRect(-1000, -1000, 2000, 2000);
    
    // 设置背景颜色（深空蓝）
    setBackgroundBrush(QBrush(QColor(20, 20, 50)));
    
    // 创建地形
    createTerrain();
    
    // 创建玩家
    createPlayer();
    
    // 创建装饰
    createDecorations();
    
    // 设置初始化标志
    m_initialized = true;
    
    qDebug() << "ExampleScene initialized successfully";
    return true;
}

/**
 * @brief 更新场景
 * 
 * 每帧调用一次，更新场景状态。
 * 
 * 更新内容：
 * 1. 更新玩家位置
 * 
 * 注意：
 * - 此函数应该每帧调用一次
 * - deltaTime 是上一帧到这一帧的时间间隔
 * - 目前只更新玩家位置，后续可以添加更多更新逻辑
 * 
 * @param deltaTime 时间增量（秒）
 */
void ExampleScene::update(float deltaTime)
{
    Q_UNUSED(deltaTime);
    
    // 检查是否已经初始化
    if (!m_initialized) {
        return;
    }

    // 更新玩家位置
    if (m_playerItem) {
        m_playerItem->setPos(m_playerPosition);
    }
}

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
 * 注意：
 * - 会同时更新位置变量和图形项
 * - 确保视觉和逻辑位置一致
 * 
 * @param x X坐标
 * @param y Y坐标
 */
void ExampleScene::setPlayerPosition(float x, float y)
{
    // 更新玩家位置变量
    m_playerPosition = QPointF(x, y);
    
    // 更新玩家图形项的位置
    if (m_playerItem) {
        m_playerItem->setPos(m_playerPosition);
    }
}

/**
 * @brief 获取玩家位置
 * 
 * 获取玩家在场景中的位置。
 * 
 * 注意：
 * - 返回的是逻辑位置，不是图形项的位置
 * - 两者应该保持一致
 * 
 * @return 位置
 */
QPointF ExampleScene::getPlayerPosition() const
{
    return m_playerPosition;
}

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
 * 
 * 等距坐标计算公式：
 * - isoX = (x - y) * gridSize * 0.5
 * - isoY = (x + y) * gridSize * 0.25
 * 
 * 注意：
 * - 等距视角（Isometric）是 2.5D 游戏常用的视角
 * - Z 值用于深度排序，确保正确的遮挡关系
 * - 交替颜色使地形更有层次感
 */
void ExampleScene::createTerrain()
{
    // 地形参数
    const int gridSize = 64;      // 网格大小（像素）
    const int mapWidth = 20;      // 地图宽度（格数）
    const int mapHeight = 15;     // 地图高度（格数）
    
    // 循环创建地形格子
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            // 计算等距坐标
            // 等距视角公式：
            // isoX = (x - y) * gridSize * 0.5
            // isoY = (x + y) * gridSize * 0.25
            float isoX = (x - y) * gridSize * 0.5f;
            float isoY = (x + y) * gridSize * 0.25f;
            
            // 创建地块
            // 注意：等距视角的地块是菱形，但我们用矩形近似
            QGraphicsRectItem* tile = new QGraphicsRectItem(
                isoX, isoY, gridSize, gridSize * 0.5f
            );
            
            // 根据位置设置不同颜色
            // 交替颜色使地形更有层次感
            QColor tileColor;
            if ((x + y) % 2 == 0) {
                tileColor = QColor(60, 80, 120);  // 浅蓝色
            } else {
                tileColor = QColor(50, 70, 110);  // 深蓝色
            }
            
            // 设置格子颜色和边框
            tile->setBrush(QBrush(tileColor));
            tile->setPen(QPen(QColor(40, 50, 80), 1));
            
            // 设置 Z 值（深度排序）
            // Z 值越大，显示在越上层
            // 我们用 y + x * 0.1 来确保正确的深度顺序
            tile->setZValue(y + x * 0.1f);
            
            // 添加到场景
            addItem(tile);
            m_terrainItems.append(tile);
        }
    }
    
    qDebug() << "Created" << m_terrainItems.size() << "terrain tiles";
}

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
 * 
 * 注意：
 * - 玩家用简单的矩形表示，后续可以替换为精灵图
 * - 青色代表星溯旅人，与游戏主题相符
 * - Z 值设为 100，确保玩家在所有地形上方
 * - 初始位置在场景中心 (0, 0)
 */
void ExampleScene::createPlayer()
{
    // 创建玩家图形（简单的矩形代表）
    // 注意：矩形大小为 32x32，原点在中心
    m_playerItem = new QGraphicsRectItem(-16, -32, 32, 32);
    
    // 设置玩家颜色（青色，代表星溯旅人）
    // 青色与游戏的科幻主题相符
    m_playerItem->setBrush(QBrush(QColor(100, 220, 255)));
    m_playerItem->setPen(QPen(QColor(255, 255, 255), 2));
    
    // 设置玩家初始位置
    // 初始位置在场景中心 (0, 0)
    m_playerPosition = QPointF(0, 0);
    m_playerItem->setPos(m_playerPosition);
    
    // 设置 Z 值（玩家在地形上方）
    // Z 值设为 100，确保玩家在所有地形和装饰上方
    m_playerItem->setZValue(100);
    
    // 添加到场景
    addItem(m_playerItem);
    
    qDebug() << "Created player at position:" << m_playerPosition;
}

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
 * 
 * 注意：
 * - 星星用椭圆表示，大小随机
 * - 星星位置随机分布在场景上方
 * - Z 值设为 -100，确保星星在背景上方但在地形下方
 * - 标题用 QGraphicsSimpleTextItem 显示
 * - 标题字体加粗，大小为 24 点
 */
void ExampleScene::createDecorations()
{
    // 创建一些装饰性的星星
    // 星星数量：50个
    for (int i = 0; i < 50; i++) {
        // 随机位置
        // X 范围：-1000 到 1000
        // Y 范围：-800 到 200（主要在上半部分）
        float x = (rand() % 2000) - 1000;
        float y = (rand() % 1000) - 800;
        
        // 随机大小（1-4 像素）
        float size = (rand() % 3) + 1;
        
        // 创建星星（用椭圆表示）
        QGraphicsEllipseItem* star = new QGraphicsEllipseItem(
            x, y, size, size
        );
        
        // 设置星星颜色（白色，半透明）
        // 半透明使星星看起来更柔和
        star->setBrush(QBrush(QColor(255, 255, 255, 200)));
        star->setPen(Qt::NoPen);
        
        // 设置 Z 值（在背景上方，但在地形下方）
        // Z 值设为 -100，确保星星在背景上方
        star->setZValue(-100);
        
        // 添加到场景
        addItem(star);
        m_decorationItems.append(star);
    }
    
    // 添加场景标题
    // 标题文本：星溯旅人 - 示例场景
    QGraphicsSimpleTextItem* title = new QGraphicsSimpleTextItem("星溯旅人 - 示例场景");
    
    // 设置标题位置（场景上方居中）
    title->setPos(-150, -400);
    
    // 设置标题颜色（青色，与玩家颜色一致）
    title->setBrush(QBrush(QColor(100, 220, 255)));
    
    // 设置标题字体
    // 字体大小：24 点，加粗
    QFont font;
    font.setPointSize(24);
    font.setBold(true);
    title->setFont(font);
    
    // 设置 Z 值（在地形上方，但在玩家下方）
    // Z 值设为 50，确保标题在地形上方但在玩家下方
    title->setZValue(50);
    
    // 添加到场景
    addItem(title);
    m_decorationItems.append(title);
    
    qDebug() << "Created" << m_decorationItems.size() << "decoration items";
}
