/**
 * @file TimeSystem.cpp
 * @brief 时间褶皱系统实现
 * 
 * 本文件实现了时间褶皱系统的所有功能，包括：
 * - 时间系统的初始化和关闭
 * - 时间线切换
 * - 时间褶皱的创建和激活
 * - 时间熵管理
 * - 悖论系统
 * - 时间能力（时间减速、时间回溯）
 * 
 * 时间褶皱系统是游戏的三大核心系统之一，
 * 管理时间线切换、时间悖论、时间熵等核心机制。
 * 
 * 主要功能：
 * 1. 时间线切换：支持在5条时间线之间切换
 * 2. 时间褶皱：创建和激活时间褶皱
 * 3. 时间熵：时间操作会增加熵值，熵值过高会有负面影响
 * 4. 悖论系统：时间操作会产生悖论，悖论等级越高影响越大
 * 5. 时间能力：时间减速、时间回溯等特殊能力
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

#include "TimeSystem.h"

#include <QDebug>
#include <cmath>

// ============================================================
// 构造函数与析构函数
// ============================================================

/**
 * @brief 构造函数
 * 
 * 创建时间系统实例，但不进行任何初始化操作。
 * 所有的初始化工作都在 initialize() 中完成。
 * 
 * @param parent 父对象，用于 Qt 对象树内存管理
 */
TimeSystem::TimeSystem(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[TimeSystem] 创建时间系统对象";
}

/**
 * @brief 析构函数
 * 
 * 销毁时间系统，自动调用 shutdown() 清理资源。
 */
TimeSystem::~TimeSystem()
{
    qDebug() << "[TimeSystem] 销毁时间系统对象";
    shutdown();
}

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
bool TimeSystem::initialize()
{
    // 防止重复初始化
    if (m_initialized) {
        qWarning() << "[TimeSystem] 时间系统已经初始化，跳过重复初始化";
        return true;
    }

    qDebug() << "[TimeSystem] 初始化时间系统...";
    
    // 1. 初始化当前时间线
    m_currentTimeline = Present;
    
    // 2. 初始化时间熵
    m_timeEntropy = 0.0f;
    
    // 3. 初始化悖论系统
    m_paradoxValue = 0.0f;
    m_paradoxLevel = ParadoxNone;
    
    // 4. 初始化时间缩放
    m_timeScale = 1.0f;
    m_timeSlowed = false;
    
    // 5. 设置初始化标志
    m_initialized = true;
    
    qDebug() << "[TimeSystem] 时间系统初始化完成";
    qDebug() << "[TimeSystem] 当前时间线:" << getTimelineName(m_currentTimeline);
    
    return true;
}

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
void TimeSystem::shutdown()
{
    // 如果没有初始化，直接返回
    if (!m_initialized) {
        return;
    }

    qDebug() << "[TimeSystem] 关闭时间系统...";
    
    // 清空褶皱映射
    m_folds.clear();
    
    // 重置初始化标志
    m_initialized = false;
    
    qDebug() << "[TimeSystem] 时间系统已关闭";
}

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
void TimeSystem::update(float deltaTime)
{
    // 如果没有初始化，不更新
    if (!m_initialized) {
        return;
    }

    // 1. 更新时间减速
    if (m_timeSlowed) {
        m_slowTimer += deltaTime;
        if (m_slowTimer >= m_slowDuration) {
            // 时间减速结束
            m_timeSlowed = false;
            m_timeScale = 1.0f;
            emit timeSlowEnded();
            qDebug() << "[TimeSystem] 时间减速结束";
        }
    }
    
    // 2. 时间熵自然衰减
    if (m_timeEntropy > 0) {
        // 每秒衰减 0.5 点
        m_timeEntropy = std::max(0.0f, m_timeEntropy - 0.5f * deltaTime);
        emit entropyChanged(m_timeEntropy);
    }
    
    // 3. 悖论自然修复
    if (m_paradoxValue > 0) {
        // 每秒修复 0.3 点
        m_paradoxValue = std::max(0.0f, m_paradoxValue - 0.3f * deltaTime);
        updateParadoxLevel();
    }
}

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
bool TimeSystem::switchTimeline(TimelineType timeline)
{
    // 如果已经在目标时间线，返回 false
    if (timeline == m_currentTimeline) {
        return false;
    }

    qDebug() << "[TimeSystem] 切换时间线:" 
             << getTimelineName(m_currentTimeline) << "->" << getTimelineName(timeline);
    
    // 设置新的时间线
    m_currentTimeline = timeline;
    
    // 切换时间线增加熵
    addEntropy(5.0f);
    
    // 切换时间线增加悖论
    addParadox(3.0f);
    
    // 发出时间线变化信号
    emit timelineChanged(timeline);
    
    return true;
}

/**
 * @brief 获取时间线名称
 * 
 * 获取指定时间线的中文名称。
 * 
 * @param timeline 时间线类型
 * @return 时间线名称
 */
QString TimeSystem::getTimelineName(TimelineType timeline) const
{
    switch (timeline) {
        case Present:         return "现在";
        case PastProsperity:  return "过去·繁荣期";
        case PastDisaster:    return "过去·灾难期";
        case FutureDecay:     return "未来·衰败期";
        case FutureMutation:  return "未来·变异期";
        default:              return "未知";
    }
}

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
bool TimeSystem::createFold(const QString& name, FoldType type, const QPointF& position)
{
    Q_UNUSED(position);
    
    // 检查名称是否已存在
    if (m_folds.contains(name)) {
        qWarning() << "[TimeSystem] 褶皱已存在:" << name;
        return false;
    }
    
    // 创建褶皱
    m_folds[name] = type;
    qDebug() << "[TimeSystem] 创建时间褶皱:" << name 
             << "类型:" << getFoldTypeName(type);
    
    return true;
}

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
bool TimeSystem::activateFold(const QString& name)
{
    // 检查褶皱是否存在
    if (!m_folds.contains(name)) {
        qWarning() << "[TimeSystem] 褶皱不存在:" << name;
        return false;
    }
    
    qDebug() << "[TimeSystem] 激活时间褶皱:" << name;
    
    // 激活褶皱增加熵
    addEntropy(10.0f);
    
    return true;
}

/**
 * @brief 获取褶皱类型名称
 * 
 * 获取指定褶皱类型的中文名称。
 * 
 * @param type 褶皱类型
 * @return 褶皱类型名称
 */
QString TimeSystem::getFoldTypeName(FoldType type) const
{
    switch (type) {
        case Stable:     return "稳定褶皱";
        case Unstable:   return "不稳定褶皱";
        case Fractured:  return "断裂褶皱";
        case Mirror:     return "镜像褶皱";
        case Loop:       return "循环褶皱";
        default:         return "未知";
    }
}

// ============================================================
// 时间熵
// ============================================================

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
void TimeSystem::addEntropy(float amount)
{
    // 增加熵值，最大为最大值
    m_timeEntropy = std::min(m_maxEntropy, m_timeEntropy + amount);
    emit entropyChanged(m_timeEntropy);
    
    // 如果熵值达到最大值，输出警告
    if (m_timeEntropy >= m_maxEntropy) {
        qWarning() << "[TimeSystem] 时间熵达到最大值！";
    }
}

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
void TimeSystem::reduceEntropy(float amount)
{
    // 减少熵值，最小为 0
    m_timeEntropy = std::max(0.0f, m_timeEntropy - amount);
    emit entropyChanged(m_timeEntropy);
}

/**
 * @brief 重置时间熵
 * 
 * 将时间熵重置为 0。
 * 
 * 执行以下操作：
 * 1. 重置熵值为 0
 * 2. 发出 entropyChanged 信号
 */
void TimeSystem::resetEntropy()
{
    // 重置熵值为 0
    m_timeEntropy = 0.0f;
    emit entropyChanged(m_timeEntropy);
    qDebug() << "[TimeSystem] 时间熵已重置";
}

// ============================================================
// 悖论系统
// ============================================================

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
void TimeSystem::addParadox(float amount)
{
    // 增加悖论值，最大为最大值
    m_paradoxValue = std::min(m_maxParadox, m_paradoxValue + amount);
    updateParadoxLevel();
}

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
void TimeSystem::repairParadox(float amount)
{
    // 减少悖论值，最小为 0
    m_paradoxValue = std::max(0.0f, m_paradoxValue - amount);
    updateParadoxLevel();
}

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
void TimeSystem::updateParadoxLevel()
{
    // 保存旧等级
    ParadoxLevel oldLevel = m_paradoxLevel;
    
    // 根据悖论值计算新等级
    if (m_paradoxValue >= 80.0f) {
        m_paradoxLevel = ParadoxCritical;
    } else if (m_paradoxValue >= 60.0f) {
        m_paradoxLevel = ParadoxSevere;
    } else if (m_paradoxValue >= 40.0f) {
        m_paradoxLevel = ParadoxModerate;
    } else if (m_paradoxValue >= 20.0f) {
        m_paradoxLevel = ParadoxMinor;
    } else {
        m_paradoxLevel = ParadoxNone;
    }
    
    // 如果等级发生变化，发出信号
    if (m_paradoxLevel != oldLevel) {
        emit paradoxLevelChanged(m_paradoxLevel);
        qDebug() << "[TimeSystem] 悖论等级变化:" << getParadoxLevelName(oldLevel) 
                 << "->" << getParadoxLevelName(m_paradoxLevel);
    }
}

/**
 * @brief 获取悖论等级名称
 * 
 * 获取指定悖论等级的中文名称。
 * 
 * @param level 悖论等级
 * @return 悖论等级名称
 */
QString TimeSystem::getParadoxLevelName(ParadoxLevel level) const
{
    switch (level) {
        case ParadoxNone:      return "无悖论";
        case ParadoxMinor:     return "轻微悖论";
        case ParadoxModerate:  return "中度悖论";
        case ParadoxSevere:    return "重度悖论";
        case ParadoxCritical:  return "临界悖论";
        default:               return "未知";
    }
}

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
bool TimeSystem::timeSlow(float duration, float slowFactor)
{
    // 如果已经在时间减速中，返回 false
    if (m_timeSlowed) {
        qWarning() << "[TimeSystem] 时间已经在减速中";
        return false;
    }

    // 设置时间减速
    m_timeSlowed = true;
    // 限制减速因子在 0.1-1.0 范围内
    m_timeScale = std::max(0.1f, std::min(1.0f, slowFactor));
    m_slowDuration = duration;
    m_slowTimer = 0.0f;
    
    // 发出时间减速开始信号
    emit timeSlowStarted();
    qDebug() << "[TimeSystem] 时间减速开始，因子:" << m_timeScale 
             << "持续时间:" << duration << "秒";
    
    return true;
}

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
bool TimeSystem::timeRewind(float seconds)
{
    Q_UNUSED(seconds);
    
    qDebug() << "[TimeSystem] 请求时间回溯:" << seconds << "秒";
    qDebug() << "[TimeSystem] 注意：完整的时间回溯需要状态快照系统，目前只是占位实现";
    
    // 完整的时间回溯需要状态快照系统，这里只做占位实现
    return true;
}
