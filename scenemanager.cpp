#include "scenemanager.h"
#include <QDebug>
SceneManager* SceneManager::s_instance=nullptr;

SceneManager* SceneManager::instance(){
    if(!s_instance) s_instance=new SceneManager();
    return s_instance;
}
SceneManager::SceneManager(QObject *parent)
    : QObject{parent}
    ,m_currentScene(nullptr)
    ,m_view(nullptr)
{}

SceneManager::~SceneManager(){
    m_scenes.clear();
}
void SceneManager::registerScene(BaseScene *scene){
    if(!scene){
        qWarning()<<"SceneManager::registerScene : 场景指针为空！";
        return ;
    }
    QString name=scene->name();
    if(m_scenes.contains(name)){
        qWarning()<<"SceneManager::registerScene : 场景 "<<name<<" 已存在，将被覆盖";
    }
    m_scenes.insert(name,scene);
    qDebug()<<"场景："<<name<<" 注册成功！";
}
void SceneManager::unregisterScene(const QString &name){
    if(!m_scenes.contains(name)){
        qWarning()<<"SceneManager::unregisterScene : 场景 "<<name<<" 不存在!";
        return;
    }
    if(m_currentScene&&m_currentScene->name()==name){
        m_currentScene->onExit();
        m_currentScene=nullptr;
    }
    m_scenes.remove(name);
    qDebug()<<"场景："<<name<<" 已注销！";
}

bool SceneManager::switchTo(const QString &name){
    if(!m_scenes.contains(name)){
        qWarning()<<"SceneManager::switchTo : 场景 "<<name<<" 不存在！";
        return false;
    }
    BaseScene *targetScene=m_scenes.value(name);
    if(m_currentScene==targetScene){
        qDebug()<<"SceneManager::switchTo : 已经在场景 "<<name<<" 中！";
        return true;
    }
    if(m_currentScene){
        qDebug()<<"离开场景："<<m_currentScene->name();
        m_currentScene->onExit();
    }
    if(m_view) m_view->setScene(m_currentScene);
    else qWarning()<<"SceneManager::switchTo : View未设置！";
    return true;
}
BaseScene* SceneManager::getScene(const QString &name) const{
    return m_scenes.value(name,nullptr);
}
void SceneManager::updateCurrentScene(float deltaTime){
    if(m_currentScene)m_currentScene->update(deltaTime);
}