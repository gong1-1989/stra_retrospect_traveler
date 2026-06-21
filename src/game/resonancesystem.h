#ifndef RESONANCESYSTEM_H
#define RESONANCESYSTEM_H

#include <QObject>

class ResonanceSystem : public QObject
{
    Q_OBJECT
public:
    explicit ResonanceSystem(QObject *parent = nullptr);

signals:
};

#endif // RESONANCESYSTEM_H
