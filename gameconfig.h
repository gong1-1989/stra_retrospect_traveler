#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <QObject>

class GameConfig : public QObject
{
    Q_OBJECT
public:
    explicit GameConfig(QObject *parent = nullptr);

signals:
};

#endif // GAMECONFIG_H
