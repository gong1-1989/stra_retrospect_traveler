#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QObject>

class SceneManager : public QObject
{
    Q_OBJECT
public:
    explicit SceneManager(QObject *parent = nullptr);

signals:
};

#endif // SCENEMANAGER_H
