/**
 * @file InputManager.cpp
 * @brief 输入管理器类实现
 * 
 * 本文件实现了输入管理器的所有功能，包括：
 * - 键盘输入状态跟踪（按下、释放、刚按下、刚释放）
 * - 鼠标输入状态跟踪（位置、移动、按钮）
 * - 输入事件处理和信号通知
 * - 帧同步状态更新
 * 
 * 输入管理器采用帧同步机制，每帧更新一次输入状态，
 * 确保游戏逻辑在同一帧内看到一致的输入状态。
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#include "InputManager.h"

#include <QDebug>
#include <QMutableMapIterator>

// ============================================================
// 构造函数与析构函数
// ============================================================

/**
 * @brief 构造函数
 * 
 * 创建输入管理器实例，但不进行任何初始化操作。
 * 所有的初始化工作都在 initialize() 中完成。
 * 
 * @param parent 父对象，用于 Qt 对象树内存管理
 */
InputManager::InputManager(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[InputManager] 创建输入管理器";
}

/**
 * @brief 析构函数
 * 
 * 销毁输入管理器，自动调用 shutdown() 清理所有状态。
 */
InputManager::~InputManager()
{
    qDebug() << "[InputManager] 销毁输入管理器";
    shutdown();
}

// ============================================================
// 生命周期管理
// ============================================================

/**
 * @brief 初始化输入管理器
 * 
 * 执行以下初始化操作：
 * 1. 检查是否已经初始化（防止重复初始化）
 * 2. 初始化鼠标位置和移动增量
 * 3. 初始化鼠标按钮状态
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
bool InputManager::initialize()
{
    // 防止重复初始化
    if (m_initialized) {
        qWarning() << "[InputManager] 输入管理器已经初始化，跳过重复初始化";
        return true;
    }

    qDebug() << "[InputManager] 初始化输入管理器...";

    // 1. 初始化鼠标位置
    m_mousePosition = QPoint(0, 0);
    m_mouseDelta = QPoint(0, 0);
    m_lastMousePosition = QPoint(0, 0);
    
    // 2. 初始化鼠标按钮状态
    m_mouseButtons[Qt::LeftButton] = false;
    m_mouseButtons[Qt::RightButton] = false;
    m_mouseButtons[Qt::MiddleButton] = false;

    // 初始化完成
    m_initialized = true;
    
    qDebug() << "[InputManager] 输入管理器初始化完成";
    return true;
}

/**
 * @brief 关闭输入管理器
 * 
 * 清理所有输入状态，包括：
 * - 清空按键状态映射
 * - 清空鼠标按钮状态
 * 
 * 此函数是幂等的，可以安全地多次调用。
 */
void InputManager::shutdown()
{
    // 如果没有初始化，直接返回
    if (!m_initialized) {
        return;
    }

    qDebug() << "[InputManager] 关闭输入管理器...";

    // 1. 清空按键状态
    qDebug() << "[InputManager] 清理按键状态，数量:" << m_keyStates.size();
    m_keyStates.clear();

    // 2. 清空鼠标按钮状态
    qDebug() << "[InputManager] 清理鼠标按钮状态，数量:" << m_mouseButtons.size();
    m_mouseButtons.clear();

    // 重置鼠标位置
    m_mousePosition = QPoint(0, 0);
    m_mouseDelta = QPoint(0, 0);
    m_lastMousePosition = QPoint(0, 0);

    // 重置状态
    m_initialized = false;
    
    qDebug() << "[InputManager] 输入管理器已关闭";
}

// ============================================================
// 帧更新
// ============================================================

/**
 * @brief 更新输入状态
 * 
 * 每帧调用一次，执行以下操作：
 * 1. 更新鼠标移动增量（当前位置 - 上帧位置）
 * 2. 更新按键状态：
 *    - KeyJustPressed -> KeyPressed（刚按下转为持续按下）
 *    - KeyJustReleased -> KeyReleased（刚释放转为持续释放）
 * 
 * 这样可以确保 isKeyJustPressed 和 isKeyJustReleased 只在
 * 事件发生的那一帧返回 true，下一帧自动转为普通状态。
 * 
 * @param deltaTime 时间增量（秒），当前未使用
 */
void InputManager::update(float deltaTime)
{
    Q_UNUSED(deltaTime);
    
    // 1. 更新鼠标移动增量
    m_mouseDelta = m_mousePosition - m_lastMousePosition;
    m_lastMousePosition = m_mousePosition;

    // 2. 更新按键状态（将瞬时状态转为持续状态）
    QMutableMapIterator<int, KeyState> it(m_keyStates);
    while (it.hasNext()) {
        it.next();
        
        if (it.value() == KeyJustPressed) {
            // 刚按下 -> 持续按下
            it.setValue(KeyPressed);
        } else if (it.value() == KeyJustReleased) {
            // 刚释放 -> 持续释放
            it.setValue(KeyReleased);
        }
    }
}

// ============================================================
// 键盘输入查询
// ============================================================

/**
 * @brief 检查按键是否按下
 * 
 * 如果按键处于 KeyPressed 或 KeyJustPressed 状态，返回 true。
 * 适合检测持续按住的情况（如角色移动）。
 * 
 * @param key 按键码（Qt::Key 枚举）
 * @return true 按键按下
 * @return false 按键释放
 */
bool InputManager::isKeyPressed(int key) const
{
    KeyState state = m_keyStates.value(key, KeyReleased);
    return (state == KeyPressed || state == KeyJustPressed);
}

/**
 * @brief 检查按键是否刚按下
 * 
 * 只在按键被按下的那一帧返回 true（KeyJustPressed 状态）。
 * 下一帧会自动转为 KeyPressed，此时返回 false。
 * 适合检测单次触发的情况（如跳跃、攻击、菜单确认）。
 * 
 * @param key 按键码（Qt::Key 枚举）
 * @return true 按键刚按下
 * @return false 按键不是刚按下
 */
bool InputManager::isKeyJustPressed(int key) const
{
    return m_keyStates.value(key, KeyReleased) == KeyJustPressed;
}

/**
 * @brief 检查按键是否刚释放
 * 
 * 只在按键被释放的那一帧返回 true（KeyJustReleased 状态）。
 * 下一帧会自动转为 KeyReleased，此时返回 false。
 * 适合检测按键释放的情况（如蓄力释放、取消操作）。
 * 
 * @param key 按键码（Qt::Key 枚举）
 * @return true 按键刚释放
 * @return false 按键不是刚释放
 */
bool InputManager::isKeyJustReleased(int key) const
{
    return m_keyStates.value(key, KeyReleased) == KeyJustReleased;
}

// ============================================================
// 鼠标输入查询
// ============================================================

/**
 * @brief 检查鼠标按钮是否按下
 * 
 * @param button 鼠标按钮（Qt::LeftButton, Qt::RightButton, Qt::MiddleButton）
 * @return true 按钮按下
 * @return false 按钮释放
 */
bool InputManager::isMouseButtonPressed(Qt::MouseButton button) const
{
    return m_mouseButtons.value(button, false);
}

// ============================================================
// 事件处理接口
// ============================================================

/**
 * @brief 处理按键按下事件
 * 
 * 由窗口的事件过滤器调用，更新按键状态。
 * 
 * 如果按键之前是释放状态，则设置为 KeyJustPressed（刚按下），
 * 并发出 keyPressed 信号。
 * 如果按键已经是按下状态，则保持不变（避免重复触发）。
 * 
 * 注意：Qt 的自动重复（auto-repeat）会持续发送按键按下事件，
 * 这里通过检查之前的状态来避免重复触发。
 * 
 * @param event 按键事件
 */
void InputManager::handleKeyPress(QKeyEvent* event)
{
    int key = event->key();
    
    // 只有之前是释放状态时，才设置为刚按下
    // 这样可以避免 Qt 自动重复导致的多次触发
    if (m_keyStates.value(key, KeyReleased) == KeyReleased) {
        m_keyStates[key] = KeyJustPressed;
        emit keyPressed(key);
    }
    // 否则保持按下状态（不重复触发）
}

/**
 * @brief 处理按键释放事件
 * 
 * 由窗口的事件过滤器调用，更新按键状态。
 * 
 * 将按键状态设置为 KeyJustReleased（刚释放），
 * 并发出 keyReleased 信号。
 * 
 * @param event 按键事件
 */
void InputManager::handleKeyRelease(QKeyEvent* event)
{
    int key = event->key();
    m_keyStates[key] = KeyJustReleased;
    emit keyReleased(key);
}

/**
 * @brief 处理鼠标移动事件
 * 
 * 由窗口的事件过滤器调用，更新鼠标位置。
 * 
 * 注意：鼠标移动增量（mouseDelta）在 update() 中计算，
 * 不是在这里直接计算。
 * 
 * @param event 鼠标事件
 */
void InputManager::handleMouseMove(QMouseEvent* event)
{
    m_mousePosition = event->pos();
}

/**
 * @brief 处理鼠标按下事件
 * 
 * 由窗口的事件过滤器调用，更新鼠标按钮状态。
 * 
 * 设置按钮为按下状态，并发出 mouseClicked 信号。
 * 
 * @param event 鼠标事件
 */
void InputManager::handleMousePress(QMouseEvent* event)
{
    Qt::MouseButton button = event->button();
    m_mouseButtons[button] = true;
    emit mouseClicked(button, event->pos());
}

/**
 * @brief 处理鼠标释放事件
 * 
 * 由窗口的事件过滤器调用，更新鼠标按钮状态。
 * 
 * 设置按钮为释放状态。
 * 
 * @param event 鼠标事件
 */
void InputManager::handleMouseRelease(QMouseEvent* event)
{
    Qt::MouseButton button = event->button();
    m_mouseButtons[button] = false;
}
