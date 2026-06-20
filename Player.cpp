/**
 * @file Player.cpp
 * @brief 玩家类实现
 * 
 * 本文件实现了玩家类的所有功能，包括：
 * - 玩家的初始化和关闭
 * - 玩家属性的管理（生命值、灵能值、经验值、等级等）
 * - 玩家状态的管理
 * - 玩家位置和移动
 * - 玩家稳定度的管理
 * - 经验值和等级系统
 * 
 * 玩家类是游戏的核心角色类，
 * 包含了玩家的所有属性和行为。
 * 
 * 主要功能：
 * 1. 属性系统：生命值、灵能值、经验值、等级、稳定度等
 * 2. 状态系统：空闲、行走、奔跑、跳跃、攻击、受伤、死亡等状态
 * 3. 成长系统：经验值、等级提升、属性成长
 * 4. 位置系统：位置、移动速度、移动控制
 * 5. 信号通知：属性变化时发出信号，便于UI更新
 * 
 * 成长系统说明：
 * - 经验值获取：击败敌人、完成任务等获得经验值
 * - 等级提升：经验值达到要求后自动升级
 * - 属性成长：升级后生命值+10，灵能值+5
 * - 生命和灵能会在升级时回满
 * - 升级公式：所需经验 = 100 * 1.5^(等级-1)
 * 
 * 自然恢复：
 * - 灵能自然恢复：每秒恢复 2 点灵能
 * - 稳定度自然恢复：每秒恢复 1 点稳定度
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#include "Player.h"

#include <QDebug>
#include <cmath>

// ============================================================
// 构造函数与析构函数
// ============================================================

/**
 * @brief 构造函数
 * 
 * 创建玩家实例，但不进行任何初始化操作。
 * 所有的初始化工作都在 initialize() 中完成。
 * 
 * @param parent 父对象，用于 Qt 对象树内存管理
 */
Player::Player(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[Player] 创建玩家对象";
}

/**
 * @brief 析构函数
 * 
 * 销毁玩家，自动调用 shutdown() 清理资源。
 */
Player::~Player()
{
    qDebug() << "[Player] 销毁玩家对象";
    shutdown();
}

// ============================================================
// 生命周期管理
// ============================================================

/**
 * @brief 初始化玩家
 * 
 * 初始化玩家的所有属性和状态。
 * 
 * 执行以下初始化操作：
 * 1. 检查是否已经初始化（防止重复初始化）
 * 2. 初始化基本信息（名称、等级、经验值）
 * 3. 初始化生命值（最大生命值、当前生命值）
 * 4. 初始化灵能值（最大灵能值、当前灵能值）
 * 5. 初始化位置（初始位置、移动速度）
 * 6. 初始化状态（空闲状态）
 * 7. 初始化稳定度
 * 8. 设置初始化标志
 * 
 * 注意：
 * - 初始位置设置为 (400, 300)，这是场景的中心位置
 * - 初始移动速度为 200 像素/秒
 * - 初始稳定度为 100（最大值）
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
bool Player::initialize()
{
    // 防止重复初始化
    if (m_initialized) {
        qWarning() << "[Player] 玩家已经初始化，跳过重复初始化";
        return true;
    }

    qDebug() << "[Player] 初始化玩家...";
    
    // 1. 初始化基本信息
    m_name = "旅者";
    m_level = 1;
    m_experience = 0;
    
    // 2. 初始化生命值
    m_maxHealth = 100.0f;
    m_currentHealth = m_maxHealth;
    
    // 3. 初始化灵能值
    m_maxMana = 50.0f;
    m_currentMana = m_maxMana;
    
    // 4. 初始化位置
    m_position = QPointF(400, 300);
    m_moveSpeed = 200.0f;
    
    // 5. 初始化状态
    m_state = Idle;
    
    // 6. 初始化稳定度
    m_stability = 100.0f;
    m_maxStability = 100.0f;
    
    // 7. 设置初始化标志
    m_initialized = true;
    
    qDebug() << "[Player] 玩家初始化完成";
    qDebug() << "[Player] 名称:" << m_name 
             << "等级:" << m_level 
             << "生命:" << m_currentHealth << "/" << m_maxHealth
             << "灵能:" << m_currentMana << "/" << m_maxMana;
    
    return true;
}

/**
 * @brief 关闭玩家
 * 
 * 关闭玩家，清理资源。
 * 目前主要是重置初始化标志。
 * 
 * 注意：
 * - 此函数是幂等的，可以安全地多次调用
 * - 不需要手动调用，析构函数会自动调用
 */
void Player::shutdown()
{
    // 如果没有初始化，直接返回
    if (!m_initialized) {
        return;
    }

    qDebug() << "[Player] 关闭玩家...";
    
    // 重置初始化标志
    m_initialized = false;
    
    qDebug() << "[Player] 玩家已关闭";
}

/**
 * @brief 更新玩家
 * 
 * 每帧调用一次，更新玩家状态。
 * 只有在玩家存活时才会更新。
 * 
 * 更新内容：
 * 1. 灵能自然恢复（每秒恢复 2 点）
 * 2. 稳定度自然恢复（每秒恢复 1 点）
 * 
 * 注意：
 * - 只有在玩家存活时才会更新
 * - 灵能恢复速率：2.0 点/秒
 * - 稳定度恢复速率：1.0 点/秒
 * - 恢复时会发出相应的信号
 * 
 * @param deltaTime 时间增量（秒）
 */
void Player::update(float deltaTime)
{
    // 如果没有初始化或玩家已死亡，不更新
    if (!m_initialized || m_state == Dead) {
        return;
    }

    // 1. 灵能自然恢复
    if (m_currentMana < m_maxMana) {
        // 每秒恢复 2 点灵能
        m_currentMana = std::min(m_maxMana, m_currentMana + 2.0f * deltaTime);
        emit manaChanged(m_currentMana, m_maxMana);
    }
    
    // 2. 稳定度自然恢复
    if (m_stability < m_maxStability) {
        // 每秒恢复 1 点稳定度
        m_stability = std::min(m_maxStability, m_stability + 1.0f * deltaTime);
        emit stabilityChanged(m_stability);
    }
}

// ============================================================
// 经验值相关
// ============================================================

/**
 * @brief 增加经验值
 * 
 * 增加玩家的经验值。
 * 增加经验值后会检查是否升级。
 * 
 * 执行以下操作：
 * 1. 增加经验值
 * 2. 发出 experienceChanged 信号
 * 3. 检查是否升级（可能连续升级）
 * 
 * 注意：
 * - 经验值可以超过升级所需经验
 * - 升级后剩余的经验值会保留
 * - 支持连续升级（一次获得大量经验可能升多级）
 * 
 * @param exp 经验值
 */
void Player::addExperience(int exp)
{
    // 增加经验值
    m_experience += exp;
    qDebug() << "[Player] 玩家获得" << exp << "点经验值，当前经验:" << m_experience;
    
    // 发出经验值变化信号
    emit experienceChanged(m_experience, m_level);
    
    // 检查是否升级
    checkLevelUp();
}

/**
 * @brief 检查升级
 * 
 * 检查玩家是否可以升级。
 * 如果经验值足够，会自动升级。
 * 支持连续升级（一次获得大量经验可能升多级）。
 * 
 * 升级效果：
 * 1. 等级 +1
 * 2. 最大生命值 +10
 * 3. 生命值回满
 * 4. 最大灵能值 +5
 * 5. 灵能值回满
 * 6. 发出 levelUp 信号
 * 7. 发出 healthChanged 和 manaChanged 信号
 * 
 * 注意：
 * - 最大等级是 50 级
 * - 升级后生命值和灵能值会回满
 * - 连续升级时，每次升级都会发出信号
 * - 升级后剩余的经验值会保留
 */
void Player::checkLevelUp()
{
    // 计算当前等级升级所需经验
    int expNeeded = getExpForLevel(m_level);
    
    // 循环检查，支持连续升级
    while (m_experience >= expNeeded && m_level < 50) {
        // 扣除升级所需经验
        m_experience -= expNeeded;
        
        // 等级提升
        m_level++;
        
        // 升级奖励：生命值+10，灵能值+5
        m_maxHealth += 10.0f;
        m_currentHealth = m_maxHealth;  // 生命回满
        m_maxMana += 5.0f;
        m_currentMana = m_maxMana;      // 灵能回满
        
        qDebug() << "[Player] 玩家升级到" << m_level << "级！";
        qDebug() << "[Player] 最大生命:" << m_maxHealth 
                 << "最大灵能:" << m_maxMana;
        
        // 发出升级信号
        emit levelUp(m_level);
        emit healthChanged(m_currentHealth, m_maxHealth);
        emit manaChanged(m_currentMana, m_maxMana);
        
        // 计算下一级所需经验
        expNeeded = getExpForLevel(m_level);
    }
}

/**
 * @brief 获取升级所需经验
 * 
 * 计算指定等级升级所需的经验值。
 * 
 * 公式：所需经验 = 100 * 1.5^(等级-1)
 * 
 * 示例：
 * - 1级升2级：100 * 1.5^0 = 100
 * - 2级升3级：100 * 1.5^1 = 150
 * - 3级升4级：100 * 1.5^2 = 225
 * - 4级升5级：100 * 1.5^3 = 337
 * 
 * 注意：
 * - 经验值增长是指数级的
 * - 等级越高，升级所需经验越多
 * - 这是一个标准的RPG升级曲线
 * 
 * @param level 当前等级
 * @return 所需经验
 */
int Player::getExpForLevel(int level) const
{
    // 升级所需经验公式：100 * 1.5^(等级-1)
    return static_cast<int>(100 * std::pow(1.5, level - 1));
}

// ============================================================
// 生命值相关
// ============================================================

/**
 * @brief 设置最大生命值
 * 
 * 设置玩家的最大生命值。
 * 如果当前生命值超过新的最大生命值，会自动调整。
 * 
 * 执行以下操作：
 * 1. 设置最大生命值（最小为 1）
 * 2. 如果当前生命值超过最大值，调整为最大值
 * 3. 发出 healthChanged 信号
 * 
 * 注意：
 * - 最大生命值最小为 1，避免除零错误
 * - 当前生命值不会超过最大生命值
 * 
 * @param health 最大生命值
 */
void Player::setMaxHealth(float health)
{
    // 设置最大生命值，最小为 1
    m_maxHealth = std::max(1.0f, health);
    
    // 如果当前生命值超过最大值，调整为最大值
    if (m_currentHealth > m_maxHealth) {
        m_currentHealth = m_maxHealth;
    }
    
    // 发出生命值变化信号
    emit healthChanged(m_currentHealth, m_maxHealth);
}

/**
 * @brief 造成伤害
 * 
 * 对玩家造成伤害。
 * 玩家死亡后不会再受到伤害。
 * 伤害值必须大于 0。
 * 
 * 执行以下操作：
 * 1. 检查玩家是否死亡或伤害无效
 * 2. 减少当前生命值（最小为 0）
 * 3. 发出 healthChanged 信号
 * 4. 检查是否死亡
 *    - 如果死亡：设置死亡状态，发出 playerDied 信号
 *    - 如果未死亡：设置受伤状态
 * 
 * 注意：
 * - 伤害值必须大于 0，否则不做任何操作
 * - 玩家死亡后不会再受到伤害
 * - 受伤时会切换到 Hurt 状态
 * - 死亡时会切换到 Dead 状态并发出 playerDied 信号
 * 
 * @param damage 伤害值
 */
void Player::takeDamage(float damage)
{
    // 如果玩家已死亡或伤害无效，不做任何操作
    if (m_state == Dead || damage <= 0) {
        return;
    }

    // 减少当前生命值，最小为 0
    m_currentHealth = std::max(0.0f, m_currentHealth - damage);
    qDebug() << "[Player] 玩家受到" << damage << "点伤害，当前生命:" << m_currentHealth;
    
    // 发出生命值变化信号
    emit healthChanged(m_currentHealth, m_maxHealth);
    
    // 检查是否死亡
    if (m_currentHealth <= 0) {
        // 玩家死亡
        setState(Dead);
        emit playerDied();
        qDebug() << "[Player] 玩家死亡！";
    } else {
        // 玩家受伤
        setState(Hurt);
    }
}

/**
 * @brief 恢复生命
 * 
 * 恢复玩家的生命值。
 * 玩家死亡后不能恢复生命。
 * 恢复量必须大于 0。
 * 
 * 执行以下操作：
 * 1. 检查玩家是否死亡或恢复量无效
 * 2. 增加当前生命值（最大为最大生命值）
 * 3. 发出 healthChanged 信号
 * 
 * 注意：
 * - 恢复量必须大于 0，否则不做任何操作
 * - 玩家死亡后不能恢复生命
 * - 当前生命值不会超过最大生命值
 * 
 * @param amount 恢复量
 */
void Player::heal(float amount)
{
    // 如果玩家已死亡或恢复量无效，不做任何操作
    if (m_state == Dead || amount <= 0) {
        return;
    }

    // 增加当前生命值，最大为最大生命值
    m_currentHealth = std::min(m_maxHealth, m_currentHealth + amount);
    qDebug() << "[Player] 玩家恢复" << amount << "点生命，当前生命:" << m_currentHealth;
    
    // 发出生命值变化信号
    emit healthChanged(m_currentHealth, m_maxHealth);
}

// ============================================================
// 灵能值相关
// ============================================================

/**
 * @brief 设置最大灵能值
 * 
 * 设置玩家的最大灵能值。
 * 如果当前灵能值超过新的最大灵能值，会自动调整。
 * 
 * 执行以下操作：
 * 1. 设置最大灵能值（最小为 1）
 * 2. 如果当前灵能值超过最大值，调整为最大值
 * 3. 发出 manaChanged 信号
 * 
 * 注意：
 * - 最大灵能值最小为 1，避免除零错误
 * - 当前灵能值不会超过最大灵能值
 * 
 * @param mana 最大灵能值
 */
void Player::setMaxMana(float mana)
{
    // 设置最大灵能值，最小为 1
    m_maxMana = std::max(1.0f, mana);
    
    // 如果当前灵能值超过最大值，调整为最大值
    if (m_currentMana > m_maxMana) {
        m_currentMana = m_maxMana;
    }
    
    // 发出灵能值变化信号
    emit manaChanged(m_currentMana, m_maxMana);
}

/**
 * @brief 消耗灵能
 * 
 * 消耗玩家的灵能值。
 * 如果灵能不足，返回 false。
 * 
 * 执行以下操作：
 * 1. 检查灵能是否足够
 * 2. 如果不足，输出警告，返回 false
 * 3. 如果足够，减少灵能值
 * 4. 发出 manaChanged 信号
 * 5. 返回 true
 * 
 * 注意：
 * - 灵能不足时返回 false，不会消耗灵能
 * - 消耗成功后会发出 manaChanged 信号
 * 
 * @param amount 消耗量
 * @return true 消耗成功
 * @return false 灵能不足
 */
bool Player::consumeMana(float amount)
{
    // 检查灵能是否足够
    if (m_currentMana < amount) {
        qWarning() << "[Player] 灵能不足！当前:" << m_currentMana << "需要:" << amount;
        return false;
    }

    // 消耗灵能
    m_currentMana -= amount;
    qDebug() << "[Player] 消耗" << amount << "点灵能，剩余:" << m_currentMana;
    
    // 发出灵能值变化信号
    emit manaChanged(m_currentMana, m_maxMana);
    
    return true;
}

/**
 * @brief 恢复灵能
 * 
 * 恢复玩家的灵能值。
 * 恢复量必须大于 0。
 * 
 * 执行以下操作：
 * 1. 检查恢复量是否有效
 * 2. 增加当前灵能值（最大为最大灵能值）
 * 3. 发出 manaChanged 信号
 * 
 * 注意：
 * - 恢复量必须大于 0，否则不做任何操作
 * - 当前灵能值不会超过最大灵能值
 * 
 * @param amount 恢复量
 */
void Player::restoreMana(float amount)
{
    // 检查恢复量是否有效
    if (amount <= 0) {
        return;
    }

    // 增加当前灵能值，最大为最大灵能值
    m_currentMana = std::min(m_maxMana, m_currentMana + amount);
    qDebug() << "[Player] 恢复" << amount << "点灵能，当前:" << m_currentMana;
    
    // 发出灵能值变化信号
    emit manaChanged(m_currentMana, m_maxMana);
}

// ============================================================
// 位置相关
// ============================================================

/**
 * @brief 移动
 * 
 * 按增量移动玩家。
 * 
 * 注意：
 * - 这是直接按增量移动，不考虑碰撞检测
 * - 碰撞检测应该在更高层的系统中处理
 * 
 * @param dx X方向增量
 * @param dy Y方向增量
 */
void Player::move(float dx, float dy)
{
    m_position.setX(m_position.x() + dx);
    m_position.setY(m_position.y() + dy);
}

// ============================================================
// 状态相关
// ============================================================

/**
 * @brief 设置状态
 * 
 * 设置玩家的状态。
 * 如果状态没有变化，不做任何操作。
 * 
 * 状态变化时会发出 stateChanged 信号。
 * 
 * 注意：
 * - 如果状态没有变化，不做任何操作
 * - 状态变化时会发出 stateChanged 信号
 * 
 * @param state 状态
 */
void Player::setState(PlayerState state)
{
    // 如果状态没有变化，不做任何操作
    if (m_state == state) {
        return;
    }

    // 设置新状态
    m_state = state;
    qDebug() << "[Player] 玩家状态变化:" << m_state << "->" << state;
    
    // 发出状态变化信号
    emit stateChanged(state);
}

// ============================================================
// 稳定度相关
// ============================================================

/**
 * @brief 设置稳定度
 * 
 * 设置玩家的稳定度。
 * 稳定度会自动限制在 0-100 范围内。
 * 
 * 执行以下操作：
 * 1. 限制稳定度在有效范围内
 * 2. 设置稳定度
 * 3. 发出 stabilityChanged 信号
 * 
 * 注意：
 * - 稳定度范围是 0-100
 * - 稳定度影响玩家的能力和状态
 * - 稳定度变化时会发出 stabilityChanged 信号
 * 
 * @param stability 稳定度
 */
void Player::setStability(float stability)
{
    // 限制稳定度在 0-100 范围内
    m_stability = std::max(0.0f, std::min(m_maxStability, stability));
    
    // 发出稳定度变化信号
    emit stabilityChanged(m_stability);
}

/**
 * @brief 增加稳定度
 * 
 * 增加玩家的稳定度。
 * 这是一个便捷方法，相当于 setStability(getStability() + amount)。
 * 
 * @param amount 增加量
 */
void Player::addStability(float amount)
{
    setStability(m_stability + amount);
}

/**
 * @brief 减少稳定度
 * 
 * 减少玩家的稳定度。
 * 这是一个便捷方法，相当于 setStability(getStability() - amount)。
 * 
 * @param amount 减少量
 */
void Player::reduceStability(float amount)
{
    setStability(m_stability - amount);
}
