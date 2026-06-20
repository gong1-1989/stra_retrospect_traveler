/**
 * @file EcosystemSystem.cpp
 * @brief 生态共生系统实现
 * 
 * 本文件实现了生态共生系统的所有功能。
 * 生态共生系统是游戏的三大核心系统之一。
 * 
 * 主要功能：
 * 1. 共生生物管理：解锁和管理多种共生生物
 * 2. 共生关系：建立和解除共生关系
 * 3. 共生进化：三条进化路线
 * 4. 共生能力：激活和使用共生生物的能力
 * 5. 生态平衡：生态指数管理
 * 6. 生态事件：触发和管理生态事件
 * 7. 生物多样性：记录发现的生物
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#include "EcosystemSystem.h"
#include <QDebug>
#include <QSet>
#include <cmath>

// ============================================================
// 构造函数和析构函数
// ============================================================

/**
 * @brief 构造函数
 * 
 * 创建生态系统实例。
 * 初始化所有成员变量为默认值。
 * 
 * 注意：
 * - 构造函数不进行实际的初始化工作
 * - 必须调用 initialize() 完成初始化后才能使用
 * - 父对象会自动管理内存
 * 
 * @param parent 父对象
 */
EcosystemSystem::EcosystemSystem(QObject* parent)
    : QObject(parent)
{
    // 成员变量在头文件中已初始化
    // 这里不需要额外操作
}

/**
 * @brief 析构函数
 * 
 * 销毁生态系统。
 * 自动调用 shutdown() 清理资源。
 * 
 * 注意：
 * - 析构函数会自动调用 shutdown()
 * - 不需要手动调用
 * - Qt 对象树会自动管理内存
 */
EcosystemSystem::~EcosystemSystem()
{
    // 调用 shutdown 清理资源
    shutdown();
}

// ============================================================
// 生命周期管理
// ============================================================

/**
 * @brief 初始化生态系统
 * 
 * 初始化生态系统的所有属性和状态。
 * 
 * 执行以下初始化操作：
 * 1. 检查是否已经初始化（防止重复初始化）
 * 2. 初始化默认共生生物
 * 3. 初始化生态指数
 * 4. 初始化生态事件
 * 5. 初始化生物多样性
 * 6. 设置初始化标志
 * 
 * 注意：
 * - 此函数是幂等的，可以安全地多次调用
 * - 如果已经初始化，会直接返回 true
 * - 初始化失败会返回 false
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
bool EcosystemSystem::initialize()
{
    // 检查是否已经初始化
    if (m_initialized) {
        qWarning() << "EcosystemSystem already initialized";
        return true;
    }

    qDebug() << "Initializing EcosystemSystem...";
    
    // 初始化默认共生生物
    initializeDefaultSymbionts();
    
    // 初始化生态指数
    m_ecoIndex = 50.0f;
    
    // 初始化生态事件
    m_ecoEventActive = false;
    
    // 初始化生物多样性
    m_biodiversityIndex = 0;
    
    // 设置初始化标志
    m_initialized = true;
    
    qDebug() << "EcosystemSystem initialized successfully";
    return true;
}

/**
 * @brief 关闭生态系统
 * 
 * 关闭生态系统，清理资源。
 * 
 * 执行以下操作：
 * 1. 检查是否已经初始化
 * 2. 清空共生生物映射
 * 3. 清空活跃共生体列表
 * 4. 清空已发现生物集合
 * 5. 重置初始化标志
 * 
 * 注意：
 * - 此函数是幂等的，可以安全地多次调用
 * - 不需要手动调用，析构函数会自动调用
 * - 关闭后需要重新调用 initialize() 才能使用
 */
void EcosystemSystem::shutdown()
{
    // 检查是否已经初始化
    if (!m_initialized) {
        return;
    }

    qDebug() << "Shutting down EcosystemSystem...";
    
    // 清空共生生物映射
    m_symbionts.clear();
    
    // 清空活跃共生体列表
    m_activeSymbionts.clear();
    
    // 清空已发现生物集合
    m_discoveredCreatures.clear();
    
    // 重置初始化标志
    m_initialized = false;
    
    qDebug() << "EcosystemSystem shut down successfully";
}

/**
 * @brief 更新生态系统
 * 
 * 每帧调用一次，更新生态系统状态。
 * 
 * 更新内容：
 * 1. 生态事件计时器
 * 2. 生态指数自然恢复
 * 
 * 注意：
 * - 此函数应该每帧调用一次
 * - deltaTime 是上一帧到这一帧的时间间隔
 * - 生态指数会自然恢复到50
 * 
 * @param deltaTime 时间增量（秒）
 */
void EcosystemSystem::update(float deltaTime)
{
    // 检查是否已经初始化
    if (!m_initialized) {
        return;
    }

    // 更新生态事件计时器
    if (m_ecoEventActive) {
        m_ecoEventTimer -= deltaTime;
        if (m_ecoEventTimer <= 0) {
            m_ecoEventActive = false;
            qDebug() << "Eco event ended";
        }
    }

    // 生态指数自然恢复
    if (m_ecoIndex < 50.0f) {
        // 低于50，缓慢恢复
        m_ecoIndex = std::min(50.0f, m_ecoIndex + 0.1f * deltaTime);
        emit ecoIndexChanged(m_ecoIndex);
    } else if (m_ecoIndex > 50.0f) {
        // 高于50，缓慢下降
        m_ecoIndex = std::max(50.0f, m_ecoIndex - 0.1f * deltaTime);
        emit ecoIndexChanged(m_ecoIndex);
    }
}

// ============================================================
// 共生生物管理
// ============================================================

/**
 * @brief 初始化默认共生生物
 * 
 * 初始化游戏中的默认共生生物。
 * 包括4种稀有共生生物：
 * 1. 时空蝶
 * 2. 星核兽
 * 3. 虚空鲸
 * 4. 幻影狐
 * 
 * 注意：
 * - 所有共生生物初始状态都是未解锁的
 * - 玩家需要通过探索和任务来解锁
 * - 每个共生生物都有独特的能力
 */
void EcosystemSystem::initializeDefaultSymbionts()
{
    // 添加一些默认共生生物
    SymbiontInfo info;
    
    // 时空蝶
    info.id = "time_butterfly";
    info.name = "时空蝶";
    info.type = PerceptionType;
    info.description = "能够感知时间异常的神秘生物";
    info.trustLevel = 0;
    info.symbiosisLevel = 1;
    info.evolutionPath = EvolutionPath::NaturalEvolution;
    info.unlocked = false;
    info.active = false;
    info.abilities = { "时间感知", "空间定位" };
    m_symbionts[info.id] = info;
    
    // 星核兽
    info.id = "star_core_beast";
    info.name = "星核兽";
    info.type = CombatType;
    info.description = "与星核能量共鸣的强大生物";
    info.trustLevel = 0;
    info.symbiosisLevel = 1;
    info.evolutionPath = EvolutionPath::PsionicEvolution;
    info.unlocked = false;
    info.active = false;
    info.abilities = { "共鸣攻击", "能量护盾" };
    m_symbionts[info.id] = info;
    
    // 虚空鲸
    info.id = "void_whale";
    info.name = "虚空鲸";
    info.type = SupportType;
    info.description = "能够在虚空中遨游的巨型生物";
    info.trustLevel = 0;
    info.symbiosisLevel = 1;
    info.evolutionPath = EvolutionPath::SymbioticEvolution;
    info.unlocked = false;
    info.active = false;
    info.abilities = { "虚空传送", "精神链接" };
    m_symbionts[info.id] = info;
    
    // 幻影狐
    info.id = "phantom_fox";
    info.name = "幻影狐";
    info.type = FlyingType;
    info.description = "能够制造幻象的灵巧生物";
    info.trustLevel = 0;
    info.symbiosisLevel = 1;
    info.evolutionPath = EvolutionPath::NaturalEvolution;
    info.unlocked = false;
    info.active = false;
    info.abilities = { "幻象制造", "隐身潜行" };
    m_symbionts[info.id] = info;
    
    qDebug() << "Initialized" << m_symbionts.size() << "default symbionts";
}

/**
 * @brief 解锁共生生物
 * 
 * 解锁指定的共生生物。
 * 解锁后共生生物的信任度为20。
 * 
 * 执行以下操作：
 * 1. 检查生物是否存在
 * 2. 检查生物是否已解锁
 * 3. 设置生物为已解锁
 * 4. 设置初始信任度（20点）
 * 5. 发出 symbiontUnlocked 信号
 * 
 * 注意：
 * - 如果生物不存在，会返回 false
 * - 如果生物已解锁，会返回 false
 * - 解锁后信任度为20
 * - 解锁后会发出 symbiontUnlocked 信号
 * 
 * @param id 生物ID
 * @return true 解锁成功
 * @return false 解锁失败（生物不存在或已解锁）
 */
bool EcosystemSystem::unlockSymbiont(const QString& id)
{
    // 检查生物是否存在
    if (!m_symbionts.contains(id)) {
        qWarning() << "Symbiont not found:" << id;
        return false;
    }

    // 检查生物是否已解锁
    if (m_symbionts[id].unlocked) {
        qWarning() << "Symbiont already unlocked:" << id;
        return false;
    }

    // 设置生物为已解锁
    m_symbionts[id].unlocked = true;
    
    // 设置初始信任度（20点）
    m_symbionts[id].trustLevel = 20;
    
    // 发出 symbiontUnlocked 信号
    emit symbiontUnlocked(id);
    
    qDebug() << "Unlocked symbiont:" << m_symbionts[id].name;
    
    return true;
}

/**
 * @brief 获取共生生物信息
 * 
 * 获取指定共生生物的详细信息。
 * 
 * 注意：
 * - 如果生物不存在，会返回空的 SymbiontInfo
 * - 返回的是副本，修改不会影响原始数据
 * 
 * @param id 生物ID
 * @return 生物信息
 */
EcosystemSystem::SymbiontInfo EcosystemSystem::getSymbiontInfo(const QString& id) const
{
    return m_symbionts.value(id, SymbiontInfo());
}

/**
 * @brief 检查共生生物是否已解锁
 * 
 * 检查指定共生生物是否已解锁。
 * 
 * 注意：
 * - 如果生物不存在，会返回 false
 * 
 * @param id 生物ID
 * @return true 已解锁
 * @return false 未解锁
 */
bool EcosystemSystem::isSymbiontUnlocked(const QString& id) const
{
    // 检查生物是否存在
    if (!m_symbionts.contains(id)) {
        return false;
    }
    return m_symbionts[id].unlocked;
}

/**
 * @brief 获取已解锁共生生物列表
 * 
 * 获取所有已解锁共生生物的ID列表。
 * 
 * 注意：
 * - 返回的是ID列表，不是完整信息
 * - 可以通过 getSymbiontInfo() 获取详细信息
 * 
 * @return 生物ID列表
 */
QStringList EcosystemSystem::getUnlockedSymbionts() const
{
    QStringList result;
    for (auto it = m_symbionts.begin(); it != m_symbionts.end(); ++it) {
        if (it.value().unlocked) {
            result.append(it.key());
        }
    }
    return result;
}

// ============================================================
// 共生关系
// ============================================================

/**
 * @brief 建立共生关系
 * 
 * 与指定共生生物建立共生关系。
 * 最多同时激活3个共生生物。
 * 
 * 执行以下操作：
 * 1. 检查生物是否已解锁
 * 2. 检查生物是否已活跃
 * 3. 检查是否达到最大共生槽位
 * 4. 添加到活跃共生体列表
 * 5. 设置生物为活跃
 * 6. 发出 symbiosisEstablished 信号
 * 
 * 注意：
 * - 如果生物未解锁，会返回 false
 * - 如果生物已活跃，会返回 false
 * - 如果达到最大共生槽位，会返回 false
 * - 建立成功后会发出 symbiosisEstablished 信号
 * 
 * @param id 生物ID
 * @return true 建立成功
 * @return false 建立失败
 */
bool EcosystemSystem::establishSymbiosis(const QString& id)
{
    // 检查生物是否已解锁
    if (!isSymbiontUnlocked(id)) {
        qWarning() << "Symbiont not unlocked:" << id;
        return false;
    }

    // 检查生物是否已活跃
    if (m_activeSymbionts.contains(id)) {
        qWarning() << "Symbiont already active:" << id;
        return false;
    }

    // 检查是否达到最大共生槽位
    if (m_activeSymbionts.size() >= m_maxSymbiontSlots) {
        qWarning() << "Max symbiont slots reached:" << m_maxSymbiontSlots;
        return false;
    }

    // 添加到活跃共生体列表
    m_activeSymbionts.append(id);
    
    // 设置生物为活跃
    m_symbionts[id].active = true;
    
    // 发出 symbiosisEstablished 信号
    emit symbiosisEstablished(id);
    
    qDebug() << "Established symbiosis with:" << m_symbionts[id].name;
    
    return true;
}

/**
 * @brief 解除共生关系
 * 
 * 与指定共生生物解除共生关系。
 * 
 * 执行以下操作：
 * 1. 检查生物是否活跃
 * 2. 从活跃共生体列表移除
 * 3. 设置生物为不活跃
 * 4. 发出 symbiosisBroken 信号
 * 
 * 注意：
 * - 如果生物不活跃，会返回 false
 * - 解除成功后会发出 symbiosisBroken 信号
 * 
 * @param id 生物ID
 * @return true 解除成功
 * @return false 解除失败（生物不活跃）
 */
bool EcosystemSystem::breakSymbiosis(const QString& id)
{
    // 检查生物是否活跃
    if (!m_activeSymbionts.contains(id)) {
        qWarning() << "Symbiont not active:" << id;
        return false;
    }

    // 从活跃共生体列表移除
    m_activeSymbionts.removeAll(id);
    
    // 设置生物为不活跃
    if (m_symbionts.contains(id)) {
        m_symbionts[id].active = false;
    }
    
    // 发出 symbiosisBroken 信号
    emit symbiosisBroken(id);
    
    qDebug() << "Broke symbiosis with:" << id;
    
    return true;
}

/**
 * @brief 增加信任度
 * 
 * 增加指定共生生物的信任度。
 * 信任度会自动限制在0-100范围内。
 * 
 * 执行以下操作：
 * 1. 检查生物是否存在
 * 2. 增加信任度（限制在0-100）
 * 3. 发出 trustLevelChanged 信号
 * 
 * 注意：
 * - 如果生物不存在，不会做任何操作
 * - 信任度会自动限制在0-100范围内
 * - 信任度变化后会发出 trustLevelChanged 信号
 * 
 * @param id 生物ID
 * @param amount 增加量
 */
void EcosystemSystem::addTrust(const QString& id, int amount)
{
    // 检查生物是否存在
    if (!m_symbionts.contains(id)) {
        return;
    }

    // 增加信任度（限制在0-100）
    m_symbionts[id].trustLevel = std::max(0, std::min(100, m_symbionts[id].trustLevel + amount));
    
    // 发出 trustLevelChanged 信号
    emit trustLevelChanged(id, m_symbionts[id].trustLevel);
}

/**
 * @brief 获取信任度
 * 
 * 获取指定共生生物的信任度。
 * 
 * 注意：
 * - 如果生物不存在，会返回 0
 * 
 * @param id 生物ID
 * @return 信任度（0-100）
 */
int EcosystemSystem::getTrustLevel(const QString& id) const
{
    // 检查生物是否存在
    if (!m_symbionts.contains(id)) {
        return 0;
    }
    return m_symbionts[id].trustLevel;
}

/**
 * @brief 获取活跃共生体数量
 * 
 * 获取当前活跃的共生体数量。
 * 
 * @return 数量
 */
int EcosystemSystem::getActiveSymbiontCount() const
{
    return m_activeSymbionts.size();
}

// ============================================================
// 共生进化
// ============================================================

/**
 * @brief 进化共生生物
 * 
 * 进化指定的共生生物。
 * 进化需要足够的信任度。
 * 
 * 执行以下操作：
 * 1. 检查生物是否已解锁
 * 2. 检查是否已达到最高等级
 * 3. 检查信任度是否足够
 * 4. 增加共生等级
 * 5. 设置进化路线
 * 6. 发出 symbiontEvolved 信号
 * 
 * 注意：
 * - 如果生物未解锁，会返回 false
 * - 如果已达到最高等级，会返回 false
 * - 如果信任度不足，会返回 false
 * - 进化需要的信任度 = 当前等级 * 10
 * - 进化后会发出 symbiontEvolved 信号
 * 
 * @param id 生物ID
 * @param path 进化路线
 * @return true 进化成功
 * @return false 进化失败
 */
bool EcosystemSystem::evolveSymbiont(const QString& id, EvolutionPath path)
{
    // 检查生物是否已解锁
    if (!isSymbiontUnlocked(id)) {
        qWarning() << "Symbiont not unlocked:" << id;
        return false;
    }

    SymbiontInfo& info = m_symbionts[id];
    
    // 检查是否已达到最高等级
    if (info.symbiosisLevel >= 10) {
        qWarning() << "Symbiont already max level:" << id;
        return false;
    }

    // 检查信任度是否足够
    if (info.trustLevel < info.symbiosisLevel * 10) {
        qWarning() << "Not enough trust for evolution";
        return false;
    }

    // 增加共生等级
    info.symbiosisLevel++;
    
    // 设置进化路线
    info.evolutionPath = path;
    
    // 发出 symbiontEvolved 信号
    emit symbiontEvolved(id, info.symbiosisLevel);
    
    qDebug() << "Evolved symbiont" << info.name << "to level" << info.symbiosisLevel;
    
    return true;
}

/**
 * @brief 获取共生等级
 * 
 * 获取指定共生生物的共生等级。
 * 
 * 注意：
 * - 如果生物不存在，会返回 0
 * 
 * @param id 生物ID
 * @return 等级（1-10）
 */
int EcosystemSystem::getSymbiosisLevel(const QString& id) const
{
    // 检查生物是否存在
    if (!m_symbionts.contains(id)) {
        return 0;
    }
    return m_symbionts[id].symbiosisLevel;
}

// ============================================================
// 共生能力
// ============================================================

/**
 * @brief 激活共生能力
 * 
 * 激活指定共生生物的能力。
 * 
 * 执行以下操作：
 * 1. 检查生物是否活跃
 * 2. 激活能力
 * 
 * 注意：
 * - 如果生物不活跃，会返回 false
 * - 目前只是打印日志，实际能力效果需要在游戏逻辑中实现
 * 
 * @param id 生物ID
 * @param abilityId 能力ID
 * @return true 激活成功
 * @return false 激活失败（生物不活跃）
 */
bool EcosystemSystem::activateAbility(const QString& id, const QString& abilityId)
{
    // 检查生物是否活跃
    if (!m_activeSymbionts.contains(id)) {
        qWarning() << "Symbiont not active:" << id;
        return false;
    }

    // 激活能力
    // 注意：这里只是打印日志，实际能力效果需要在游戏逻辑中实现
    qDebug() << "Activated ability" << abilityId << "from" << id;
    
    return true;
}

/**
 * @brief 获取共生生物能力列表
 * 
 * 获取指定共生生物的能力列表。
 * 
 * 注意：
 * - 如果生物不存在，会返回空列表
 * - 返回的是副本，修改不会影响原始数据
 * 
 * @param id 生物ID
 * @return 能力列表
 */
QList<QString> EcosystemSystem::getAbilities(const QString& id) const
{
    // 检查生物是否存在
    if (!m_symbionts.contains(id)) {
        return QList<QString>();
    }
    return m_symbionts[id].abilities;
}

// ============================================================
// 生态平衡
// ============================================================

/**
 * @brief 设置生态指数
 * 
 * 设置生态指数。
 * 生态指数会自动限制在0-100范围内。
 * 
 * 执行以下操作：
 * 1. 设置生态指数（限制在0-100）
 * 2. 发出 ecoIndexChanged 信号
 * 
 * 注意：
 * - 生态指数会自动限制在0-100范围内
 * - 生态指数变化后会发出 ecoIndexChanged 信号
 * 
 * @param index 生态指数
 */
void EcosystemSystem::setEcoIndex(float index)
{
    // 设置生态指数（限制在0-100）
    m_ecoIndex = std::max(0.0f, std::min(m_maxEcoIndex, index));
    
    // 发出 ecoIndexChanged 信号
    emit ecoIndexChanged(m_ecoIndex);
}

/**
 * @brief 增加生态指数
 * 
 * 增加生态指数。
 * 
 * 注意：
 * - 生态指数会自动限制在0-100范围内
 * 
 * @param amount 增加量
 */
void EcosystemSystem::addEcoIndex(float amount)
{
    setEcoIndex(m_ecoIndex + amount);
}

/**
 * @brief 减少生态指数
 * 
 * 减少生态指数。
 * 
 * 注意：
 * - 生态指数会自动限制在0-100范围内
 * 
 * @param amount 减少量
 */
void EcosystemSystem::reduceEcoIndex(float amount)
{
    setEcoIndex(m_ecoIndex - amount);
}

/**
 * @brief 更新生态指数
 * 
 * 更新生态指数的逻辑。
 * 目前为空，预留扩展接口。
 * 
 * 注意：
 * - 目前为空，预留扩展接口
 * - 实际的生态指数更新在 update() 函数中实现
 */
void EcosystemSystem::updateEcoIndex()
{
    // 生态指数更新逻辑
    // 目前为空，预留扩展接口
}

// ============================================================
// 生态事件
// ============================================================

/**
 * @brief 触发生态事件
 * 
 * 触发指定的生态事件。
 * 事件持续60秒。
 * 
 * 执行以下操作：
 * 1. 检查是否已有活跃事件
 * 2. 设置当前事件类型
 * 3. 设置事件为活跃
 * 4. 设置事件计时器（60秒）
 * 5. 发出 ecoEventTriggered 信号
 * 
 * 注意：
 * - 如果已有活跃事件，会返回 false
 * - 事件持续60秒
 * - 触发后会发出 ecoEventTriggered 信号
 * 
 * @param eventType 事件类型
 * @return true 触发成功
 * @return false 触发失败（已有活跃事件）
 */
bool EcosystemSystem::triggerEcoEvent(EcoEventType eventType)
{
    // 检查是否已有活跃事件
    if (m_ecoEventActive) {
        qWarning() << "Eco event already active";
        return false;
    }

    // 设置当前事件类型
    m_currentEcoEvent = eventType;
    
    // 设置事件为活跃
    m_ecoEventActive = true;
    
    // 设置事件计时器（60秒）
    m_ecoEventTimer = 60.0f;
    
    // 发出 ecoEventTriggered 信号
    emit ecoEventTriggered(eventType);
    
    qDebug() << "Triggered eco event:" << static_cast<int>(eventType);
    
    return true;
}

// ============================================================
// 生物多样性
// ============================================================

/**
 * @brief 记录发现的生物
 * 
 * 记录发现的新生物。
 * 如果是新发现的生物，会发出 creatureDiscovered 信号。
 * 
 * 执行以下操作：
 * 1. 检查生物是否已发现
 * 2. 添加到已发现集合
 * 3. 更新生物多样性指数
 * 4. 发出 creatureDiscovered 信号
 * 
 * 注意：
 * - 如果生物已发现，不会做任何操作
 * - 发现新生物后会发出 creatureDiscovered 信号
 * - 生物多样性指数等于已发现生物的数量
 * 
 * @param creatureId 生物ID
 */
void EcosystemSystem::recordCreatureDiscovery(const QString& creatureId)
{
    // 检查生物是否已发现
    if (m_discoveredCreatures.contains(creatureId)) {
        return;
    }

    // 添加到已发现集合
    m_discoveredCreatures.insert(creatureId);
    
    // 更新生物多样性指数
    m_biodiversityIndex = m_discoveredCreatures.size();
    
    // 发出 creatureDiscovered 信号
    emit creatureDiscovered(creatureId);
    
    qDebug() << "Discovered new creature:" << creatureId;
}
