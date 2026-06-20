#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <QObject>
#include <QHash>
#include <QPoint>
#include <Qt>
//===================================
/**
 * InputManager类：输入管理器（单例模式）
 * 功能：
 * 1.统一管理键盘、鼠标输入
 * 2.提供三种按键状态查询：
 *      -isPressed:当前是否按下
 *      -isJustPressed:这一帧是否刚按下
 *      -isJustReleased:这一帧是否刚松开
 * 3.记录鼠标位置和移动
 */
//===================================
class InputManager : public QObject
{
    Q_OBJECT
public:
    static InputManager* instance();
    bool isKeyPressde(Qt::Key key)const;
    bool isKeyJustPressed(Qt::Key key)const;
    bool isKeyJustReleased(Qt::Key key)const;
    bool isMouseButtonPressed(Qt::MouseButton button)const;
    QPoint mousePosition()const{return m_mousePos;}
    QPoint mouseDelta()const {return m_mouseDelta;}

    void onKeyPress(Qt::Key key);
    void onKeyRelease(Qt::Key key);
    void onMousePress(Qt::MouseButton button);
    void onMouseRelease(Qt::MouseButton button);
    void onMouseMove(const QPoint &pos);

    void endFrame();
private:
    explicit InputManager(QObject *parent = nullptr);
    ~InputManager();
    Q_DISABLE_COPY(InputManager)
    static InputManager* s_instance;
    QHash<Qt::Key,bool> m_keysPressed;
    QHash<Qt::Key,bool> m_keysJustPressed;
    QHash<Qt::Key,bool> m_keysJustReleased;
    QHash<Qt::MouseButton,bool> m_mouseButonsPressed;
    QPoint m_mousePos;
    QPoint m_mouseDelta;
    QPoint m_lastMousePos;
};

#endif // INPUTMANAGER_H
