#ifndef ECOSYSTEMSYSTEM_H
#define ECOSYSTEMSYSTEM_H

#include <QObject>

class EcosystemSystem : public QObject
{
    Q_OBJECT
public:
    explicit EcosystemSystem(QObject *parent = nullptr);

signals:
};

#endif // ECOSYSTEMSYSTEM_H
