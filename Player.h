/**
 * @file Player.h
 * @brief 玩家类
 * 
 * 本文件定义了玩家类，管理玩家的属性、状态和行为。
 * 玩家类是游戏的核心角色类，包含了玩家的所有属性和行为。
 * 
 * 主要特性：
 * 1. 属性系统：生命值、灵能值、经验值、等级、稳定度等
 * 2. 状态系统：空闲、行走、奔跑、跳跃、攻击、受伤、死亡等状态
 * 3. 成长系统：经验值、等级提升、属性成长
 * 4. 位置系统：位置、移动速度、移动控制
 * 5. 信号通知：属性变化时发出信号，便于UI更新
 * 
 * 玩家属性：
 * - 基本信息：名称、等级、经验值
 * - 生命值：最大生命值、当前生命值
 * - 灵能值：最大灵能值、当前灵能值
 * - 位置：当前位置、移动速度
 * - 状态：当前状态
 * - 稳定度：稳定度、最大稳定度
 * 
 * 玩家状态：
 * - Idle：空闲
 * - Walking：行走
 * - Running：奔跑
 * - Jumping：跳跃
 * - Attacking：攻击
 * - Hurt：受伤
 * - Dead：死亡
 * 
 * 成长系统：
 * - 经验值获取：击败敌人、完成任务等获得经验值
 * - 等级提升：经验值达到要求后升级
 * - 属性成长：升级后生命值、灵能值等属性提升
 * - 升级公式：所需经验 = 100 * 1.5^(等级-1)
 * 
 * 使用示例：
 * @code
 * Player player;
 * player.initialize();
 * 
 * // 设置玩家名称
 * player.setName("星溯旅人");
 * 
 * // 造成伤害
 * player.takeDamage(20.0f);
 * 
 * // 恢复生命
 * player.heal(10.0f);
 * 
 * // 消耗灵能
 * if (player.consumeMana(10.0f)) {
 *     // 释放技能
 * }
 * 
 * // 增加经验值
 * player.addExperience(100);
 * 
 * // 移动玩家
 * player.move(10.0f, 5.0f);
 * 
 * // 获取玩家状态
 * if (player.isAlive()) {
 *     qDebug() << "玩家存活，生命值:" << player.getCurrentHealth();
 * }
 * @endcode
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QPointF>
#include <QString>

/**
 * @brief 玩家类
 * 
 * 管理玩家的属性、状态和行为。
 * 玩家类是游戏的核心角色类，包含了玩家的所有属性和行为。
 * 
 * 架构说明：
 * - 玩家类继承 QObject，支持信号槽机制
 * - 属性变化时会发出相应信号，便于UI更新
 * - 玩家状态通过状态机管理
 * - 经验值和等级系统支持自动升级
 * 
 * 属性系统：
 * - 基本属性：名称、等级、经验值
 * - 生命属性：最大生命值、当前生命值
 * - 灵能属性：最大灵能值、当前灵能值
 * - 位置属性：当前位置、移动速度
 * - 稳定属性：稳定度、最大稳定度
 * 
 * 状态系统：
 * - Idle：空闲状态，玩家静止
 * - Walking：行走状态，玩家慢速移动
 * - Running：奔跑状态，玩家快速移动
 * - Jumping：跳跃状态，玩家跳跃中
 * - Attacking：攻击状态，玩家攻击中
 * - Hurt：受伤状态，玩家受伤
 * - Dead：死亡状态，玩家死亡
 * 
 * 成长系统：
 * - 经验值获取：击败敌人、完成任务等获得经验值
 * - 等级提升：经验值达到要求后自动升级
 * - 属性成长：升级后生命值+10，灵能值+5
 * - 生命和灵能会在升级时回满
 * 
 * 自然恢复：
 * - 灵能自然恢复：每秒恢复 2 点灵能
 * - 稳定度自然恢复：每秒恢复 1 点稳定度
 * 
 * 注意：
 * - 玩家死亡后（状态为 Dead），update() 不会再更新
 * - 生命值和灵能值会自动限制在有效范围内
 * - 稳定度范围是 0-100
 * - 最大等级是 50 级
 * 
 * 线程安全：
 * - 目前玩家类不是线程安全的
 * - 所有操作应该在主线程中执行
 * - 后续可以添加线程安全支持
 */
class Player : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 玩家状态枚举
     * 
     * 定义玩家的所有可能状态。
     * 玩家状态通过状态机管理，状态变化时发出 stateChanged 信号。
     */
    enum PlayerState {
        Idle = 0,       ///< 空闲
        Walking = 1,    ///< 行走
        Running = 2,    ///< 奔跑
        Jumping = 3,    ///< 跳跃
        Attacking = 4,  ///< 攻击
        Hurt = 5,       ///< 受伤
        Dead = 6        ///< 死亡
    };

    /**
     * @brief 构造函数
     * 
     * 创建玩家实例，但不进行初始化。
     * 必须调用 initialize() 完成初始化后才能使用。
     * 
     * @param parent 父对象，用于 Qt 对象树内存管理
     */
    explicit Player(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     * 
     * 销毁玩家，自动调用 shutdown() 清理资源。
     */
    ~Player() override;

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
     * 
     * @return true 初始化成功
     * @return false 初始化失败
     */
    bool initialize();
    
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
    void shutdown();
    
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
     * @param deltaTime 时间增量（秒）
     */
    void update(float deltaTime);

    // ============================================================
    // 基本属性
    // ============================================================
    
    /**
     * @brief 获取玩家名称
     * @return 名称
     */
    QString getName() const { return m_name; }
    
    /**
     * @brief 设置玩家名称
     * @param name 名称
     */
    void setName(const QString& name) { m_name = name; }
    
    /**
     * @brief 获取等级
     * @return 等级
     */
    int getLevel() const { return m_level; }
    
    /**
     * @brief 设置等级
     * 
     * 设置玩家等级。
     * 注意：这不会触发升级效果，只是直接设置等级。
     * 正常升级应该通过 addExperience() 实现。
     * 
     * @param level 等级
     */
    void setLevel(int level) { m_level = level; }
    
    /**
     * @brief 获取经验值
     * @return 经验值
     */
    int getExperience() const { return m_experience; }
    
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
     * @param exp 经验值
     */
    void addExperience(int exp);

    // ============================================================
    // 生命值相关
    // ============================================================
    
    /**
     * @brief 获取最大生命值
     * @return 最大生命值
     */
    float getMaxHealth() const { return m_maxHealth; }
    
    /**
     * @brief 获取当前生命值
     * @return 当前生命值
     */
    float getCurrentHealth() const { return m_currentHealth; }
    
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
     * @param health 最大生命值
     */
    void setMaxHealth(float health);
    
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
     * @param damage 伤害值
     */
    void takeDamage(float damage);
    
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
     * @param amount 恢复量
     */
    void heal(float amount);

    // ============================================================
    // 灵能值相关
    // ============================================================
    
    /**
     * @brief 获取最大灵能值
     * @return 最大灵能值
     */
    float getMaxMana() const { return m_maxMana; }
    
    /**
     * @brief 获取当前灵能值
     * @return 当前灵能值
     */
    float getCurrentMana() const { return m_currentMana; }
    
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
     * @param mana 最大灵能值
     */
    void setMaxMana(float mana);
    
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
     * @param amount 消耗量
     * @return true 消耗成功
     * @return false 灵能不足
     */
    bool consumeMana(float amount);
    
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
     * @param amount 恢复量
     */
    void restoreMana(float amount);

    // ============================================================
    // 位置相关
    // ============================================================
    
    /**
     * @brief 获取位置
     * @return 位置
     */
    QPointF getPosition() const { return m_position; }
    
    /**
     * @brief 设置位置
     * 
     * 直接设置玩家的位置。
     * 
     * @param pos 位置
     */
    void setPosition(const QPointF& pos) { m_position = pos; }
    
    /**
     * @brief 移动
     * 
     * 按增量移动玩家。
     * 
     * @param dx X方向增量
     * @param dy Y方向增量
     */
    void move(float dx, float dy);
    
    /**
     * @brief 获取移动速度
     * @return 速度
     */
    float getMoveSpeed() const { return m_moveSpeed; }
    
    /**
     * @brief 设置移动速度
     * @param speed 速度
     */
    void setMoveSpeed(float speed) { m_moveSpeed = speed; }

    // ============================================================
    // 状态相关
    // ============================================================
    
    /**
     * @brief 获取当前状态
     * @return 状态
     */
    PlayerState getState() const { return m_state; }
    
    /**
     * @brief 设置状态
     * 
     * 设置玩家的状态。
     * 如果状态没有变化，不做任何操作。
     * 
     * 状态变化时会发出 stateChanged 信号。
     * 
     * @param state 状态
     */
    void setState(PlayerState state);
    
    /**
     * @brief 是否存活
     * 
     * 检查玩家是否存活。
     * 玩家状态不是 Dead 就算存活。
     * 
     * @return true 存活
     * @return false 死亡
     */
    bool isAlive() const { return m_state != Dead; }

    // ============================================================
    // 稳定度相关
    // ============================================================
    
    /**
     * @brief 获取稳定度
     * 
     * 稳定度范围是 0-100。
     * 稳定度影响玩家的能力和状态。
     * 
     * @return 稳定度（0-100）
     */
    float getStability() const { return m_stability; }
    
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
     * @param stability 稳定度
     */
    void setStability(float stability);
    
    /**
     * @brief 增加稳定度
     * 
     * 增加玩家的稳定度。
     * 这是一个便捷方法，相当于 setStability(getStability() + amount)。
     * 
     * @param amount 增加量
     */
    void addStability(float amount);
    
    /**
     * @brief 减少稳定度
     * 
     * 减少玩家的稳定度。
     * 这是一个便捷方法，相当于 setStability(getStability() - amount)。
     * 
     * @param amount 减少量
     */
    void reduceStability(float amount);

signals:
    /**
     * @brief 生命值变化信号
     * 
     * 当生命值变化时发出，携带当前生命值和最大生命值。
     * 可以连接此信号来更新UI。
     * 
     * @param current 当前生命值
     * @param max 最大生命值
     */
    void healthChanged(float current, float max);
    
    /**
     * @brief 灵能值变化信号
     * 
     * 当灵能值变化时发出，携带当前灵能值和最大灵能值。
     * 可以连接此信号来更新UI。
     * 
     * @param current 当前灵能值
     * @param max 最大灵能值
     */
    void manaChanged(float current, float max);
    
    /**
     * @brief 经验值变化信号
     * 
     * 当经验值变化时发出，携带当前经验值和等级。
     * 可以连接此信号来更新UI。
     * 
     * @param current 当前经验值
     * @param level 当前等级
     */
    void experienceChanged(int current, int level);
    
    /**
     * @brief 等级提升信号
     * 
     * 当玩家升级时发出，携带新等级。
     * 可以连接此信号来播放升级特效或显示升级提示。
     * 
     * @param newLevel 新等级
     */
    void levelUp(int newLevel);
    
    /**
     * @brief 玩家死亡信号
     * 
     * 当玩家死亡时发出。
     * 可以连接此信号来处理死亡逻辑。
     */
    void playerDied();
    
    /**
     * @brief 状态变化信号
     * 
     * 当玩家状态变化时发出，携带新状态。
     * 可以连接此信号来更新动画或状态显示。
     * 
     * @param newState 新状态
     */
    void stateChanged(PlayerState newState);
    
    /**
     * @brief 稳定度变化信号
     * 
     * 当稳定度变化时发出，携带当前稳定度。
     * 可以连接此信号来更新UI。
     * 
     * @param stability 稳定度
     */
    void stabilityChanged(float stability);

private:
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
     */
    void checkLevelUp();
    
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
     * 
     * @param level 当前等级
     * @return 所需经验
     */
    int getExpForLevel(int level) const;

    // ============================================================
    // 成员变量
    // ============================================================
    
    // 基本信息
    QString m_name = "旅者";  ///< 玩家名称
    int m_level = 1;          ///< 等级
    int m_experience = 0;     ///< 经验值
    
    // 生命值
    float m_maxHealth = 100.0f;      ///< 最大生命值
    float m_currentHealth = 100.0f;  ///< 当前生命值
    
    // 灵能值
    float m_maxMana = 50.0f;      ///< 最大灵能值
    float m_currentMana = 50.0f;  ///< 当前灵能值
    
    // 位置
    QPointF m_position = QPointF(0, 0);  ///< 当前位置
    float m_moveSpeed = 200.0f;          ///< 移动速度（像素/秒）
    
    // 状态
    PlayerState m_state = Idle;  ///< 当前状态
    
    // 稳定度
    float m_stability = 100.0f;     ///< 稳定度
    float m_maxStability = 100.0f;  ///< 最大稳定度
    
    // 初始化状态
    bool m_initialized = false;  ///< 是否已初始化
};

#endif // PLAYER_H
