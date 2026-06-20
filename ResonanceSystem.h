/**
 * @file ResonanceSystem.h
 * @brief 星核共鸣系统
 * 
 * 本文件定义了星核共鸣系统，这是游戏的三大核心系统之一。
 * 星核共鸣系统管理星核收集、共鸣等级、环境操控等核心机制。
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

#ifndef RESONANCE_SYSTEM_H
#define RESONANCE_SYSTEM_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QPointF>

/**
 * @brief 星核共鸣系统
 * 
 * 管理星核收集、共鸣等级、环境操控等核心机制。
 * 星核共鸣系统是游戏的三大核心系统之一。
 * 
 * 架构说明：
 * - 星核系统继承 QObject，支持信号槽机制
 * - 星核解锁时会发出 starCoreUnlocked 信号
 * - 共鸣等级变化时会发出 resonanceLevelChanged 信号
 * - 共鸣激活时会发出 resonanceActivated 信号
 * - 反噬等级变化时会发出 backlashLevelChanged 信号
 * - 环境操控时会发出 environmentManipulated 信号
 * 
 * 星核系统：
 * - 支持6种星核：大地、海洋、烈焰、风暴、生命、虚空
 * - 每个星核都有独特的能力和人格
 * - 默认解锁大地星核
 * - 星核需要通过探索和任务解锁
 * 
 * 共鸣系统：
 * - 共鸣度范围：0-100
 * - 共鸣分为4个等级
 * - 共鸣度越高，能力越强
 * - 共鸣度可以通过使用和互动增加
 * 
 * 环境操控：
 * - 地形操控：改变地形
 * - 天气操控：改变天气
 * - 生命操控：影响生物
 * 
 * 反噬系统：
 * - 共鸣操作会产生反噬
 * - 反噬值范围：0-100
 * - 反噬分为5个等级
 * - 反噬会自然恢复
 * - 反噬等级越高，影响越大
 * 
 * 共鸣组合技：
 * - 需要至少2个星核
 * - 组合技威力强大
 * - 组合技会产生更多反噬
 * 
 * 使用示例：
 * @code
 * ResonanceSystem resonanceSystem;
 * resonanceSystem.initialize();
 * 
 * // 解锁星核
 * resonanceSystem.unlockStarCore(ResonanceSystem::Ocean);
 * 
 * // 增加共鸣度
 * resonanceSystem.addResonance(ResonanceSystem::Earth, 20.0f);
 * 
 * // 激活共鸣
 * resonanceSystem.activateResonance(ResonanceSystem::Earth);
 * 
 * // 环境操控
 * resonanceSystem.terrainManipulation(QPointF(100, 200), 50.0f);
 * 
 * // 获取当前状态
 * qDebug() << "已解锁星核数量:" << resonanceSystem.getUnlockedCount();
 * qDebug() << "当前激活星核:" << resonanceSystem.getStarCoreName(resonanceSystem.getActiveStarCore());
 * qDebug() << "反噬等级:" << resonanceSystem.getBacklashLevelName(resonanceSystem.getBacklashLevel());
 * @endcode
 */
class ResonanceSystem : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 星核类型
     * 
     * 定义游戏中的6种星核。
     * 每种星核都有独特的能力和人格。
     */
    enum class StarCoreType {
        Earth = 0,      ///< 大地星核
        Ocean = 1,      ///< 海洋星核
        Flame = 2,      ///< 烈焰星核
        Storm = 3,      ///< 风暴星核
        Life = 4,       ///< 生命星核
        Void = 5        ///< 虚空星核
    };

    /**
     * @brief 共鸣等级
     * 
     * 定义共鸣的等级。
     * 共鸣等级越高，星核能力越强。
     */
    enum class ResonanceLevel {
        ResonanceNone = 0,     ///< 无共鸣
        ResonanceShallow = 1,  ///< 浅层共鸣
        ResonanceDeep = 2,     ///< 深层共鸣
        ResonanceFull = 3      ///< 完全共鸣
    };

    /**
     * @brief 反噬等级
     * 
     * 定义反噬的等级。
     * 反噬等级越高，对玩家的影响越大。
     */
    enum class BacklashLevel {
        BacklashNone = 0,       ///< 无反噬
        BacklashMild = 1,       ///< 轻度反噬
        BacklashModerate = 2,   ///< 中度反噬
        BacklashSevere = 3,     ///< 重度反噬
        BacklashPermanent = 4   ///< 永久反噬
    };

    /**
     * @brief 星核信息结构
     * 
     * 存储星核的所有信息。
     */
    struct StarCoreInfo {
        StarCoreType type;          ///< 星核类型
        QString name;               ///< 星核名称
        QString description;        ///< 星核描述
        ResonanceLevel resonanceLevel;  ///< 共鸣等级
        float resonanceValue;       ///< 共鸣度 0-100
        float stability;            ///< 稳定度 0-100
        bool unlocked;              ///< 是否已解锁
        QString personality;        ///< 星核人格
    };

    /**
     * @brief 构造函数
     * 
     * 创建星核系统实例，但不进行初始化。
     * 必须调用 initialize() 完成初始化后才能使用。
     * 
     * @param parent 父对象，用于 Qt 对象树内存管理
     */
    explicit ResonanceSystem(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     * 
     * 销毁星核系统，自动调用 shutdown() 清理资源。
     */
    ~ResonanceSystem() override;

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
    bool initialize();
    
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
    void shutdown();
    
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
    void update(float deltaTime);

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
    bool unlockStarCore(StarCoreType type);
    
    /**
     * @brief 获取星核信息
     * 
     * 获取指定星核的详细信息。
     * 
     * @param type 星核类型
     * @return 星核信息
     */
    StarCoreInfo getStarCoreInfo(StarCoreType type) const;
    
    /**
     * @brief 检查星核是否已解锁
     * @param type 星核类型
     * @return true 已解锁
     * @return false 未解锁
     */
    bool isStarCoreUnlocked(StarCoreType type) const;
    
    /**
     * @brief 获取已解锁星核数量
     * @return 已解锁星核数量
     */
    int getUnlockedCount() const;
    
    /**
     * @brief 获取星核名称
     * 
     * 获取指定星核的中文名称。
     * 
     * @param type 星核类型
     * @return 星核名称
     */
    QString getStarCoreName(StarCoreType type) const;

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
    bool addResonance(StarCoreType type, float amount);
    
    /**
     * @brief 获取共鸣等级
     * @param type 星核类型
     * @return 共鸣等级
     */
    ResonanceLevel getResonanceLevel(StarCoreType type) const;
    
    /**
     * @brief 获取共鸣度值
     * @param type 星核类型
     * @return 共鸣度（0-100）
     */
    float getResonanceValue(StarCoreType type) const;
    
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
    bool activateResonance(StarCoreType type);
    
    /**
     * @brief 取消星核共鸣
     * 
     * 取消当前星核的共鸣。
     * 
     * @param type 星核类型（未使用，预留）
     * @return true 取消成功
     * @return false 取消失败
     */
    bool deactivateResonance(StarCoreType type);
    
    /**
     * @brief 获取当前激活的星核
     * @return 当前激活的星核类型
     */
    StarCoreType getActiveStarCore() const { return m_activeStarCore; }
    
    /**
     * @brief 是否有激活的星核
     * @return true 有激活的星核
     * @return false 没有激活的星核
     */
    bool hasActiveStarCore() const { return m_resonanceActive; }

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
    bool terrainManipulation(const QPointF& position, float radius);
    
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
    bool weatherManipulation(int weatherType);
    
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
    bool lifeManipulation(const QPointF& position, float radius);

    // ============================================================
    // 反噬系统
    // ============================================================
    
    /**
     * @brief 获取反噬等级
     * @return 反噬等级
     */
    BacklashLevel getBacklashLevel() const { return m_backlashLevel; }
    
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
    void addBacklash(float amount);
    
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
    void recoverBacklash(float amount);
    
    /**
     * @brief 获取反噬等级名称
     * 
     * 获取指定反噬等级的中文名称。
     * 
     * @param level 反噬等级
     * @return 反噬等级名称
     */
    QString getBacklashLevelName(BacklashLevel level) const;

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
    bool releaseCombo(int comboType);
    
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
    bool isComboAvailable(int comboType) const;

    // ============================================================
    // 灵能消耗
    // ============================================================
    
    /**
     * @brief 获取共鸣激活消耗
     * @return 灵能消耗
     */
    float getResonanceCost() const { return m_resonanceCost; }
    
    /**
     * @brief 设置共鸣激活消耗
     * @param cost 消耗
     */
    void setResonanceCost(float cost) { m_resonanceCost = cost; }

signals:
    /**
     * @brief 星核解锁信号
     * 
     * 当星核解锁时发出，携带星核类型。
     * 可以连接此信号来更新UI或触发事件。
     * 
     * @param type 星核类型
     */
    void starCoreUnlocked(StarCoreType type);
    
    /**
     * @brief 共鸣等级变化信号
     * 
     * 当共鸣等级变化时发出，携带星核类型和新等级。
     * 可以连接此信号来更新UI。
     * 
     * @param type 星核类型
     * @param level 新等级
     */
    void resonanceLevelChanged(StarCoreType type, ResonanceLevel level);
    
    /**
     * @brief 共鸣激活信号
     * 
     * 当共鸣激活时发出，携带星核类型。
     * 可以连接此信号来播放特效或更新UI。
     * 
     * @param type 星核类型
     */
    void resonanceActivated(StarCoreType type);
    
    /**
     * @brief 反噬等级变化信号
     * 
     * 当反噬等级变化时发出，携带新等级。
     * 可以连接此信号来更新UI或触发事件。
     * 
     * @param level 新等级
     */
    void backlashLevelChanged(BacklashLevel level);
    
    /**
     * @brief 环境操控信号
     * 
     * 当环境操控时发出，携带操控类型。
     * 可以连接此信号来播放特效或更新场景。
     * 
     * @param manipulationType 操控类型
     */
    void environmentManipulated(int manipulationType);

private:
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
    void updateBacklashLevel();
    
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
    void checkResonanceLevelUp(StarCoreType type);

    // ============================================================
    // 成员变量
    // ============================================================
    
    // 星核映射
    QMap<StarCoreType, StarCoreInfo> m_starCores;  ///< 星核类型到信息的映射
    
    // 当前激活的星核
    StarCoreType m_activeStarCore = StarCoreType::Earth;  ///< 当前激活的星核
    bool m_resonanceActive = false;         ///< 共鸣是否激活
    
    // 反噬系统
    float m_backlashValue = 0.0f;        ///< 反噬值
    float m_maxBacklash = 100.0f;        ///< 最大反噬值
    BacklashLevel m_backlashLevel = BacklashLevel::BacklashNone;  ///< 当前反噬等级
    
    // 灵能消耗
    float m_resonanceCost = 15.0f;  ///< 共鸣激活消耗
    
    // 初始化状态
    bool m_initialized = false;  ///< 是否已初始化
};

#endif // RESONANCE_SYSTEM_H
