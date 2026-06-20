/**
 * @file ExampleContentPlugin.cpp
 * @brief 示例内容插件实现
 * 
 * 本文件实现了示例内容插件的所有功能。
 * 
 * 这个插件演示了如何：
 * - 实现 IContentPlugin 接口
 * - 向游戏添加自定义内容
 * - 提供内容信息和元数据
 * 
 * @author 星溯旅人开发团队
 * @version 1.0.0
 * @date 2026
 */
#include "ExampleContentPlugin.h"
#include <QDebug>

// ============================================================
// 构造函数与析构函数
// ============================================================

ExampleContentPlugin::ExampleContentPlugin()
    : m_initialized(false)
{
    qDebug() << "[ExampleContentPlugin] 创建示例内容插件";
}

ExampleContentPlugin::~ExampleContentPlugin()
{
    qDebug() << "[ExampleContentPlugin] 销毁示例内容插件";
}

// ============================================================
// IPlugin 接口实现
// ============================================================

QString ExampleContentPlugin::getName() const
{
    return "ExampleContentPlugin";
}

QString ExampleContentPlugin::getVersion() const
{
    return "1.0.0";
}

QString ExampleContentPlugin::getAuthor() const
{
    return "星溯旅人开发团队";
}

QString ExampleContentPlugin::getDescription() const
{
    return "示例内容插件，演示内容插件的开发方法。"
           "提供示例场景、角色、物品、任务、技能、敌人等内容。";
}

bool ExampleContentPlugin::initialize()
{
    if (m_initialized) {
        return true;
    }
    
    qDebug() << "[ExampleContentPlugin] 初始化示例内容插件...";
    
    // 初始化示例场景
    ContentItemInfo scene1;
    scene1.id = "example_forest";
    scene1.name = "示例森林";
    scene1.description = "一片神秘的森林，是示例内容插件提供的场景。";
    scene1.category = "自然";
    scene1.iconPath = ":/example/images/forest.png";
    scene1.properties["level"] = 1;
    scene1.properties["weather"] = "晴朗";
    m_scenes[scene1.id] = scene1;
    
    ContentItemInfo scene2;
    scene2.id = "example_cave";
    scene2.name = "示例洞穴";
    scene2.description = "一个幽深的洞穴，隐藏着未知的秘密。";
    scene2.category = "地下城";
    scene2.iconPath = ":/example/images/cave.png";
    scene2.properties["level"] = 5;
    scene2.properties["darkness"] = 80;
    m_scenes[scene2.id] = scene2;
    
    // 初始化示例角色
    ContentItemInfo char1;
    char1.id = "example_hero";
    char1.name = "示例英雄";
    char1.description = "一位勇敢的冒险者，来自示例内容插件。";
    char1.category = "主角";
    char1.iconPath = ":/example/images/hero.png";
    char1.properties["level"] = 1;
    char1.properties["hp"] = 100;
    char1.properties["mp"] = 50;
    m_characters[char1.id] = char1;
    
    ContentItemInfo char2;
    char2.id = "example_merchant";
    char2.name = "示例商人";
    char2.description = "一位神秘的商人，售卖各种奇特物品。";
    char2.category = "NPC";
    char2.iconPath = ":/example/images/merchant.png";
    char2.properties["friendly"] = true;
    char2.properties["shop_type"] = "通用";
    m_characters[char2.id] = char2;
    
    // 初始化示例物品
    ContentItemInfo item1;
    item1.id = "example_potion";
    item1.name = "示例药水";
    item1.description = "一瓶神奇的药水，可以恢复生命值。";
    item1.category = "消耗品";
    item1.iconPath = ":/example/images/potion.png";
    item1.properties["heal_amount"] = 50;
    item1.properties["stackable"] = true;
    item1.properties["price"] = 100;
    m_items[item1.id] = item1;
    
    ContentItemInfo item2;
    item2.id = "example_sword";
    item2.name = "示例之剑";
    item2.description = "一把锋利的宝剑，蕴含着神秘力量。";
    item2.category = "武器";
    item2.iconPath = ":/example/images/sword.png";
    item2.properties["attack"] = 25;
    item2.properties["durability"] = 100;
    item2.properties["price"] = 500;
    m_items[item2.id] = item2;
    
    // 初始化示例任务
    ContentItemInfo quest1;
    quest1.id = "example_quest_001";
    quest1.name = "示例任务：森林探索";
    quest1.description = "探索示例森林，寻找传说中的宝藏。";
    quest1.category = "主线";
    quest1.iconPath = ":/example/images/quest.png";
    quest1.properties["level"] = 1;
    quest1.properties["reward_exp"] = 100;
    quest1.properties["reward_gold"] = 500;
    m_quests[quest1.id] = quest1;
    
    ContentItemInfo quest2;
    quest2.id = "example_quest_002";
    quest2.name = "示例任务：洞穴探险";
    quest2.description = "深入示例洞穴，击败洞穴中的怪物。";
    quest2.category = "支线";
    quest2.iconPath = ":/example/images/quest.png";
    quest2.properties["level"] = 5;
    quest2.properties["reward_exp"] = 300;
    quest2.properties["reward_gold"] = 1000;
    m_quests[quest2.id] = quest2;
    
    // 初始化示例技能
    ContentItemInfo skill1;
    skill1.id = "example_fireball";
    skill1.name = "示例火球术";
    skill1.description = "发射一颗火球，对敌人造成火焰伤害。";
    skill1.category = "魔法";
    skill1.iconPath = ":/example/images/fireball.png";
    skill1.properties["damage"] = 30;
    skill1.properties["mp_cost"] = 15;
    skill1.properties["cooldown"] = 3;
    m_skills[skill1.id] = skill1;
    
    ContentItemInfo skill2;
    skill2.id = "example_heal";
    skill2.name = "示例治愈术";
    skill2.description = "恢复自身或友方的生命值。";
    skill2.category = "辅助";
    skill2.iconPath = ":/example/images/heal.png";
    skill2.properties["heal"] = 40;
    skill2.properties["mp_cost"] = 20;
    skill2.properties["cooldown"] = 5;
    m_skills[skill2.id] = skill2;
    
    // 初始化示例敌人
    ContentItemInfo enemy1;
    enemy1.id = "example_slime";
    enemy1.name = "示例史莱姆";
    enemy1.description = "一只可爱但危险的史莱姆。";
    enemy1.category = "普通怪物";
    enemy1.iconPath = ":/example/images/slime.png";
    enemy1.properties["level"] = 1;
    enemy1.properties["hp"] = 30;
    enemy1.properties["attack"] = 5;
    enemy1.properties["exp_reward"] = 20;
    enemy1.properties["gold_reward"] = 10;
    m_enemies[enemy1.id] = enemy1;
    
    ContentItemInfo enemy2;
    enemy2.id = "example_goblin";
    enemy2.name = "示例哥布林";
    enemy2.description = "狡猾的哥布林，喜欢偷袭旅行者。";
    enemy2.category = "普通怪物";
    enemy2.iconPath = ":/example/images/goblin.png";
    enemy2.properties["level"] = 3;
    enemy2.properties["hp"] = 50;
    enemy2.properties["attack"] = 12;
    enemy2.properties["exp_reward"] = 50;
    enemy2.properties["gold_reward"] = 30;
    m_enemies[enemy2.id] = enemy2;
    
    // 初始化音频
    m_bgmList << "example_bgm_forest" << "example_bgm_cave";
    m_sfxList << "example_sfx_attack" << "example_sfx_heal" << "example_sfx_levelup";
    
    m_initialized = true;
    
    qDebug() << "[ExampleContentPlugin] 示例内容插件初始化完成";
    qDebug() << "[ExampleContentPlugin] 内容统计:"
             << "场景:" << m_scenes.size()
             << "角色:" << m_characters.size()
             << "物品:" << m_items.size()
             << "任务:" << m_quests.size()
             << "技能:" << m_skills.size()
             << "敌人:" << m_enemies.size();
    
    return true;
}

void ExampleContentPlugin::shutdown()
{
    if (!m_initialized) {
        return;
    }
    
    qDebug() << "[ExampleContentPlugin] 关闭示例内容插件...";
    
    // 清理数据
    m_scenes.clear();
    m_characters.clear();
    m_items.clear();
    m_quests.clear();
    m_skills.clear();
    m_enemies.clear();
    m_bgmList.clear();
    m_sfxList.clear();
    
    m_initialized = false;
    
    qDebug() << "[ExampleContentPlugin] 示例内容插件已关闭";
}

// ============================================================
// 场景内容
// ============================================================

QStringList ExampleContentPlugin::getScenes() const
{
    return m_scenes.keys();
}

ContentItemInfo ExampleContentPlugin::getSceneInfo(const QString& sceneId) const
{
    return m_scenes.value(sceneId, ContentItemInfo());
}

// ============================================================
// 角色内容
// ============================================================

QStringList ExampleContentPlugin::getCharacters() const
{
    return m_characters.keys();
}

ContentItemInfo ExampleContentPlugin::getCharacterInfo(const QString& characterId) const
{
    return m_characters.value(characterId, ContentItemInfo());
}

// ============================================================
// 物品内容
// ============================================================

QStringList ExampleContentPlugin::getItems() const
{
    return m_items.keys();
}

ContentItemInfo ExampleContentPlugin::getItemInfo(const QString& itemId) const
{
    return m_items.value(itemId, ContentItemInfo());
}

// ============================================================
// 任务内容
// ============================================================

QStringList ExampleContentPlugin::getQuests() const
{
    return m_quests.keys();
}

ContentItemInfo ExampleContentPlugin::getQuestInfo(const QString& questId) const
{
    return m_quests.value(questId, ContentItemInfo());
}

// ============================================================
// 技能内容
// ============================================================

QStringList ExampleContentPlugin::getSkills() const
{
    return m_skills.keys();
}

ContentItemInfo ExampleContentPlugin::getSkillInfo(const QString& skillId) const
{
    return m_skills.value(skillId, ContentItemInfo());
}

// ============================================================
// 敌人内容
// ============================================================

QStringList ExampleContentPlugin::getEnemies() const
{
    return m_enemies.keys();
}

ContentItemInfo ExampleContentPlugin::getEnemyInfo(const QString& enemyId) const
{
    return m_enemies.value(enemyId, ContentItemInfo());
}

// ============================================================
// 音频内容
// ============================================================

QStringList ExampleContentPlugin::getBackgroundMusic() const
{
    return m_bgmList;
}

QStringList ExampleContentPlugin::getSoundEffects() const
{
    return m_sfxList;
}

// ============================================================
// 资源管理
// ============================================================

QString ExampleContentPlugin::getResourcePrefix() const
{
    return ":/example/";
}

// ============================================================
// 内容统计
// ============================================================

QMap<QString, int> ExampleContentPlugin::getContentCount() const
{
    QMap<QString, int> count;
    count["scenes"] = m_scenes.size();
    count["characters"] = m_characters.size();
    count["items"] = m_items.size();
    count["quests"] = m_quests.size();
    count["skills"] = m_skills.size();
    count["enemies"] = m_enemies.size();
    count["bgm"] = m_bgmList.size();
    count["sfx"] = m_sfxList.size();
    return count;
}
