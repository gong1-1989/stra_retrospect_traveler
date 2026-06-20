/**
 * @file ContentPlugin.cpp
 * @brief 内容插件示例实现 - 时空裂隙
 * 
 * 本文件实现了内容插件的所有功能。
 * 展示如何通过插件系统扩展游戏内容。
 * 
 * @author 星溯旅人开发团队
 * @version 1.2.0
 * @date 2026
 */
#include "ContentPlugin.h"
#include <QDebug>

// ============================================================
// 构造函数与析构函数
// ============================================================

ContentPlugin::ContentPlugin(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[ContentPlugin] 内容插件对象创建";
}

ContentPlugin::~ContentPlugin()
{
    qDebug() << "[ContentPlugin] 内容插件对象销毁";
}

// ============================================================
// IPlugin 接口实现
// ============================================================

QString ContentPlugin::getName() const
{
    return "TimeRiftContent";
}

QString ContentPlugin::getVersion() const
{
    return "1.2.0";
}

QString ContentPlugin::getAuthor() const
{
    return "星溯旅人开发团队";
}

QString ContentPlugin::getDescription() const
{
    return "时空裂隙内容扩展包：添加新场景、新角色、新物品";
}

bool ContentPlugin::initialize()
{
    qDebug() << "========================================";
    qDebug() << "  🌀 时空裂隙内容扩展包加载中...";
    qDebug() << "========================================";
    
    // 初始化所有内容
    initializeScenes();
    initializeCharacters();
    initializeItems();
    
    // 输出加载统计
    qDebug() << "[ContentPlugin] 场景加载完成: " << m_scenes.size() << "个";
    qDebug() << "[ContentPlugin] 角色加载完成: " << m_characters.size() << "个";
    qDebug() << "[ContentPlugin] 物品加载完成: " << m_items.size() << "个";
    
    qDebug() << "========================================";
    qDebug() << "  ✅ 时空裂隙内容扩展包加载完成！";
    qDebug() << "========================================";
    
    return true;
}

void ContentPlugin::shutdown()
{
    qDebug() << "[ContentPlugin] 卸载时空裂隙内容扩展包...";
    
    // 清理所有内容
    m_scenes.clear();
    m_characters.clear();
    m_items.clear();
    
    qDebug() << "[ContentPlugin] 内容扩展包已卸载";
}

// ============================================================
// 内容插件特有功能
// ============================================================

QList<SceneInfo> ContentPlugin::getNewScenes() const
{
    return m_scenes.values();
}

QList<CharacterInfo> ContentPlugin::getNewCharacters() const
{
    return m_characters.values();
}

QList<ItemInfo> ContentPlugin::getNewItems() const
{
    return m_items.values();
}

ItemInfo ContentPlugin::getItem(const QString& itemId) const
{
    return m_items.value(itemId, ItemInfo());
}

// ============================================================
// 私有初始化方法
// ============================================================

void ContentPlugin::initializeScenes()
{
    qDebug() << "[ContentPlugin] 初始化场景内容...";
    
    // 场景1：时空裂隙入口
    SceneInfo riftEntrance;
    riftEntrance.id = "time_rift_entrance";
    riftEntrance.name = "时空裂隙入口";
    riftEntrance.description = "时空裂隙的入口处，时间在这里变得扭曲，空气中弥漫着奇异的能量波动。";
    riftEntrance.theme = "时空";
    riftEntrance.difficulty = 3;
    m_scenes[riftEntrance.id] = riftEntrance;
    
    // 场景2：时间回廊
    SceneInfo timeCorridor;
    timeCorridor.id = "time_corridor";
    timeCorridor.name = "时间回廊";
    timeCorridor.description = "一条无尽的回廊，两侧是无数的时间碎片，每一片都记录着不同的历史。";
    timeCorridor.theme = "时间";
    timeCorridor.difficulty = 4;
    m_scenes[timeCorridor.id] = timeCorridor;
    
    // 场景3：悖论核心
    SceneInfo paradoxCore;
    paradoxCore.id = "paradox_core";
    paradoxCore.name = "悖论核心";
    paradoxCore.description = "时空裂隙的最深处，悖论能量在这里凝聚成核心，极度危险。";
    paradoxCore.theme = "悖论";
    paradoxCore.difficulty = 5;
    m_scenes[paradoxCore.id] = paradoxCore;
    
    qDebug() << "[ContentPlugin] 已添加" << m_scenes.size() << "个新场景";
}

void ContentPlugin::initializeCharacters()
{
    qDebug() << "[ContentPlugin] 初始化角色内容...";
    
    // 角色1：时空守护者
    CharacterInfo guardian;
    guardian.id = "time_guardian";
    guardian.name = "时空守护者·艾恩";
    guardian.description = "守护时空裂隙的神秘存在，拥有操控时间的力量。她已经在这里守护了千年。";
    guardian.role = "守护者";
    guardian.level = 50;
    m_characters[guardian.id] = guardian;
    
    // 角色2：时空旅者
    CharacterInfo traveler;
    traveler.id = "time_traveler";
    traveler.name = "迷途旅者·克洛诺斯";
    traveler.description = "一个迷失在时空中的旅者，他在寻找回家的路。";
    traveler.role = "NPC";
    traveler.level = 30;
    m_characters[traveler.id] = traveler;
    
    qDebug() << "[ContentPlugin] 已添加" << m_characters.size() << "个新角色";
}

void ContentPlugin::initializeItems()
{
    qDebug() << "[ContentPlugin] 初始化物品内容...";
    
    // 物品1：时空碎片
    ItemInfo timeShard;
    timeShard.id = "time_shard";
    timeShard.name = "时空碎片";
    timeShard.description = "从时空裂隙中掉落的碎片，蕴含着微弱的时间能量。可以用来修复时间装置。";
    timeShard.type = "材料";
    timeShard.rarity = 3;
    timeShard.value = 100.0f;
    m_items[timeShard.id] = timeShard;
    
    // 物品2：时空罗盘
    ItemInfo timeCompass;
    timeCompass.id = "time_compass";
    timeCompass.name = "时空罗盘";
    timeCompass.description = "一个古老的罗盘，能够指示时空裂隙的位置。是探索时空裂隙的必备工具。";
    timeCompass.type = "工具";
    timeCompass.rarity = 4;
    timeCompass.value = 500.0f;
    m_items[timeCompass.id] = timeCompass;
    
    // 物品3：时间沙漏
    ItemInfo timeHourglass;
    timeHourglass.id = "time_hourglass";
    timeHourglass.name = "时间沙漏";
    timeHourglass.description = "一个神奇的沙漏，里面的沙子永远不会流完。据说可以短暂地停止时间。";
    timeHourglass.type = "神器";
    timeHourglass.rarity = 5;
    timeHourglass.value = 2000.0f;
    m_items[timeHourglass.id] = timeHourglass;
    
    // 物品4：悖论结晶
    ItemInfo paradoxCrystal;
    paradoxCrystal.id = "paradox_crystal";
    paradoxCrystal.name = "悖论结晶";
    paradoxCrystal.description = "由悖论能量凝聚而成的结晶，极其不稳定，但蕴含着巨大的能量。";
    paradoxCrystal.type = "材料";
    paradoxCrystal.rarity = 5;
    paradoxCrystal.value = 1500.0f;
    m_items[paradoxCrystal.id] = paradoxCrystal;
    
    qDebug() << "[ContentPlugin] 已添加" << m_items.size() << "个新物品";
}
