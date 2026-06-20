#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QObject>
#include <QHash>
#include <QString>
#include "BaseScene.h"
#include <QGraphicsView>
//===========================
/**
 * SceneManager类：场景管理器（单例模式）
 * 功能：
 * 1.注册和管理所有的游戏场景
 * 2.处理场景切换
 * 3.更新当前场景
 */
//==============================
class SceneManager : public QObject
{
    Q_OBJECT
public:
    static SceneManager* instance();
    void registerScene(BaseScene *scene);
    void unregisterScene(const QString &name);
    bool switchTo(const QString &name);
    BaseScene* currentScene()const{return m_currentScene;}
    BaseScene* getScene(const QString &name)const;
    void setView(QGraphicsView* view){m_view=view;}
    QGraphicsView* view()const{ return m_view; }
    void updateCurrentScene(float deltaTime);
private:
    explicit SceneManager(QObject *parent = nullptr);
    ~SceneManager();
    Q_DISABLE_COPY(SceneManager)
    static SceneManager* s_instance;
    QHash<QString,BaseScene*> m_scenes;
    BaseScene* m_currentScene=nullptr;
    QGraphicsView* m_view=nullptr;
};

#endif // SCENEMANAGER_H
