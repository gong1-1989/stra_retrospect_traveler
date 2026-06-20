/**
 * @file ExampleContentPlugin.h
 * @brief 示例内容插件头文件
 * 
 * 本文件定义了一个示例内容插件，
 * 演示如何实现 IContentPlugin 接口。
 * 
 * 这个插件展示了如何向游戏添加自定义内容：
 * - 自定义场景
 * - 自定义角色
 * - 自定义物品
 * - 自定义任务
 * - 自定义技能
 * - 自定义敌人
 * - 自定义音频
 * 
 * @author 星溯旅人开发团队
 * @version 1.0.0
 * @date 2026
 */
#ifndef EXAMPLE_CONTENT_PLUGIN_H
#define EXAMPLE_CONTENT_PLUGIN_H

#include "IContentPlugin.h"
#include <QString>
#include <QList>

/**
 * @class ExampleContentPlugin
 * @brief 示例内容插件类
 * 
 * 这个类是一个示例内容插件，
 * 演示了如何实现 IContentPlugin 接口。
 * 
 * 插件功能：
 * - 添加示例场景
 * - 添加示例角色
 * - 添加示例物品
 * - 添加示例任务
 * - 添加示例技能
 * - 添加示例敌人
 * 
 * 使用场景：
 * - 学习内容插件开发
 * - 测试内容插件系统
 * - 作为内容插件开发模板
 */
class ExampleContentPlugin : public IContentPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.starretrospect.IContentPlugin" FILE "example_content_plugin.json")
    Q_INTERFACES(IContentPlugin IPlugin)
    
public:
    // ============================================================
    // 构造函数与析构函数
    // ============================================================
    
    /**
     * @brief 构造函数
     */
    ExampleContentPlugin();
    
    /**
     * @brief 析构函数
     */
    virtual ~ExampleContentPlugin();
    
    // ============================================================
    // IPlugin 接口实现
    // ============================================================
    
    QString getName() const override;
    QString getVersion() const override;
    QString getAuthor() const override;
    QString getDescription() const override;
    bool initialize() override;
    void shutdown() override;
    
    // ============================================================
    // 场景内容
    // ============================================================
    
    /**
     * @brief 获取所有场景ID
     * 
     * @return 场景ID列表
     */
    QStringList getScenes() const override;
    
    /**
     * @brief 获取场景信息
     * 
     * @param sceneId 场景ID
     * @return 场景信息
     */
    ContentItemInfo getSceneInfo(const QString& sceneId) const override;
    
    // ============================================================
    // 角色内容
    // ============================================================
    
    /**
     * @brief 获取所有角色ID
     * 
     * @return 角色ID列表
     */
    QStringList getCharacters() const override;
    
    /**
     * @brief 获取角色信息
     * 
     * @param characterId 角色ID
     * @return 角色信息
     */
    ContentItemInfo getCharacterInfo(const QString& characterId) const override;
    
    // ============================================================
    // 物品内容
    // ============================================================
    
    /**
     * @brief 获取所有物品ID
     * 
     * @return 物品ID列表
     */
    QStringList getItems() const override;
    
    /**
     * @brief 获取物品信息
     * 
     * @param itemId 物品ID
     * @return 物品信息
     */
    ContentItemInfo getItemInfo(const QString& itemId) const override;
    
    // ============================================================
    // 任务内容
    // ============================================================
    
    /**
     * @brief 获取所有任务ID
     * 
     * @return 任务ID列表
     */
    QStringList getQuests() const override;
    
    /**
     * @brief 获取任务信息
     * 
     * @param questId 任务ID
     * @return 任务信息
     */
    ContentItemInfo getQuestInfo(const QString& questId) const override;
    
    // ============================================================
    // 技能内容
    // ============================================================
    
    /**
     * @brief 获取所有技能ID
     * 
     * @return 技能ID列表
     */
    QStringList getSkills() const override;
    
    /**
     * @brief 获取技能信息
     * 
     * @param skillId 技能ID
     * @return 技能信息
     */
    ContentItemInfo getSkillInfo(const QString& skillId) const override;
    
    // ============================================================
    // 敌人内容
    // ============================================================
    
    /**
     * @brief 获取所有敌人ID
     * 
     * @return 敌人ID列表
     */
    QStringList getEnemies() const override;
    
    /**
     * @brief 获取敌人信息
     * 
     * @param enemyId 敌人ID
     * @return 敌人信息
     */
    ContentItemInfo getEnemyInfo(const QString& enemyId) const override;
    
    // ============================================================
    // 音频内容
    // ============================================================
    
    /**
     * @brief 获取背景音乐列表
     * 
     * @return 背景音乐ID列表
     */
    QStringList getBackgroundMusic() const override;
    
    /**
     * @brief 获取音效列表
     * 
     * @return 音效ID列表
     */
    QStringList getSoundEffects() const override;
    
    // ============================================================
    // 资源管理
    // ============================================================
    
    /**
     * @brief 获取资源前缀
     * 
     * @return 资源前缀路径
     */
    QString getResourcePrefix() const override;
    
    // ============================================================
    // 内容统计
    // ============================================================
    
    /**
     * @brief 获取内容数量统计
     * 
     * @return 各类内容数量的映射
     */
    QMap<QString, int> getContentCount() const override;
    
private:
    // ============================================================
    // 私有成员变量
    // ============================================================
    
    bool m_initialized;              ///< 是否已初始化
    
    // 内容数据
    QMap<QString, ContentItemInfo> m_scenes;      ///< 场景数据
    QMap<QString, ContentItemInfo> m_characters;  ///< 角色数据
    QMap<QString, ContentItemInfo> m_items;       ///< 物品数据
    QMap<QString, ContentItemInfo> m_quests;      ///< 任务数据
    QMap<QString, ContentItemInfo> m_skills;      ///< 技能数据
    QMap<QString, ContentItemInfo> m_enemies;     ///< 敌人数据
    
    QStringList m_bgmList;        ///< 背景音乐列表
    QStringList m_sfxList;        ///< 音效列表
};

#endif // EXAMPLE_CONTENT_PLUGIN_H
