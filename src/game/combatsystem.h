#ifndef COMBATSYSTEM_H
#define COMBATSYSTEM_H

#include <QObject>

class CombatSystem : public QObject
{
    Q_OBJECT
public:
    explicit CombatSystem(QObject *parent = nullptr);

signals:
};

#endif // COMBATSYSTEM_H
