/**
 * @file ExampleGameplayPlugin.h
 * @brief 示例玩法插件头文件
 * 
 * 本文件定义了一个示例玩法插件，
 * 演示如何实现 IGameplayPlugin 接口。
 * 
 * 这个插件展示了以下功能：
 * - 游戏生命周期回调
 * - 玩家相关回调
 * - 战斗相关回调
 * - 场景相关回调
 * - 物品相关回调
 * - 任务相关回调
 * - 游戏更新回调
 * 
 * 使用方法：
 * 1. 继承 IGameplayPlugin
 * 2. 实现所有纯虚函数
 * 3. 使用 Q_PLUGIN_METADATA 宏声明插件
 * 4. 使用 Q_INTERFACES 宏声明接口
 * 
 * @author 星溯旅人开发团队
 * @version 1.0.0
 * @date 2026
 */
#ifndef EXAMPLE_GAMEPLAY_PLUGIN_H
#define EXAMPLE_GAMEPLAY_PLUGIN_H

#include "IGameplayPlugin.h"
#include <QString>

/**
 * @class ExampleGameplayPlugin
 * @brief 示例玩法插件类
 * 
 * 这个类是一个示例玩法插件，
 * 演示了如何实现 IGameplayPlugin 接口。
 * 
 * 插件功能：
 * - 记录游戏事件日志
 * - 提供伤害修正功能
 * - 演示物品使用拦截
 * 
 * 使用场景：
 * - 学习插件开发
 * - 测试插件系统
 * - 作为插件开发模板
 * 
 * 注意：
 * - 这是一个示例插件，实际使用时需要根据需求修改
 * - 所有回调方法都是可选的，可以只实现需要的部分
 */
class ExampleGameplayPlugin : public IGameplayPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.starretrospect.IGameplayPlugin" FILE "example_gameplay_plugin.json")
    Q_INTERFACES(IGameplayPlugin IPlugin)
    
public:
    // ============================================================
    // 构造函数与析构函数
    // ============================================================
    
    /**
     * @brief 构造函数
     */
    ExampleGameplayPlugin();
    
    /**
     * @brief 析构函数
     */
    virtual ~ExampleGameplayPlugin();
    
    // ============================================================
    // IPlugin 接口实现
    // ============================================================
    
    /**
     * @brief 获取插件名称
     * @return 插件名称
     */
    QString getName() const override;
    
    /**
     * @brief 获取插件版本
     * @return 版本字符串
     */
    QString getVersion() const override;
    
    /**
     * @brief 获取插件作者
     * @return 作者名称
     */
    QString getAuthor() const override;
    
    /**
     * @brief 获取插件描述
     * @return 描述文本
     */
    QString getDescription() const override;
    
    /**
     * @brief 初始化插件
     * 
     * 初始化插件，准备运行。
     * 
     * @return true 初始化成功
     * @return false 初始化失败
     */
    bool initialize() override;
    
    /**
     * @brief 关闭插件
     * 
     * 关闭插件，清理资源。
     */
    void shutdown() override;
    
    // ============================================================
    // 游戏生命周期回调
    // ============================================================
    
    /**
     * @brief 游戏开始回调
     * 
     * 游戏开始时调用。
     */
    void onGameStart() override;
    
    /**
     * @brief 游戏结束回调
     * 
     * 游戏结束时调用。
     */
    void onGameEnd() override;
    
    /**
     * @brief 游戏暂停回调
     * 
     * 游戏暂停时调用。
     */
    void onGamePause() override;
    
    /**
     * @brief 游戏恢复回调
     * 
     * 游戏恢复时调用。
     */
    void onGameResume() override;
    
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
    void onPlayerLevelUp(int newLevel, int oldLevel) override;
    
    /**
     * @brief 玩家死亡回调
     * 
     * 玩家死亡时调用。
     */
    void onPlayerDeath() override;
    
    /**
     * @brief 玩家复活回调
     * 
     * 玩家复活时调用。
     */
    void onPlayerRespawn() override;
    
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
    void onCombatStart(const QString& enemyId) override;
    
    /**
     * @brief 战斗结束回调
     * 
     * 战斗结束时调用。
     * 
     * @param victory 是否胜利
     * @param enemyId 敌人ID
     */
    void onCombatEnd(bool victory, const QString& enemyId) override;
    
    /**
     * @brief 造成伤害回调
     * 
     * 造成伤害时调用，可以修改伤害值。
     * 
     * @param damage 伤害值（引用，可修改）
     * @param source 伤害来源
     * @param target 伤害目标
     */
    void onDamageDealt(int& damage, const QString& source, const QString& target) override;
    
    /**
     * @brief 受到伤害回调
     * 
     * 受到伤害时调用，可以修改伤害值。
     * 
     * @param damage 伤害值（引用，可修改）
     * @param source 伤害来源
     * @param target 伤害目标
     */
    void onDamageTaken(int& damage, const QString& source, const QString& target) override;
    
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
    void onSceneEnter(const QString& sceneName) override;
    
    /**
     * @brief 离开场景回调
     * 
     * 离开场景时调用。
     * 
     * @param sceneName 场景名称
     */
    void onSceneLeave(const QString& sceneName) override;
    
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
    void onItemAcquired(const QString& itemId, int count) override;
    
    /**
     * @brief 使用物品回调
     * 
     * 使用物品时调用。
     * 
     * @param itemId 物品ID
     * @return true 阻止默认行为
     * @return false 继续默认行为
     */
    bool onItemUsed(const QString& itemId) override;
    
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
    void onQuestAccepted(const QString& questId) override;
    
    /**
     * @brief 完成任务回调
     * 
     * 完成任务时调用。
     * 
     * @param questId 任务ID
     */
    void onQuestCompleted(const QString& questId) override;
    
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
    void onGameUpdate(float deltaTime) override;
    
private:
    // ============================================================
    // 私有成员变量
    // ============================================================
    
    bool m_initialized;          ///< 是否已初始化
    int m_eventCount;            ///< 事件计数
    float m_damageMultiplier;    ///< 伤害倍率
    QString m_currentScene;      ///< 当前场景
};

#endif // EXAMPLE_GAMEPLAY_PLUGIN_H
