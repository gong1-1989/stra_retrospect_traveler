#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include <QObject>

class CameraSystem : public QObject
{
    Q_OBJECT
public:
    explicit CameraSystem(QObject *parent = nullptr);

signals:
};

#endif // CAMERASYSTEM_H
