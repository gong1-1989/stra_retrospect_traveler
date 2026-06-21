#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <QObject>

class EventManager : public QObject
{
    Q_OBJECT
public:
    explicit EventManager(QObject *parent = nullptr);

signals:
};

#endif // EVENTMANAGER_H
