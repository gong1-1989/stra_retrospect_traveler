#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>

class GameEngine : public QObject
{
    Q_OBJECT
public:
    explicit GameEngine(QObject *parent = nullptr);

signals:
};

#endif // GAMEENGINE_H
