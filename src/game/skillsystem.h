#ifndef SKILLSYSTEM_H
#define SKILLSYSTEM_H

#include <QObject>

class SkillSystem : public QObject
{
    Q_OBJECT
public:
    explicit SkillSystem(QObject *parent = nullptr);

signals:
};

#endif // SKILLSYSTEM_H
