/**
 * @file InputManager.h
 * @brief 输入管理器类
 * 
 * 输入管理器负责游戏中所有输入设备的管理，包括：
 * - 键盘输入（按键按下、释放、刚按下、刚释放）
 * - 鼠标输入（位置、移动、按钮）
 * - 输入状态的查询和事件通知
 * 
 * 主要特性：
 * 1. 按键状态跟踪：支持按下、释放、刚按下、刚释放四种状态
 * 2. 鼠标状态跟踪：位置、移动增量、按钮状态
 * 3. 事件通知：通过信号通知输入事件
 * 4. 帧同步：每帧更新输入状态，确保状态一致性
 * 
 * 使用示例：
 * @code
 * InputManager input;
 * input.initialize();
 * 
 * // 在游戏循环中
 * input.update(deltaTime);
 * if (input.isKeyJustPressed(Qt::Key_Space)) {
 *     // 跳跃
 * }
 * @endcode
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <QObject>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMap>
#include <QPoint>
#include <Qt>

/**
 * @brief 输入管理器类
 * 
 * 负责键盘、鼠标等输入设备的管理和状态跟踪。
 * 提供按键状态查询、鼠标位置获取、输入事件通知等功能。
 * 
 * 按键状态说明：
 * - KeyReleased: 按键处于释放状态
 * - KeyPressed: 按键处于按下状态（持续按住）
 * - KeyJustPressed: 按键在本帧刚被按下
 * - KeyJustReleased: 按键在本帧刚被释放
 * 
 * 注意：isKeyJustPressed 和 isKeyJustReleased 只在事件发生的那一帧返回 true，
 * 下一帧会自动转为 KeyPressed 或 KeyReleased。
 */
class InputManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 按键状态枚举
     * 
     * 定义按键的四种状态：
     * - KeyReleased: 释放状态
     * - KeyPressed: 按下状态（持续按住）
     * - KeyJustPressed: 刚按下（本帧触发）
     * - KeyJustReleased: 刚释放（本帧触发）
     */
    enum KeyState {
        KeyReleased = 0,     ///< 释放状态
        KeyPressed = 1,      ///< 按下状态（持续按住）
        KeyJustPressed = 2,  ///< 刚按下（本帧触发）
        KeyJustReleased = 3  ///< 刚释放（本帧触发）
    };

    /**
     * @brief 构造函数
     * 
     * 创建输入管理器实例，但不进行初始化。
     * 必须调用 initialize() 完成初始化后才能使用。
     * 
     * @param parent 父对象，用于 Qt 对象树内存管理
     */
    explicit InputManager(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     * 
     * 销毁输入管理器，自动调用 shutdown() 清理资源。
     */
    ~InputManager() override;

    // ============================================================
    // 生命周期管理
    // ============================================================
    
    /**
     * @brief 初始化输入管理器
     * 
     * 初始化鼠标位置、按钮状态等。
     * 
     * @return true 初始化成功
     * @return false 初始化失败
     */
    bool initialize();
    
    /**
     * @brief 关闭输入管理器
     * 
     * 清理所有输入状态，重置为初始状态。
     */
    void shutdown();

    // ============================================================
    // 帧更新
    // ============================================================
    
    /**
     * @brief 更新输入状态
     * 
     * 每帧调用一次，用于：
     * 1. 更新鼠标移动增量
     * 2. 将 JustPressed 转为 Pressed
     * 3. 将 JustReleased 转为 Released
     * 
     * 必须在每帧的输入事件处理之后、逻辑更新之前调用。
     * 
     * @param deltaTime 时间增量（秒），当前未使用
     */
    void update(float deltaTime);

    // ============================================================
    // 键盘输入查询
    // ============================================================
    
    /**
     * @brief 检查按键是否按下
     * 
     * 如果按键处于按下状态（包括刚按下），返回 true。
     * 适合检测持续按住的情况（如移动）。
     * 
     * @param key 按键码（Qt::Key 枚举）
     * @return true 按键按下
     * @return false 按键释放
     */
    bool isKeyPressed(int key) const;
    
    /**
     * @brief 检查按键是否刚按下
     * 
     * 只在按键被按下的那一帧返回 true，之后返回 false。
     * 适合检测单次触发的情况（如跳跃、攻击）。
     * 
     * @param key 按键码（Qt::Key 枚举）
     * @return true 按键刚按下
     * @return false 按键不是刚按下
     */
    bool isKeyJustPressed(int key) const;
    
    /**
     * @brief 检查按键是否刚释放
     * 
     * 只在按键被释放的那一帧返回 true，之后返回 false。
     * 适合检测按键释放的情况（如蓄力释放）。
     * 
     * @param key 按键码（Qt::Key 枚举）
     * @return true 按键刚释放
     * @return false 按键不是刚释放
     */
    bool isKeyJustReleased(int key) const;

    // ============================================================
    // 鼠标输入查询
    // ============================================================
    
    /**
     * @brief 获取鼠标当前位置
     * 
     * 返回鼠标相对于窗口的坐标。
     * 
     * @return QPoint 鼠标位置（x, y）
     */
    QPoint getMousePosition() const { return m_mousePosition; }
    
    /**
     * @brief 获取鼠标移动增量
     * 
     * 返回本帧鼠标移动的距离。
     * 
     * @return QPoint 鼠标移动增量（dx, dy）
     */
    QPoint getMouseDelta() const { return m_mouseDelta; }
    
    /**
     * @brief 检查鼠标按钮是否按下
     * 
     * @param button 鼠标按钮（Qt::LeftButton, Qt::RightButton, Qt::MiddleButton）
     * @return true 按钮按下
     * @return false 按钮释放
     */
    bool isMouseButtonPressed(Qt::MouseButton button) const;

    // ============================================================
    // 事件处理接口
    // ============================================================
    
    /**
     * @brief 处理按键按下事件
     * 
     * 由窗口的事件过滤器调用，更新按键状态。
     * 
     * @param event 按键事件
     */
    void handleKeyPress(QKeyEvent* event);
    
    /**
     * @brief 处理按键释放事件
     * 
     * 由窗口的事件过滤器调用，更新按键状态。
     * 
     * @param event 按键事件
     */
    void handleKeyRelease(QKeyEvent* event);
    
    /**
     * @brief 处理鼠标移动事件
     * 
     * 由窗口的事件过滤器调用，更新鼠标位置。
     * 
     * @param event 鼠标事件
     */
    void handleMouseMove(QMouseEvent* event);
    
    /**
     * @brief 处理鼠标按下事件
     * 
     * 由窗口的事件过滤器调用，更新鼠标按钮状态。
     * 
     * @param event 鼠标事件
     */
    void handleMousePress(QMouseEvent* event);
    
    /**
     * @brief 处理鼠标释放事件
     * 
     * 由窗口的事件过滤器调用，更新鼠标按钮状态。
     * 
     * @param event 鼠标事件
     */
    void handleMouseRelease(QMouseEvent* event);

signals:
    /**
     * @brief 按键按下信号
     * 
     * 当按键被按下时发出，携带按键码。
     * 
     * @param key 按键码（Qt::Key 枚举）
     */
    void keyPressed(int key);
    
    /**
     * @brief 按键释放信号
     * 
     * 当按键被释放时发出，携带按键码。
     * 
     * @param key 按键码（Qt::Key 枚举）
     */
    void keyReleased(int key);
    
    /**
     * @brief 鼠标点击信号
     * 
     * 当鼠标按钮被按下时发出，携带按钮和位置。
     * 
     * @param button 鼠标按钮
     * @param position 点击位置
     */
    void mouseClicked(Qt::MouseButton button, const QPoint& position);

private:
    // ============================================================
    // 成员变量
    // ============================================================
    
    // 键盘状态
    QMap<int, KeyState> m_keyStates;  ///< 按键状态映射（按键码 -> 状态）
    
    // 鼠标状态
    QPoint m_mousePosition;           ///< 鼠标当前位置
    QPoint m_mouseDelta;              ///< 鼠标移动增量（本帧）
    QPoint m_lastMousePosition;       ///< 上一帧鼠标位置
    QMap<Qt::MouseButton, bool> m_mouseButtons;  ///< 鼠标按钮状态
    
    // 状态
    bool m_initialized = false;  ///< 是否已初始化
};

#endif // INPUT_MANAGER_H
