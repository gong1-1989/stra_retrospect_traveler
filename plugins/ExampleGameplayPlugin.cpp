/**
 * @file ExampleGameplayPlugin.cpp
 * @brief 示例玩法插件实现
 * 
 * 本文件实现了示例玩法插件的所有功能。
 * 
 * 这个插件演示了如何：
 * - 实现 IGameplayPlugin 接口
 * - 处理游戏事件回调
 * - 修改游戏数据（如伤害值）
 * - 拦截游戏行为（如物品使用）
 * 
 * @author 星溯旅人开发团队
 * @version 1.0.0
 * @date 2026
 */
#include "ExampleGameplayPlugin.h"
#include <QDebug>

// ============================================================
// 构造函数与析构函数
// ============================================================

/**
 * @brief 构造函数
 */
ExampleGameplayPlugin::ExampleGameplayPlugin()
    : m_initialized(false)
    , m_eventCount(0)
    , m_damageMultiplier(1.0f)
    , m_currentScene("Unknown")
{
    qDebug() << "[ExampleGameplayPlugin] 创建示例玩法插件";
}

/**
 * @brief 析构函数
 */
ExampleGameplayPlugin::~ExampleGameplayPlugin()
{
    qDebug() << "[ExampleGameplayPlugin] 销毁示例玩法插件";
}

// ============================================================
// IPlugin 接口实现
// ============================================================

/**
 * @brief 获取插件名称
 * @return 插件名称
 */
QString ExampleGameplayPlugin::getName() const
{
    return "ExampleGameplayPlugin";
}

/**
 * @brief 获取插件版本
 * @return 版本字符串
 */
QString ExampleGameplayPlugin::getVersion() const
{
    return "1.0.0";
}

/**
 * @brief 获取插件作者
 * @return 作者名称
 */
QString ExampleGameplayPlugin::getAuthor() const
{
    return "星溯旅人开发团队";
}

/**
 * @brief 获取插件描述
 * @return 描述文本
 */
QString ExampleGameplayPlugin::getDescription() const
{
    return "示例玩法插件，演示游戏玩法插件的开发方法。"
           "提供事件日志、伤害修正、物品拦截等功能。";
}

/**
 * @brief 初始化插件
 * 
 * 初始化插件，准备运行。
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
bool ExampleGameplayPlugin::initialize()
{
    if (m_initialized) {
        qWarning() << "[ExampleGameplayPlugin] 插件已经初始化";
        return true;
    }
    
    qDebug() << "[ExampleGameplayPlugin] 初始化示例玩法插件...";
    
    // 初始化伤害倍率
    m_damageMultiplier = 1.0f;
    m_eventCount = 0;
    
    m_initialized = true;
    
    qDebug() << "[ExampleGameplayPlugin] 示例玩法插件初始化完成";
    return true;
}

/**
 * @brief 关闭插件
 * 
 * 关闭插件，清理资源。
 */
void ExampleGameplayPlugin::shutdown()
{
    if (!m_initialized) {
        return;
    }
    
    qDebug() << "[ExampleGameplayPlugin] 关闭示例玩法插件...";
    qDebug() << "[ExampleGameplayPlugin] 共处理" << m_eventCount << "个事件";
    
    m_initialized = false;
    
    qDebug() << "[ExampleGameplayPlugin] 示例玩法插件已关闭";
}

// ============================================================
// 游戏生命周期回调
// ============================================================

/**
 * @brief 游戏开始回调
 * 
 * 游戏开始时调用。
 */
void ExampleGameplayPlugin::onGameStart()
{
    m_eventCount++;
    qDebug() << "[ExampleGameplayPlugin] 游戏开始";
}

/**
 * @brief 游戏结束回调
 * 
 * 游戏结束时调用。
 */
void ExampleGameplayPlugin::onGameEnd()
{
    m_eventCount++;
    qDebug() << "[ExampleGameplayPlugin] 游戏结束";
}

/**
 * @brief 游戏暂停回调
 * 
 * 游戏暂停时调用。
 */
void ExampleGameplayPlugin::onGamePause()
{
    m_eventCount++;
    qDebug() << "[ExampleGameplayPlugin] 游戏暂停";
}

/**
 * @brief 游戏恢复回调
 * 
 * 游戏恢复时调用。
 */
void ExampleGameplayPlugin::onGameResume()
{
    m_eventCount++;
    qDebug() << "[ExampleGameplayPlugin] 游戏恢复";
}

// ============================================================
// 玩家相关回调
// ============================================================

/**
 * @brief 玩家升级回调
 * 
 * 玩家升级时调用。
 * 
 * @param newLevel 新等级
 * @param oldLevel 旧等级
 */
void ExampleGameplayPlugin::onPlayerLevelUp(int newLevel, int oldLevel)
{
    m_eventCount++;
    qDebug() << "[ExampleGameplayPlugin] 玩家升级:" << oldLevel << "->" << newLevel;
    
    // 升级时增加伤害倍率
    m_damageMultiplier += 0.01f;
    qDebug() << "[ExampleGameplayPlugin] 伤害倍率提升至:" << m_damageMultiplier;
}

/**
 * @brief 玩家死亡回调
 * 
 * 玩家死亡时调用。
 */
void ExampleGameplayPlugin::onPlayerDeath()
{
    m_eventCount++;
    qDebug() << "[ExampleGameplayPlugin] 玩家死亡";
}

/**
 * @brief 玩家复活回调
 * 
 * 玩家复活时调用。
 */
void ExampleGameplayPlugin::onPlayerRespawn()
{
    m_eventCount++;
    qDebug() << "[ExampleGameplayPlugin] 玩家复活";
}

// ============================================================
// 战斗相关回调
// ============================================================

/**
 * @brief 战斗开始回调
 * 
 * 战斗开始时调用。
 * 
 * @param enemyId 敌人ID
 */
void ExampleGameplayPlugin::onCombatStart(const QString& enemyId)
{
    m_eventCount++;
    qDebug() << "[ExampleGameplayPlugin] 战斗开始，敌人:" << enemyId;
}

/**
 * @brief 战斗结束回调
 * 
 * 战斗结束时调用。
 * 
 * @param victory 是否胜利
 * @param enemyId 敌人ID
 */
void ExampleGameplayPlugin::onCombatEnd(bool victory, const QString& enemyId)
{
    m_eventCount++;
    qDebug() << "[ExampleGameplayPlugin] 战斗结束，敌人:" << enemyId 
             << "结果:" << (victory ? "胜利" : "失败");
}

/**
 * @brief 造成伤害回调
 * 
 * 造成伤害时调用，可以修改伤害值。
 * 
 * @param damage 伤害值（引用，可修改）
 * @param source 伤害来源
 * @param target 伤害目标
 */
void ExampleGameplayPlugin::onDamageDealt(int& damage, const QString& source, const QString& target)
{
    m_eventCount++;
    
    int originalDamage = damage;
    damage = static_cast<int>(damage * m_damageMultiplier);
    
    qDebug() << "[ExampleGameplayPlugin] 造成伤害:" << source << "->" << target
             << "原始:" << originalDamage << "修正后:" << damage;
}

/**
 * @brief 受到伤害回调
 * 
 * 受到伤害时调用，可以修改伤害值。
 * 
 * @param damage 伤害值（引用，可修改）
 * @param source 伤害来源
 * @param target 伤害目标
 */
void ExampleGameplayPlugin::onDamageTaken(int& damage, const QString& source, const QString& target)
{
    m_eventCount++;
    
    qDebug() << "[ExampleGameplayPlugin] 受到伤害:" << source << "->" << target
             << "伤害值:" << damage;
}

// ============================================================
// 场景相关回调
// ============================================================

/**
 * @brief 进入场景回调
 * 
 * 进入场景时调用。
 * 
 * @param sceneName 场景名称
 */
void ExampleGameplayPlugin::onSceneEnter(const QString& sceneName)
{
    m_eventCount++;
    m_currentScene = sceneName;
    
    qDebug() << "[ExampleGameplayPlugin] 进入场景:" << sceneName;
}

/**
 * @brief 离开场景回调
 * 
 * 离开场景时调用。
 * 
 * @param sceneName 场景名称
 */
void ExampleGameplayPlugin::onSceneLeave(const QString& sceneName)
{
    m_eventCount++;
    
    qDebug() << "[ExampleGameplayPlugin] 离开场景:" << sceneName;
}

// ============================================================
// 物品相关回调
// ============================================================

/**
 * @brief 获得物品回调
 * 
 * 获得物品时调用。
 * 
 * @param itemId 物品ID
 * @param count 数量
 */
void ExampleGameplayPlugin::onItemAcquired(const QString& itemId, int count)
{
    m_eventCount++;
    
    qDebug() << "[ExampleGameplayPlugin] 获得物品:" << itemId << "x" << count;
}

/**
 * @brief 使用物品回调
 * 
 * 使用物品时调用。
 * 
 * @param itemId 物品ID
 * @return true 阻止默认行为
 * @return false 继续默认行为
 */
bool ExampleGameplayPlugin::onItemUsed(const QString& itemId)
{
    m_eventCount++;
    
    qDebug() << "[ExampleGameplayPlugin] 使用物品:" << itemId;
    
    // 示例：不阻止默认行为
    return false;
}

// ============================================================
// 任务相关回调
// ============================================================

/**
 * @brief 接受任务回调
 * 
 * 接受任务时调用。
 * 
 * @param questId 任务ID
 */
void ExampleGameplayPlugin::onQuestAccepted(const QString& questId)
{
    m_eventCount++;
    
    qDebug() << "[ExampleGameplayPlugin] 接受任务:" << questId;
}

/**
 * @brief 完成任务回调
 * 
 * 完成任务时调用。
 * 
 * @param questId 任务ID
 */
void ExampleGameplayPlugin::onQuestCompleted(const QString& questId)
{
    m_eventCount++;
    
    qDebug() << "[ExampleGameplayPlugin] 完成任务:" << questId;
}

// ============================================================
// 游戏更新回调
// ============================================================

/**
 * @brief 游戏更新回调
 * 
 * 每帧调用，用于插件的更新逻辑。
 * 
 * @param deltaTime 帧间隔时间（秒）
 */
void ExampleGameplayPlugin::onGameUpdate(float deltaTime)
{
    // 注意：不要在这里输出太多日志，会影响性能
    // 这里只是示例，实际插件可以在这里执行更新逻辑
    
    Q_UNUSED(deltaTime);
}
