#ifndef BASESCENE_H
#define BASESCENE_H
#include <QGraphicsScene>
#include <QString>
//===============================================
/**
 * BaseScene 类：所有游戏场景的基类
 * 继承自QGraphicsScene（Qt的2D图形场景）
 */
//================================================
class BaseScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit BaseScene(const QString &name,QObject *parent=nullptr);
    virtual ~BaseScene();

    QString name()const{return m_name;}
    virtual void onEnter(){}
    virtual void onExit(){}
    virtual void update(float deltaTime){Q_UNUSED(deltaTime);}
    virtual void render(){}
protected:
    QString m_name;
};

#endif // BASESCENE_H
