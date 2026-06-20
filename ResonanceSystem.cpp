/**
 * @file ResonanceSystem.cpp
 * @brief 星核共鸣系统实现
 * 
 * 本文件实现了星核共鸣系统的所有功能，包括：
 * - 星核系统的初始化和关闭
 * - 星核解锁和管理
 * - 共鸣系统
 * - 环境操控
 * - 反噬系统
 * - 共鸣组合技
 * 
 * 星核共鸣系统是游戏的三大核心系统之一，
 * 管理星核收集、共鸣等级、环境操控等核心机制。
 * 
 * 主要功能：
 * 1. 星核管理：解锁和管理6种星核
 * 2. 共鸣系统：增加共鸣度，提升共鸣等级
 * 3. 环境操控：地形、天气、生命操控
 * 4. 反噬系统：共鸣操作会产生反噬
 * 5. 共鸣组合技：多种星核组合释放强力技能
 * 
 * 星核类型：
 * - Earth：大地星核
 * - Ocean：海洋星核
 * - Flame：烈焰星核
 * - Storm：风暴星核
 * - Life：生命星核
 * - Void：虚空星核
 * 
 * 共鸣等级：
 * - ResonanceNone：无共鸣
 * - ResonanceShallow：浅层共鸣
 * - ResonanceDeep：深层共鸣
 * - ResonanceFull：完全共鸣
 * 
 * 反噬等级：
 * - BacklashNone：无反噬
 * - BacklashMild：轻度反噬
 * - BacklashModerate：中度反噬
 * - BacklashSevere：重度反噬
 * - BacklashPermanent：永久反噬
 * 
 * 星核人格：
 * - 每个星核都有独特的人格
 * - 大地星核：沉稳
 * - 海洋星核：温柔
 * - 烈焰星核：暴躁
 * - 风暴星核：自由
 * - 生命星核：慈爱
 * - 虚空星核：神秘
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#include "ResonanceSystem.h"

#include <QDebug>
#include <cmath>

// ============================================================
// 构造函数与析构函数
// ============================================================

/**
 * @brief 构造函数
 * 
 * 创建星核系统实例，但不进行任何初始化操作。
 * 所有的初始化工作都在 initialize() 中完成。
 * 
 * @param parent 父对象，用于 Qt 对象树内存管理
 */
ResonanceSystem::ResonanceSystem(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[ResonanceSystem] 创建星核系统对象";
}

/**
 * @brief 析构函数
 * 
 * 销毁星核系统，自动调用 shutdown() 清理资源。
 */
ResonanceSystem::~ResonanceSystem()
{
    qDebug() << "[ResonanceSystem] 销毁星核系统对象";
    shutdown();
}

// ============================================================
// 生命周期管理
// ============================================================

/**
 * @brief 初始化星核系统
 * 
 * 初始化星核系统的所有属性和状态。
 * 
 * 执行以下初始化操作：
 * 1. 检查是否已经初始化（防止重复初始化）
 * 2. 初始化所有6种星核
 * 3. 默认解锁大地星核
 * 4. 初始化当前激活星核
 * 5. 初始化反噬系统
 * 6. 设置初始化标志
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
bool ResonanceSystem::initialize()
{
    // 防止重复初始化
    if (m_initialized) {
        qWarning() << "[ResonanceSystem] 星核系统已经初始化，跳过重复初始化";
        return true;
    }

    qDebug() << "[ResonanceSystem] 初始化星核系统...";
    
    // 1. 初始化所有星核
    QList<StarCoreType> types = { StarCoreType::Earth,
                                 StarCoreType::Ocean,
                                 StarCoreType::Flame,
                                 StarCoreType::Storm,
                                 StarCoreType::Life,
                                 StarCoreType::Void };
    QStringList names = { "大地星核", "海洋星核", "烈焰星核", "风暴星核", "生命星核", "虚空星核" };
    QStringList descs = {
        "掌控大地与岩石的力量",
        "掌控海洋与水流的力量",
        "掌控烈焰与温度的力量",
        "掌控风暴与雷电的力量",
        "掌控生命与自然的力量",
        "掌控虚空与混沌的力量"
    };
    QStringList personalities = { "沉稳", "温柔", "暴躁", "自由", "慈爱", "神秘" };
    
    for (int i = 0; i < types.size(); ++i) {
        StarCoreInfo info;
        info.type = types[i];
        info.name = names[i];
        info.description = descs[i];
        info.resonanceLevel = ResonanceLevel::ResonanceNone;
        info.resonanceValue = 0.0f;
        info.stability = 100.0f;
        info.unlocked = false;
        info.personality = personalities[i];
        m_starCores[types[i]] = info;
    }
    
    // 2. 默认解锁大地星核
    unlockStarCore(StarCoreType::Earth);
    
    // 3. 初始化当前激活星核
    m_activeStarCore = StarCoreType::Earth;
    m_resonanceActive = false;
    
    // 4. 初始化反噬系统
    m_backlashValue = 0.0f;
    m_backlashLevel = BacklashLevel::BacklashNone;
    
    // 5. 设置初始化标志
    m_initialized = true;
    
    qDebug() << "[ResonanceSystem] 星核系统初始化完成";
    qDebug() << "[ResonanceSystem] 默认解锁星核:" << getStarCoreName(StarCoreType::Earth);
    qDebug() << "[ResonanceSystem] 已解锁星核数量:" << getUnlockedCount();
    
    return true;
}

/**
 * @brief 关闭星核系统
 * 
 * 关闭星核系统，清理资源。
 * 
 * 执行以下操作：
 * 1. 检查是否已经初始化
 * 2. 清空星核映射
 * 3. 重置初始化标志
 * 
 * 注意：
 * - 此函数是幂等的，可以安全地多次调用
 * - 不需要手动调用，析构函数会自动调用
 */
void ResonanceSystem::shutdown()
{
    // 如果没有初始化，直接返回
    if (!m_initialized) {
        return;
    }

    qDebug() << "[ResonanceSystem] 关闭星核系统...";
    
    // 清空星核映射
    m_starCores.clear();
    
    // 重置初始化标志
    m_initialized = false;
    
    qDebug() << "[ResonanceSystem] 星核系统已关闭";
}

/**
 * @brief 更新星核系统
 * 
 * 每帧调用一次，更新星核系统状态。
 * 
 * 更新内容：
 * 1. 反噬自然恢复（每秒0.5点）
 * 
 * @param deltaTime 时间增量（秒）
 */
void ResonanceSystem::update(float deltaTime)
{
    // 如果没有初始化，不更新
    if (!m_initialized) {
        return;
    }

    // 1. 反噬自然恢复
    if (m_backlashValue > 0) {
        // 每秒恢复 0.5 点
        m_backlashValue = std::max(0.0f, m_backlashValue - 0.5f * deltaTime);
        updateBacklashLevel();
    }
}

// ============================================================
// 星核管理
// ============================================================

/**
 * @brief 解锁星核
 * 
 * 解锁指定的星核。
 * 解锁后星核的共鸣度为10，共鸣等级为浅层共鸣。
 * 
 * 执行以下操作：
 * 1. 检查星核类型是否有效
 * 2. 检查星核是否已解锁
 * 3. 设置星核为已解锁
 * 4. 设置初始共鸣度（10点）
 * 5. 设置初始共鸣等级（浅层共鸣）
 * 6. 发出 starCoreUnlocked 信号
 * 7. 发出 resonanceLevelChanged 信号
 * 
 * @param type 星核类型
 * @return true 解锁成功
 * @return false 解锁失败（类型无效或已解锁）
 */
bool ResonanceSystem::unlockStarCore(StarCoreType type)
{
    // 检查星核类型是否有效
    if (!m_starCores.contains(type)) {
        qWarning() << "[ResonanceSystem] 无效的星核类型:" << static_cast<int>(type);
        return false;
    }
    
    // 检查星核是否已解锁
    if (m_starCores[type].unlocked) {
        qWarning() << "[ResonanceSystem] 星核已解锁:" << getStarCoreName(type);
        return false;
    }
    
    // 设置星核为已解锁
    m_starCores[type].unlocked = true;
    // 设置初始共鸣度
    m_starCores[type].resonanceValue = 10.0f;
    // 设置初始共鸣等级
    m_starCores[type].resonanceLevel = ResonanceLevel::ResonanceShallow;
    
    // 发出信号
    emit starCoreUnlocked(type);
    emit resonanceLevelChanged(type, ResonanceLevel::ResonanceShallow);
    
    qDebug() << "[ResonanceSystem] 解锁星核:" << getStarCoreName(type);
    
    return true;
}

/**
 * @brief 获取星核信息
 * 
 * 获取指定星核的详细信息。
 * 
 * @param type 星核类型
 * @return 星核信息
 */
ResonanceSystem::StarCoreInfo ResonanceSystem::getStarCoreInfo(StarCoreType type) const
{
    return m_starCores.value(type, StarCoreInfo());
}

/**
 * @brief 检查星核是否已解锁
 * @param type 星核类型
 * @return true 已解锁
 * @return false 未解锁
 */
bool ResonanceSystem::isStarCoreUnlocked(StarCoreType type) const
{
    // 检查星核类型是否有效
    if (!m_starCores.contains(type)) {
        return false;
    }
    return m_starCores[type].unlocked;
}

/**
 * @brief 获取已解锁星核数量
 * @return 已解锁星核数量
 */
int ResonanceSystem::getUnlockedCount() const
{
    int count = 0;
    for (const StarCoreInfo& info : m_starCores) {
        if (info.unlocked) {
            count++;
        }
    }
    return count;
}

/**
 * @brief 获取星核名称
 * 
 * 获取指定星核的中文名称。
 * 
 * @param type 星核类型
 * @return 星核名称
 */
QString ResonanceSystem::getStarCoreName(StarCoreType type) const
{
    switch (type) {
        case StarCoreType::Earth:  return "大地星核";
        case StarCoreType::Ocean:  return "海洋星核";
        case StarCoreType::Flame:  return "烈焰星核";
        case StarCoreType::Storm:  return "风暴星核";
        case StarCoreType::Life:   return "生命星核";
        case StarCoreType::Void:   return "虚空星核";
        default:     return "未知星核";
    }
}

// ============================================================
// 共鸣系统
// ============================================================

/**
 * @brief 增加共鸣度
 * 
 * 增加指定星核的共鸣度。
 * 共鸣度会自动限制在最大值以内。
 * 增加共鸣度后会自动检查是否升级。
 * 
 * 执行以下操作：
 * 1. 检查星核是否已解锁
 * 2. 增加共鸣度（最大为100）
 * 3. 检查共鸣等级升级
 * 
 * @param type 星核类型
 * @param amount 增加量
 * @return true 增加成功
 * @return false 增加失败（星核未解锁）
 */
bool ResonanceSystem::addResonance(StarCoreType type, float amount)
{
    // 检查星核是否已解锁
    if (!isStarCoreUnlocked(type)) {
        qWarning() << "[ResonanceSystem] 星核未解锁:" << getStarCoreName(type);
        return false;
    }
    
    // 增加共鸣度，最大为 100
    m_starCores[type].resonanceValue = std::min(100.0f, m_starCores[type].resonanceValue + amount);
    // 检查共鸣等级升级
    checkResonanceLevelUp(type);
    
    return true;
}

/**
 * @brief 获取共鸣等级
 * @param type 星核类型
 * @return 共鸣等级
 */
ResonanceSystem::ResonanceLevel ResonanceSystem::getResonanceLevel(StarCoreType type) const
{
    // 检查星核类型是否有效
    if (!m_starCores.contains(type)) {
        return ResonanceLevel::ResonanceNone;
    }
    return m_starCores[type].resonanceLevel;
}

/**
 * @brief 获取共鸣度值
 * @param type 星核类型
 * @return 共鸣度（0-100）
 */
float ResonanceSystem::getResonanceValue(StarCoreType type) const
{
    // 检查星核类型是否有效
    if (!m_starCores.contains(type)) {
        return 0.0f;
    }
    return m_starCores[type].resonanceValue;
}

/**
 * @brief 激活星核共鸣
 * 
 * 激活指定星核的共鸣。
 * 激活共鸣会增加反噬。
 * 
 * 执行以下操作：
 * 1. 检查星核是否已解锁
 * 2. 设置当前激活星核
 * 3. 设置共鸣激活标志
 * 4. 增加反噬（5点）
 * 5. 发出 resonanceActivated 信号
 * 
 * @param type 星核类型
 * @return true 激活成功
 * @return false 激活失败（星核未解锁）
 */
bool ResonanceSystem::activateResonance(StarCoreType type)
{
    // 检查星核是否已解锁
    if (!isStarCoreUnlocked(type)) {
        qWarning() << "[ResonanceSystem] 星核未解锁:" << getStarCoreName(type);
        return false;
    }
    
    // 设置当前激活星核
    m_activeStarCore = type;
    // 设置共鸣激活标志
    m_resonanceActive = true;
    
    // 激活共鸣增加反噬
    addBacklash(5.0f);
    
    // 发出信号
    emit resonanceActivated(type);
    qDebug() << "[ResonanceSystem] 激活共鸣:" << getStarCoreName(type);
    
    return true;
}

/**
 * @brief 取消星核共鸣
 * 
 * 取消当前星核的共鸣。
 * 
 * @param type 星核类型（未使用，预留）
 * @return true 取消成功
 * @return false 取消失败
 */
bool ResonanceSystem::deactivateResonance(StarCoreType type)
{
    Q_UNUSED(type);
    
    // 取消共鸣
    m_resonanceActive = false;
    
    return true;
}

/**
 * @brief 检查共鸣等级升级
 * 
 * 根据当前共鸣度检查是否升级。
 * 如果等级发生变化，会发出 resonanceLevelChanged 信号。
 * 
 * 等级划分：
 * - 0-10：无共鸣
 * - 10-50：浅层共鸣
 * - 50-80：深层共鸣
 * - 80-100：完全共鸣
 * 
 * @param type 星核类型
 */
void ResonanceSystem::checkResonanceLevelUp(StarCoreType type)
{
    StarCoreInfo& info = m_starCores[type];
    ResonanceLevel oldLevel = info.resonanceLevel;
    
    // 根据共鸣度计算新等级
    if (info.resonanceValue >= 80.0f) {
        info.resonanceLevel = ResonanceLevel::ResonanceFull;
    } else if (info.resonanceValue >= 50.0f) {
        info.resonanceLevel = ResonanceLevel::ResonanceDeep;
    } else if (info.resonanceValue >= 10.0f) {
        info.resonanceLevel = ResonanceLevel::ResonanceShallow;
    } else {
        info.resonanceLevel = ResonanceLevel::ResonanceNone;
    }
    
    // 如果等级发生变化，发出信号
    if (info.resonanceLevel != oldLevel) {
        emit resonanceLevelChanged(type, info.resonanceLevel);
        qDebug() << "[ResonanceSystem]" << getStarCoreName(type) 
                 << "共鸣等级提升到" << static_cast<int>(info.resonanceLevel);
    }
}

// ============================================================
// 环境操控
// ============================================================

/**
 * @brief 地形操控
 * 
 * 使用当前激活星核的力量操控地形。
 * 地形操控会增加反噬。
 * 
 * 执行以下操作：
 * 1. 检查是否有激活的共鸣
 * 2. 增加反噬（2点）
 * 3. 发出 environmentManipulated 信号
 * 
 * @param position 操控位置
 * @param radius 操控半径
 * @return true 操控成功
 * @return false 操控失败（没有激活的共鸣）
 */
bool ResonanceSystem::terrainManipulation(const QPointF& position, float radius)
{
    Q_UNUSED(position);
    Q_UNUSED(radius);
    
    // 检查是否有激活的共鸣
    if (!m_resonanceActive) {
        qWarning() << "[ResonanceSystem] 没有激活的共鸣";
        return false;
    }
    
    qDebug() << "[ResonanceSystem] 地形操控，使用:" << getStarCoreName(m_activeStarCore);
    // 增加反噬
    addBacklash(2.0f);
    // 发出信号
    emit environmentManipulated(0);
    
    return true;
}

/**
 * @brief 天气操控
 * 
 * 使用当前激活星核的力量操控天气。
 * 天气操控会增加反噬。
 * 
 * 执行以下操作：
 * 1. 检查是否有激活的共鸣
 * 2. 增加反噬（3点）
 * 3. 发出 environmentManipulated 信号
 * 
 * @param weatherType 天气类型
 * @return true 操控成功
 * @return false 操控失败（没有激活的共鸣）
 */
bool ResonanceSystem::weatherManipulation(int weatherType)
{
    Q_UNUSED(weatherType);
    
    // 检查是否有激活的共鸣
    if (!m_resonanceActive) {
        qWarning() << "[ResonanceSystem] 没有激活的共鸣";
        return false;
    }
    
    qDebug() << "[ResonanceSystem] 天气操控，使用:" << getStarCoreName(m_activeStarCore);
    // 增加反噬
    addBacklash(3.0f);
    // 发出信号
    emit environmentManipulated(1);
    
    return true;
}

/**
 * @brief 生命操控
 * 
 * 使用当前激活星核的力量操控生命。
 * 生命操控会增加反噬。
 * 
 * 执行以下操作：
 * 1. 检查是否有激活的共鸣
 * 2. 增加反噬（2.5点）
 * 3. 发出 environmentManipulated 信号
 * 
 * @param position 操控位置
 * @param radius 操控半径
 * @return true 操控成功
 * @return false 操控失败（没有激活的共鸣）
 */
bool ResonanceSystem::lifeManipulation(const QPointF& position, float radius)
{
    Q_UNUSED(position);
    Q_UNUSED(radius);
    
    // 检查是否有激活的共鸣
    if (!m_resonanceActive) {
        qWarning() << "[ResonanceSystem] 没有激活的共鸣";
        return false;
    }
    
    qDebug() << "[ResonanceSystem] 生命操控，使用:" << getStarCoreName(m_activeStarCore);
    // 增加反噬
    addBacklash(2.5f);
    // 发出信号
    emit environmentManipulated(2);
    
    return true;
}

// ============================================================
// 反噬系统
// ============================================================

/**
 * @brief 增加反噬
 * 
 * 增加反噬值。
 * 反噬值会自动限制在最大值以内。
 * 增加反噬后会自动更新反噬等级。
 * 
 * 执行以下操作：
 * 1. 增加反噬值（最大为最大值）
 * 2. 更新反噬等级
 * 
 * @param amount 增加量
 */
void ResonanceSystem::addBacklash(float amount)
{
    // 增加反噬值，最大为最大值
    m_backlashValue = std::min(m_maxBacklash, m_backlashValue + amount);
    // 更新反噬等级
    updateBacklashLevel();
}

/**
 * @brief 恢复反噬
 * 
 * 减少反噬值（恢复反噬）。
 * 反噬值会自动限制在最小值（0）以上。
 * 恢复反噬后会自动更新反噬等级。
 * 
 * 执行以下操作：
 * 1. 减少反噬值（最小为 0）
 * 2. 更新反噬等级
 * 
 * @param amount 恢复量
 */
void ResonanceSystem::recoverBacklash(float amount)
{
    // 减少反噬值，最小为 0
    m_backlashValue = std::max(0.0f, m_backlashValue - amount);
    // 更新反噬等级
    updateBacklashLevel();
}

/**
 * @brief 更新反噬等级
 * 
 * 根据当前反噬值更新反噬等级。
 * 如果等级发生变化，会发出 backlashLevelChanged 信号。
 * 
 * 等级划分：
 * - 0-20：无反噬
 * - 20-40：轻度反噬
 * - 40-60：中度反噬
 * - 60-80：重度反噬
 * - 80-100：永久反噬
 */
void ResonanceSystem::updateBacklashLevel()
{
    BacklashLevel oldLevel = m_backlashLevel;
    
    // 根据反噬值计算新等级
    if (m_backlashValue >= 80.0f) {
        m_backlashLevel = BacklashLevel::BacklashPermanent;
    } else if (m_backlashValue >= 60.0f) {
        m_backlashLevel = BacklashLevel::BacklashSevere;
    } else if (m_backlashValue >= 40.0f) {
        m_backlashLevel = BacklashLevel::BacklashModerate;
    } else if (m_backlashValue >= 20.0f) {
        m_backlashLevel = BacklashLevel::BacklashMild;
    } else {
        m_backlashLevel = BacklashLevel::BacklashNone;
    }
    
    // 如果等级发生变化，发出信号
    if (m_backlashLevel != oldLevel) {
        emit backlashLevelChanged(m_backlashLevel);
        qDebug() << "[ResonanceSystem] 反噬等级变化:" << getBacklashLevelName(oldLevel) 
                 << "->" << getBacklashLevelName(m_backlashLevel);
    }
}

/**
 * @brief 获取反噬等级名称
 * 
 * 获取指定反噬等级的中文名称。
 * 
 * @param level 反噬等级
 * @return 反噬等级名称
 */
QString ResonanceSystem::getBacklashLevelName(BacklashLevel level) const
{
    switch (level) {
        case BacklashLevel::BacklashNone:       return "无反噬";
        case BacklashLevel::BacklashMild:       return "轻度反噬";
        case BacklashLevel::BacklashModerate:   return "中度反噬";
        case BacklashLevel::BacklashSevere:     return "重度反噬";
        case BacklashLevel::BacklashPermanent:  return "永久反噬";
        default:                 return "未知";
    }
}

// ============================================================
// 共鸣组合技
// ============================================================

/**
 * @brief 释放组合技
 * 
 * 释放共鸣组合技。
 * 需要至少2个星核才能释放组合技。
 * 组合技会产生更多反噬。
 * 
 * 执行以下操作：
 * 1. 检查是否有至少2个星核
 * 2. 增加反噬（10点）
 * 
 * @param comboType 组合技类型
 * @return true 释放成功
 * @return false 释放失败（星核不足）
 */
bool ResonanceSystem::releaseCombo(int comboType)
{
    Q_UNUSED(comboType);
    
    // 检查是否有至少2个星核
    if (getUnlockedCount() < 2) {
        qWarning() << "[ResonanceSystem] 组合技需要至少2个星核";
        return false;
    }
    
    qDebug() << "[ResonanceSystem] 释放组合技";
    // 增加反噬
    addBacklash(10.0f);
    
    return true;
}

/**
 * @brief 检查组合技是否可用
 * 
 * 检查是否可以释放组合技。
 * 需要至少2个星核才能释放组合技。
 * 
 * @param comboType 组合技类型
 * @return true 可用
 * @return false 不可用
 */
bool ResonanceSystem::isComboAvailable(int comboType) const
{
    Q_UNUSED(comboType);
    // 需要至少2个星核
    return getUnlockedCount() >= 2;
}
