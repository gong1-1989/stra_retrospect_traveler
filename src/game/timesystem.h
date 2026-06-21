#ifndef TIMESYSTEM_H
#define TIMESYSTEM_H

#include <QObject>

class TimeSystem : public QObject
{
    Q_OBJECT
public:
    explicit TimeSystem(QObject *parent = nullptr);

signals:
};

#endif // TIMESYSTEM_H
