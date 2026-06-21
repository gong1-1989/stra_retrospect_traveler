#ifndef QUESTSYSTEM_H
#define QUESTSYSTEM_H

#include <QObject>

class QuestSystem : public QObject
{
    Q_OBJECT
public:
    explicit QuestSystem(QObject *parent = nullptr);

signals:
};

#endif // QUESTSYSTEM_H
