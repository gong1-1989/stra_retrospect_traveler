#ifndef AISYSTEM_H
#define AISYSTEM_H

#include <QObject>

class AISystem : public QObject
{
    Q_OBJECT
public:
    explicit AISystem(QObject *parent = nullptr);

signals:
};

#endif // AISYSTEM_H
