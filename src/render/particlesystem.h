#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <QObject>

class ParticleSystem : public QObject
{
    Q_OBJECT
public:
    explicit ParticleSystem(QObject *parent = nullptr);

signals:
};

#endif // PARTICLESYSTEM_H
