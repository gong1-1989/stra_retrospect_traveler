/**
 * @file EcosystemSystem.h
 * @brief 生态共生系统
 * 
 * 本文件定义了生态共生系统，这是游戏的三大核心系统之一。
 * 生态共生系统管理共生生物收集、共生关系、生态平衡等核心机制。
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
 * 共生生物类型：
 * - FlyingType：飞行类
 * - AquaticType：水生类
 * - CombatType：战斗类
 * - PerceptionType：感知类
 * - SupportType：辅助类
 * 
 * 共生进化路线：
 * - NaturalEvolution：自然进化
 * - PsionicEvolution：灵能进化
 * - SymbioticEvolution：共生进化
 * 
 * 生态事件类型：
 * - Migration：生物迁徙
 * - Disaster：生态灾难
 * - Invasion：物种入侵
 * - PsionicSurge：灵能潮涌
 * 
 * 稀有共生生物：
 * - 时空蝶：能够感知时间异常的神秘生物
 * - 星核兽：与星核能量共鸣的强大生物
 * - 虚空鲸：能够在虚空中遨游的巨型生物
 * - 幻影狐：能够制造幻象的灵巧生物
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#ifndef ECOSYSTEM_SYSTEM_H
#define ECOSYSTEM_SYSTEM_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QString>
#include <QPointF>
#include <QSet>

/**
 * @brief 生态共生系统
 * 
 * 管理共生生物收集、共生关系、生态平衡等核心机制。
 * 生态共生系统是游戏的三大核心系统之一。
 * 
 * 架构说明：
 * - 生态系统继承 QObject，支持信号槽机制
 * - 共生生物解锁时会发出 symbiontUnlocked 信号
 * - 共生关系建立时会发出 symbiosisEstablished 信号
 * - 共生关系解除时会发出 symbiosisBroken 信号
 * - 信任度变化时会发出 trustLevelChanged 信号
 * - 共生进化时会发出 symbiontEvolved 信号
 * - 生态指数变化时会发出 ecoIndexChanged 信号
 * - 生态事件触发时会发出 ecoEventTriggered 信号
 * - 生物发现时会发出 creatureDiscovered 信号
 * 
 * 共生生物系统：
 * - 支持5种类型：飞行、水生、战斗、感知、辅助
 * - 每个共生生物都有独特的能力
 * - 共生生物需要通过探索和任务解锁
 * - 最多同时激活3个共生生物
 * 
 * 共生关系：
 * - 信任度范围：0-100
 * - 共生等级范围：1-10
 * - 信任度越高，共生能力越强
 * - 信任度可以通过互动和任务增加
 * 
 * 共生进化：
 * - 三条进化路线：自然、灵能、共生
 * - 进化需要足够的信任度
 * - 进化后能力会增强
 * 
 * 生态平衡：
 * - 生态指数范围：0-100
 * - 生态指数会自然恢复到50
 * - 玩家行为会影响生态指数
 * - 生态指数影响游戏世界
 * 
 * 生态事件：
 * - 4种生态事件类型
 * - 事件持续60秒
 * - 事件会影响游戏世界
 * 
 * 生物多样性：
 * - 记录发现的生物
 * - 生物多样性指数
 * - 发现新生物会发出信号
 * 
 * 使用示例：
 * @code
 * EcosystemSystem ecosystemSystem;
 * ecosystemSystem.initialize();
 * 
 * // 解锁共生生物
 * ecosystemSystem.unlockSymbiont("time_butterfly");
 * 
 * // 建立共生关系
 * ecosystemSystem.establishSymbiosis("time_butterfly");
 * 
 * // 增加信任度
 * ecosystemSystem.addTrust("time_butterfly", 30);
 * 
 * // 进化共生生物
 * ecosystemSystem.evolveSymbiont("time_butterfly", EcosystemSystem::NaturalEvolution);
 * 
 * // 激活共生能力
 * ecosystemSystem.activateAbility("time_butterfly", "时间感知");
 * 
 * // 获取当前状态
 * qDebug() << "已解锁共生生物数量:" << ecosystemSystem.getUnlockedSymbionts().size();
 * qDebug() << "活跃共生体数量:" << ecosystemSystem.getActiveSymbiontCount();
 * qDebug() << "生态指数:" << ecosystemSystem.getEcoIndex();
 * qDebug() << "生物多样性指数:" << ecosystemSystem.getBiodiversityIndex();
 * @endcode
 */
class EcosystemSystem : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 共生生物类型
     * 
     * 定义共生生物的类型。
     * 每种类型都有独特的能力和特点。
     */
    enum CreatureType {
        FlyingType = 0,      ///< 飞行类
        AquaticType = 1,     ///< 水生类
        CombatType = 2,      ///< 战斗类
        PerceptionType = 3,  ///< 感知类
        SupportType = 4      ///< 辅助类
    };

    /**
     * @brief 共生进化路线
     * 
     * 定义共生生物的进化路线。
     * 每条路线都有独特的进化方向。
     */
    enum class EvolutionPath {
        NaturalEvolution = 0,   ///< 自然进化
        PsionicEvolution = 1,   ///< 灵能进化
        SymbioticEvolution = 2  ///< 共生进化
    };

    /**
     * @brief 共生生物信息结构
     * 
     * 存储共生生物的所有信息。
     */
    struct SymbiontInfo {
        QString id;                 ///< 生物ID
        QString name;               ///< 生物名称
        CreatureType type;          ///< 生物类型
        QString description;        ///< 生物描述
        int trustLevel;             ///< 信任度 0-100
        int symbiosisLevel;         ///< 共生等级 1-10
        EvolutionPath evolutionPath;  ///< 进化路线
        bool unlocked;              ///< 是否已解锁
        bool active;                ///< 是否活跃
        QList<QString> abilities;   ///< 能力列表
    };

    /**
     * @brief 生态事件类型
     * 
     * 定义生态事件的类型。
     * 每种事件都会对游戏世界产生影响。
     */
    enum class EcoEventType {
        Migration = 0,       ///< 生物迁徙
        Disaster = 1,        ///< 生态灾难
        Invasion = 2,        ///< 物种入侵
        PsionicSurge = 3     ///< 灵能潮涌
    };

    /**
     * @brief 构造函数
     * 
     * 创建生态系统实例，但不进行初始化。
     * 必须调用 initialize() 完成初始化后才能使用。
     * 
     * @param parent 父对象，用于 Qt 对象树内存管理
     */
    explicit EcosystemSystem(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     * 
     * 销毁生态系统，自动调用 shutdown() 清理资源。
     */
    ~EcosystemSystem() override;

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
     * @return true 初始化成功
     * @return false 初始化失败
     */
    bool initialize();
    
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
     */
    void shutdown();
    
    /**
     * @brief 更新生态系统
     * 
     * 每帧调用一次，更新生态系统状态。
     * 
     * 更新内容：
     * 1. 生态事件计时器
     * 2. 生态指数自然恢复
     * 
     * @param deltaTime 时间增量（秒）
     */
    void update(float deltaTime);

    // ============================================================
    // 共生生物管理
    // ============================================================
    
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
     * @param id 生物ID
     * @return true 解锁成功
     * @return false 解锁失败（生物不存在或已解锁）
     */
    bool unlockSymbiont(const QString& id);
    
    /**
     * @brief 获取共生生物信息
     * 
     * 获取指定共生生物的详细信息。
     * 
     * @param id 生物ID
     * @return 生物信息
     */
    SymbiontInfo getSymbiontInfo(const QString& id) const;
    
    /**
     * @brief 检查共生生物是否已解锁
     * @param id 生物ID
     * @return true 已解锁
     * @return false 未解锁
     */
    bool isSymbiontUnlocked(const QString& id) const;
    
    /**
     * @brief 获取已解锁共生生物列表
     * @return 生物ID列表
     */
    QStringList getUnlockedSymbionts() const;
    
    /**
     * @brief 获取所有共生生物
     * @return 生物映射
     */
    QMap<QString, SymbiontInfo> getAllSymbionts() const { return m_symbionts; }

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
     * @param id 生物ID
     * @return true 建立成功
     * @return false 建立失败
     */
    bool establishSymbiosis(const QString& id);
    
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
     * @param id 生物ID
     * @return true 解除成功
     * @return false 解除失败（生物不活跃）
     */
    bool breakSymbiosis(const QString& id);
    
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
     * @param id 生物ID
     * @param amount 增加量
     */
    void addTrust(const QString& id, int amount);
    
    /**
     * @brief 获取信任度
     * @param id 生物ID
     * @return 信任度（0-100）
     */
    int getTrustLevel(const QString& id) const;
    
    /**
     * @brief 获取活跃共生体数量
     * @return 数量
     */
    int getActiveSymbiontCount() const;
    
    /**
     * @brief 获取最大共生槽位
     * @return 槽位数
     */
    int getMaxSymbiontSlots() const { return m_maxSymbiontSlots; }
    
    /**
     * @brief 设置最大共生槽位
     * @param slots 槽位数
     */
    void setMaxSymbiontSlots(int slot) { m_maxSymbiontSlots = slot; }

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
     * @param id 生物ID
     * @param path 进化路线
     * @return true 进化成功
     * @return false 进化失败
     */
    bool evolveSymbiont(const QString& id, EvolutionPath path);
    
    /**
     * @brief 获取共生等级
     * @param id 生物ID
     * @return 等级（1-10）
     */
    int getSymbiosisLevel(const QString& id) const;

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
     * @param id 生物ID
     * @param abilityId 能力ID
     * @return true 激活成功
     * @return false 激活失败（生物不活跃）
     */
    bool activateAbility(const QString& id, const QString& abilityId);
    
    /**
     * @brief 获取共生生物能力列表
     * @param id 生物ID
     * @return 能力列表
     */
    QList<QString> getAbilities(const QString& id) const;

    // ============================================================
    // 生态平衡
    // ============================================================
    
    /**
     * @brief 获取生态指数
     * @return 生态指数（0-100）
     */
    float getEcoIndex() const { return m_ecoIndex; }
    
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
     * @param index 生态指数
     */
    void setEcoIndex(float index);
    
    /**
     * @brief 增加生态指数
     * @param amount 增加量
     */
    void addEcoIndex(float amount);
    
    /**
     * @brief 减少生态指数
     * @param amount 减少量
     */
    void reduceEcoIndex(float amount);

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
     * @param eventType 事件类型
     * @return true 触发成功
     * @return false 触发失败（已有活跃事件）
     */
    bool triggerEcoEvent(EcoEventType eventType);
    
    /**
     * @brief 获取当前生态事件
     * @return 事件类型
     */
    EcoEventType getCurrentEcoEvent() const { return m_currentEcoEvent; }
    
    /**
     * @brief 是否有活跃的生态事件
     * @return true 有事件
     * @return false 没有事件
     */
    bool hasActiveEcoEvent() const { return m_ecoEventActive; }

    // ============================================================
    // 生物多样性
    // ============================================================
    
    /**
     * @brief 获取生物多样性指数
     * @return 多样性指数
     */
    int getBiodiversityIndex() const { return m_biodiversityIndex; }
    
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
     * @param creatureId 生物ID
     */
    void recordCreatureDiscovery(const QString& creatureId);
    
    /**
     * @brief 获取已发现生物数量
     * @return 数量
     */
    int getDiscoveredCreatureCount() const { return m_discoveredCreatures.size(); }

signals:
    /**
     * @brief 共生生物解锁信号
     * 
     * 当共生生物解锁时发出，携带生物ID。
     * 可以连接此信号来更新UI或触发事件。
     * 
     * @param id 生物ID
     */
    void symbiontUnlocked(const QString& id);
    
    /**
     * @brief 共生关系建立信号
     * 
     * 当共生关系建立时发出，携带生物ID。
     * 可以连接此信号来更新UI或触发事件。
     * 
     * @param id 生物ID
     */
    void symbiosisEstablished(const QString& id);
    
    /**
     * @brief 共生关系解除信号
     * 
     * 当共生关系解除时发出，携带生物ID。
     * 可以连接此信号来更新UI或触发事件。
     * 
     * @param id 生物ID
     */
    void symbiosisBroken(const QString& id);
    
    /**
     * @brief 信任度变化信号
     * 
     * 当信任度变化时发出，携带生物ID和信任度。
     * 可以连接此信号来更新UI。
     * 
     * @param id 生物ID
     * @param trustLevel 信任度
     */
    void trustLevelChanged(const QString& id, int trustLevel);
    
    /**
     * @brief 共生进化信号
     * 
     * 当共生生物进化时发出，携带生物ID和新等级。
     * 可以连接此信号来更新UI或播放特效。
     * 
     * @param id 生物ID
     * @param newLevel 新等级
     */
    void symbiontEvolved(const QString& id, int newLevel);
    
    /**
     * @brief 生态指数变化信号
     * 
     * 当生态指数变化时发出，携带生态指数。
     * 可以连接此信号来更新UI或触发事件。
     * 
     * @param index 生态指数
     */
    void ecoIndexChanged(float index);
    
    /**
     * @brief 生态事件触发信号
     * 
     * 当生态事件触发时发出，携带事件类型。
     * 可以连接此信号来更新UI或触发事件。
     * 
     * @param eventType 事件类型
     */
    void ecoEventTriggered(EcoEventType eventType);
    
    /**
     * @brief 生物发现信号
     * 
     * 当发现新生物时发出，携带生物ID。
     * 可以连接此信号来更新UI或触发成就。
     * 
     * @param creatureId 生物ID
     */
    void creatureDiscovered(const QString& creatureId);

private:
    /**
     * @brief 初始化默认共生生物
     * 
     * 初始化游戏中的默认共生生物。
     * 包括4种稀有共生生物：
     * 1. 时空蝶
     * 2. 星核兽
     * 3. 虚空鲸
     * 4. 幻影狐
     */
    void initializeDefaultSymbionts();
    
    /**
     * @brief 更新生态指数
     * 
     * 更新生态指数的逻辑。
     * 目前为空，预留扩展接口。
     */
    void updateEcoIndex();

    // ============================================================
    // 成员变量
    // ============================================================
    
    // 共生生物映射
    QMap<QString, SymbiontInfo> m_symbionts;  ///< 生物ID到信息的映射
    
    // 活跃共生体
    QStringList m_activeSymbionts;   ///< 活跃共生体ID列表
    int m_maxSymbiontSlots = 3;      ///< 最大共生槽位数
    
    // 生态平衡
    float m_ecoIndex = 50.0f;     ///< 生态指数
    float m_maxEcoIndex = 100.0f; ///< 最大生态指数
    
    // 生态事件
    EcoEventType m_currentEcoEvent = EcoEventType::Migration;  ///< 当前生态事件类型
    bool m_ecoEventActive = false;               ///< 生态事件是否活跃
    float m_ecoEventTimer = 0.0f;                ///< 生态事件计时器
    
    // 生物多样性
    int m_biodiversityIndex = 0;      ///< 生物多样性指数
    QSet<QString> m_discoveredCreatures;  ///< 已发现生物集合
    
    // 初始化状态
    bool m_initialized = false;  ///< 是否已初始化
};

#endif // ECOSYSTEM_SYSTEM_H
