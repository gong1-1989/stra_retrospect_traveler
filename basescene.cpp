#include "basescene.h"

BaseScene::BaseScene(const QString &name, QObject *parent)
    :QGraphicsScene(parent)
    ,m_name(name)
{

}
BaseScene::~BaseScene(){

}