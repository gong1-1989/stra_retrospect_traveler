/**
 * @file TimeSystem.h
 * @brief 时间褶皱系统
 * 
 * 本文件定义了时间褶皱系统，这是游戏的三大核心系统之一。
 * 时间褶皱系统管理时间线切换、时间悖论、时间熵等核心机制。
 * 
 * 主要功能：
 * 1. 时间线切换：支持在5条时间线之间切换
 * 2. 时间褶皱：创建和激活时间褶皱
 * 3. 时间熵：时间操作会增加熵值，熵值过高会有负面影响
 * 4. 悖论系统：时间操作会产生悖论，悖论等级越高影响越大
 * 5. 时间能力：时间减速、时间回溯等特殊能力
 * 
 * 时间线类型：
 * - Present：现在
 * - PastProsperity：过去·繁荣期
 * - PastDisaster：过去·灾难期
 * - FutureDecay：未来·衰败期
 * - FutureMutation：未来·变异期
 * 
 * 褶皱类型：
 * - Stable：稳定褶皱
 * - Unstable：不稳定褶皱
 * - Fractured：断裂褶皱
 * - Mirror：镜像褶皱
 * - Loop：循环褶皱
 * 
 * 悖论等级：
 * - ParadoxNone：无悖论
 * - ParadoxMinor：轻微悖论
 * - ParadoxModerate：中度悖论
 * - ParadoxSevere：重度悖论
 * - ParadoxCritical：临界悖论
 * 
 * 时间熵说明：
 * - 每次时间操作都会增加时间熵
 * - 时间熵会自然衰减（每秒0.5点）
 * - 熵值过高会有负面影响
 * - 可以通过特殊道具或技能减少熵值
 * 
 * 悖论系统说明：
 * - 每次时间操作都会产生悖论
 * - 悖论会自然修复（每秒0.3点）
 * - 悖论等级越高，对游戏的影响越大
 * - 可以通过特殊道具或技能修复悖论
 * 
 * 时间能力：
 * - 时间减速：减慢时间流速，消耗灵能
 * - 时间回溯：回溯时间，需要状态快照系统支持
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#ifndef TIME_SYSTEM_H
#define TIME_SYSTEM_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QPointF>

/**
 * @brief 时间褶皱系统
 * 
 * 管理时间线切换、时间悖论、时间熵等核心机制。
 * 时间褶皱系统是游戏的三大核心系统之一。
 * 
 * 架构说明：
 * - 时间系统继承 QObject，支持信号槽机制
 * - 时间线切换时会发出 timelineChanged 信号
 * - 时间熵变化时会发出 entropyChanged 信号
 * - 悖论等级变化时会发出 paradoxLevelChanged 信号
 * - 时间减速开始和结束时会发出相应信号
 * 
 * 时间线系统：
 * - 支持5条时间线：现在、过去繁荣期、过去灾难期、未来衰败期、未来变异期
 * - 切换时间线需要消耗灵能
 * - 切换时间线会增加时间熵和悖论
 * - 不同时间线有不同的环境和敌人
 * 
 * 时间褶皱：
 * - 可以创建和激活时间褶皱
 * - 褶皱有不同类型，效果不同
 * - 激活褶皱会增加时间熵
 * 
 * 时间熵：
 * - 时间操作会增加熵值
 * - 熵值范围：0-100
 * - 熵值会自然衰减
 * - 熵值过高会有负面影响
 * 
 * 悖论系统：
 * - 时间操作会产生悖论
 * - 悖论值范围：0-100
 * - 悖论分为5个等级
 * - 悖论会自然修复
 * - 悖论等级越高，影响越大
 * 
 * 时间能力：
 * - 时间减速：减慢时间流速
 * - 时间回溯：回溯时间（占位实现）
 * 
 * 使用示例：
 * @code
 * TimeSystem timeSystem;
 * timeSystem.initialize();
 * 
 * // 切换时间线
 * timeSystem.switchTimeline(TimeSystem::PastProsperity);
 * 
 * // 创建时间褶皱
 * timeSystem.createFold("遗迹入口", TimeSystem::Stable, QPointF(100, 200));
 * 
 * // 激活时间褶皱
 * timeSystem.activateFold("遗迹入口");
 * 
 * // 时间减速
 * timeSystem.timeSlow(5.0f, 0.3f);
 * 
 * // 获取当前状态
 * qDebug() << "当前时间线:" << timeSystem.getTimelineName(timeSystem.getCurrentTimeline());
 * qDebug() << "时间熵:" << timeSystem.getTimeEntropy();
 * qDebug() << "悖论等级:" << timeSystem.getParadoxLevelName(timeSystem.getParadoxLevel());
 * @endcode
 */
class TimeSystem : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 时间线类型
     * 
     * 定义游戏中的5条时间线。
     * 每条时间线有不同的环境、敌人和剧情。
     */
    enum TimelineType {
        Present = 0,           ///< 现在
        PastProsperity = 1,    ///< 过去·繁荣期
        PastDisaster = 2,      ///< 过去·灾难期
        FutureDecay = 3,       ///< 未来·衰败期
        FutureMutation = 4     ///< 未来·变异期
    };

    /**
     * @brief 褶皱类型
     * 
     * 定义时间褶皱的类型。
     * 不同类型的褶皱有不同的效果和风险。
     */
    enum FoldType {
        Stable = 0,      ///< 稳定褶皱
        Unstable = 1,    ///< 不稳定褶皱
        Fractured = 2,   ///< 断裂褶皱
        Mirror = 3,      ///< 镜像褶皱
        Loop = 4         ///< 循环褶皱
    };

    /**
     * @brief 悖论等级
     * 
     * 定义悖论的等级。
     * 悖论等级越高，对游戏的影响越大。
     */
    enum ParadoxLevel {
        ParadoxNone = 0,       ///< 无悖论
        ParadoxMinor = 1,      ///< 轻微悖论
        ParadoxModerate = 2,   ///< 中度悖论
        ParadoxSevere = 3,     ///< 重度悖论
        ParadoxCritical = 4    ///< 临界悖论
    };

    /**
     * @brief 构造函数
     * 
     * 创建时间系统实例，但不进行初始化。
     * 必须调用 initialize() 完成初始化后才能使用。
     * 
     * @param parent 父对象，用于 Qt 对象树内存管理
     */
    explicit TimeSystem(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     * 
     * 销毁时间系统，自动调用 shutdown() 清理资源。
     */
    ~TimeSystem() override;

    // ============================================================
    // 生命周期管理
    // ============================================================
    
    /**
     * @brief 初始化时间系统
     * 
     * 初始化时间系统的所有属性和状态。
     * 
     * 执行以下初始化操作：
     * 1. 检查是否已经初始化（防止重复初始化）
     * 2. 初始化当前时间线（现在）
     * 3. 初始化时间熵（0）
     * 4. 初始化悖论系统（无悖论）
     * 5. 初始化时间缩放（正常速度）
     * 6. 设置初始化标志
     * 
     * @return true 初始化成功
     * @return false 初始化失败
     */
    bool initialize();
    
    /**
     * @brief 关闭时间系统
     * 
     * 关闭时间系统，清理资源。
     * 
     * 执行以下操作：
     * 1. 检查是否已经初始化
     * 2. 清空褶皱映射
     * 3. 重置初始化标志
     * 
     * 注意：
     * - 此函数是幂等的，可以安全地多次调用
     * - 不需要手动调用，析构函数会自动调用
     */
    void shutdown();
    
    /**
     * @brief 更新时间系统
     * 
     * 每帧调用一次，更新时间系统状态。
     * 
     * 更新内容：
     * 1. 更新时间减速（检查是否结束）
     * 2. 时间熵自然衰减（每秒0.5点）
     * 3. 悖论自然修复（每秒0.3点）
     * 
     * @param deltaTime 时间增量（秒）
     */
    void update(float deltaTime);

    // ============================================================
    // 时间线切换
    // ============================================================
    
    /**
     * @brief 切换时间线
     * 
     * 切换到指定的时间线。
     * 切换时间线会增加时间熵和悖论。
     * 
     * 执行以下操作：
     * 1. 检查是否已经在目标时间线
     * 2. 设置新的时间线
     * 3. 增加时间熵（5点）
     * 4. 增加悖论（3点）
     * 5. 发出 timelineChanged 信号
     * 
     * 注意：
     * - 如果已经在目标时间线，返回 false
     * - 切换时间线会增加时间熵和悖论
     * - 切换时间线需要消耗灵能（在外部检查）
     * 
     * @param timeline 目标时间线
     * @return true 切换成功
     * @return false 切换失败（已经在目标时间线）
     */
    bool switchTimeline(TimelineType timeline);
    
    /**
     * @brief 获取当前时间线
     * @return 当前时间线
     */
    TimelineType getCurrentTimeline() const { return m_currentTimeline; }
    
    /**
     * @brief 获取时间线名称
     * 
     * 获取指定时间线的中文名称。
     * 
     * @param timeline 时间线类型
     * @return 时间线名称
     */
    QString getTimelineName(TimelineType timeline) const;

    // ============================================================
    // 时间褶皱
    // ============================================================
    
    /**
     * @brief 创建时间褶皱
     * 
     * 在指定位置创建一个时间褶皱。
     * 
     * 执行以下操作：
     * 1. 检查褶皱名称是否已存在
     * 2. 如果已存在，输出警告，返回 false
     * 3. 创建褶皱并添加到映射
     * 4. 返回 true
     * 
     * 注意：
     * - 褶皱名称必须唯一
     * - position 参数目前未使用，预留用于未来扩展
     * 
     * @param name 褶皱名称
     * @param type 褶皱类型
     * @param position 褶皱位置
     * @return true 创建成功
     * @return false 创建失败（名称已存在）
     */
    bool createFold(const QString& name, FoldType type, const QPointF& position);
    
    /**
     * @brief 激活时间褶皱
     * 
     * 激活指定的时间褶皱。
     * 激活褶皱会增加时间熵。
     * 
     * 执行以下操作：
     * 1. 检查褶皱是否存在
     * 2. 如果不存在，输出警告，返回 false
     * 3. 增加时间熵（10点）
     * 4. 返回 true
     * 
     * 注意：
     * - 激活褶皱会增加时间熵
     * - 具体的褶皱效果需要在游戏逻辑中实现
     * 
     * @param name 褶皱名称
     * @return true 激活成功
     * @return false 激活失败（褶皱不存在）
     */
    bool activateFold(const QString& name);
    
    /**
     * @brief 获取褶皱类型名称
     * 
     * 获取指定褶皱类型的中文名称。
     * 
     * @param type 褶皱类型
     * @return 褶皱类型名称
     */
    QString getFoldTypeName(FoldType type) const;

    // ============================================================
    // 时间熵
    // ============================================================
    
    /**
     * @brief 获取时间熵值
     * 
     * 时间熵范围是 0-100。
     * 时间熵越高，时间操作的风险越大。
     * 
     * @return 熵值（0-100）
     */
    float getTimeEntropy() const { return m_timeEntropy; }
    
    /**
     * @brief 增加时间熵
     * 
     * 增加时间熵值。
     * 熵值会自动限制在最大值以内。
     * 
     * 执行以下操作：
     * 1. 增加熵值（最大为最大值）
     * 2. 发出 entropyChanged 信号
     * 3. 如果熵值达到最大值，输出警告
     * 
     * @param amount 增加量
     */
    void addEntropy(float amount);
    
    /**
     * @brief 减少时间熵
     * 
     * 减少时间熵值。
     * 熵值会自动限制在最小值（0）以上。
     * 
     * 执行以下操作：
     * 1. 减少熵值（最小为 0）
     * 2. 发出 entropyChanged 信号
     * 
     * @param amount 减少量
     */
    void reduceEntropy(float amount);
    
    /**
     * @brief 重置时间熵
     * 
     * 将时间熵重置为 0。
     * 
     * 执行以下操作：
     * 1. 重置熵值为 0
     * 2. 发出 entropyChanged 信号
     */
    void resetEntropy();

    // ============================================================
    // 悖论系统
    // ============================================================
    
    /**
     * @brief 获取当前悖论等级
     * @return 悖论等级
     */
    ParadoxLevel getParadoxLevel() const { return m_paradoxLevel; }
    
    /**
     * @brief 增加悖论
     * 
     * 增加悖论值。
     * 悖论值会自动限制在最大值以内。
     * 增加悖论后会自动更新悖论等级。
     * 
     * 执行以下操作：
     * 1. 增加悖论值（最大为最大值）
     * 2. 更新悖论等级
     * 
     * @param amount 增加量
     */
    void addParadox(float amount);
    
    /**
     * @brief 修复悖论
     * 
     * 减少悖论值（修复悖论）。
     * 悖论值会自动限制在最小值（0）以上。
     * 修复悖论后会自动更新悖论等级。
     * 
     * 执行以下操作：
     * 1. 减少悖论值（最小为 0）
     * 2. 更新悖论等级
     * 
     * @param amount 修复量
     */
    void repairParadox(float amount);
    
    /**
     * @brief 获取悖论等级名称
     * 
     * 获取指定悖论等级的中文名称。
     * 
     * @param level 悖论等级
     * @return 悖论等级名称
     */
    QString getParadoxLevelName(ParadoxLevel level) const;

    // ============================================================
    // 时间能力
    // ============================================================
    
    /**
     * @brief 时间减速
     * 
     * 启动时间减速效果。
     * 时间减速会减慢游戏时间的流速。
     * 
     * 执行以下操作：
     * 1. 检查是否已经在时间减速中
     * 2. 如果已经在减速中，输出警告，返回 false
     * 3. 设置时间减速标志
     * 4. 设置时间缩放因子（限制在 0.1-1.0）
     * 5. 设置减速持续时间
     * 6. 重置减速计时器
     * 7. 发出 timeSlowStarted 信号
     * 8. 返回 true
     * 
     * 注意：
     * - 减速因子范围：0.1-1.0
     * - 0.1 表示时间流速为正常的 10%
     * - 1.0 表示正常速度
     * 
     * @param duration 持续时间（秒）
     * @param slowFactor 减速因子（0.1-1.0）
     * @return true 启动成功
     * @return false 启动失败（已经在减速中）
     */
    bool timeSlow(float duration, float slowFactor = 0.5f);
    
    /**
     * @brief 时间回溯
     * 
     * 回溯指定秒数的时间。
     * 
     * 注意：
     * - 完整的时间回溯需要状态快照系统
     * - 目前只是占位实现
     * - 未来版本会实现完整的时间回溯功能
     * 
     * @param seconds 回溯秒数
     * @return true 回溯成功
     * @return false 回溯失败
     */
    bool timeRewind(float seconds);
    
    /**
     * @brief 是否正在时间减速
     * @return true 正在减速
     * @return false 未减速
     */
    bool isTimeSlowed() const { return m_timeSlowed; }
    
    /**
     * @brief 获取时间缩放因子
     * 
     * 获取当前的时间缩放因子。
     * 1.0 表示正常速度，0.5 表示半速。
     * 
     * @return 缩放因子
     */
    float getTimeScale() const { return m_timeScale; }

    // ============================================================
    // 灵能消耗
    // ============================================================
    
    /**
     * @brief 获取时间线切换消耗
     * @return 灵能消耗
     */
    float getTimelineSwitchCost() const { return m_timelineSwitchCost; }
    
    /**
     * @brief 设置时间线切换消耗
     * @param cost 消耗
     */
    void setTimelineSwitchCost(float cost) { m_timelineSwitchCost = cost; }

signals:
    /**
     * @brief 时间线切换信号
     * 
     * 当时间线切换时发出，携带新时间线。
     * 可以连接此信号来更新场景或UI。
     * 
     * @param newTimeline 新时间线
     */
    void timelineChanged(TimelineType newTimeline);
    
    /**
     * @brief 时间熵变化信号
     * 
     * 当时间熵变化时发出，携带当前熵值。
     * 可以连接此信号来更新UI。
     * 
     * @param entropy 当前熵值
     */
    void entropyChanged(float entropy);
    
    /**
     * @brief 悖论等级变化信号
     * 
     * 当悖论等级变化时发出，携带新等级。
     * 可以连接此信号来更新UI或触发事件。
     * 
     * @param level 新等级
     */
    void paradoxLevelChanged(ParadoxLevel level);
    
    /**
     * @brief 时间减速开始信号
     * 
     * 当时间减速开始时发出。
     * 可以连接此信号来播放特效或更新UI。
     */
    void timeSlowStarted();
    
    /**
     * @brief 时间减速结束信号
     * 
     * 当时间减速结束时发出。
     * 可以连接此信号来停止特效或更新UI。
     */
    void timeSlowEnded();

private:
    /**
     * @brief 更新悖论等级
     * 
     * 根据当前悖论值更新悖论等级。
     * 如果等级发生变化，会发出 paradoxLevelChanged 信号。
     * 
     * 等级划分：
     * - 0-20：无悖论
     * - 20-40：轻微悖论
     * - 40-60：中度悖论
     * - 60-80：重度悖论
     * - 80-100：临界悖论
     */
    void updateParadoxLevel();

    // ============================================================
    // 成员变量
    // ============================================================
    
    // 时间线
    TimelineType m_currentTimeline = Present;  ///< 当前时间线
    
    // 时间熵
    float m_timeEntropy = 0.0f;    ///< 时间熵值
    float m_maxEntropy = 100.0f;   ///< 最大时间熵
    
    // 悖论系统
    float m_paradoxValue = 0.0f;       ///< 悖论值
    float m_maxParadox = 100.0f;       ///< 最大悖论值
    ParadoxLevel m_paradoxLevel = ParadoxNone;  ///< 当前悖论等级
    
    // 时间缩放
    bool m_timeSlowed = false;      ///< 是否正在时间减速
    float m_timeScale = 1.0f;       ///< 时间缩放因子
    float m_slowDuration = 0.0f;    ///< 减速持续时间
    float m_slowTimer = 0.0f;       ///< 减速计时器
    
    // 灵能消耗
    float m_timelineSwitchCost = 20.0f;  ///< 时间线切换消耗
    
    // 褶皱映射
    QMap<QString, FoldType> m_folds;  ///< 褶皱名称到类型的映射
    
    // 初始化状态
    bool m_initialized = false;  ///< 是否已初始化
};

#endif // TIME_SYSTEM_H
