#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <QObject>

class RenderManager : public QObject
{
    Q_OBJECT
public:
    explicit RenderManager(QObject *parent = nullptr);

signals:
};

#endif // RENDERMANAGER_H
