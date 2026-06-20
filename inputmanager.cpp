#include "inputmanager.h"
#include <QDebug>
InputManager* InputManager::s_instance=nullptr;
InputManager* InputManager::instance(){
    if(!s_instance) s_instance=new InputManager();
    return s_instance;
}
InputManager::InputManager(QObject *parent)
    : QObject{parent}
    ,m_mousePos(0,0)
    ,m_mouseDelta(0,0)
    ,m_lastMousePos(0,0)
{
    qDebug()<<"输入管理器初始化完成！";
}

InputManager::~InputManager(){
    qDebug()<<"输入管理器已销毁！";
}

bool InputManager::isKeyPressde(Qt::Key key) const{
    return m_keysPressed.value(key,false);
}
bool InputManager::isKeyJustPressed(Qt::Key key) const{
    return m_keysJustPressed.value(key,false);
}
bool InputManager::isKeyJustReleased(Qt::Key key) const{
    return m_keysJustReleased.value(key,false);
}
bool InputManager::isMouseButtonPressed(Qt::MouseButton button) const{
    return m_mouseButonsPressed.value(button,false);
}
void InputManager::onKeyPress(Qt::Key key){
    if(!isKeyPressde(key)){
        m_keysJustPressed.insert(key,true);
    }
    m_keysPressed.insert(key,true);
}
void InputManager::onKeyRelease(Qt::Key key){
    if(isKeyPressde(key)) m_keysJustReleased.insert(key,true);
    m_keysPressed.insert(key,false);
}
void InputManager::onMousePress(Qt::MouseButton button){
    m_mouseButonsPressed.insert(button,true);
}
void InputManager::onMouseRelease(Qt::MouseButton button){
    m_mouseButonsPressed.insert(button,false);
}
void InputManager::onMouseMove(const QPoint &pos){
    m_mousePos=pos;
    m_mouseDelta=pos-m_lastMousePos;
}
void InputManager::endFrame(){
    m_keysPressed.clear();
    m_keysJustPressed.clear();
    m_keysJustReleased.clear();
    m_lastMousePos=m_mousePos;
    m_mouseDelta=QPoint(0,0);

}