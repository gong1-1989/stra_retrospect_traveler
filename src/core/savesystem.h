#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include <QObject>

class SaveSystem : public QObject
{
    Q_OBJECT
public:
    explicit SaveSystem(QObject *parent = nullptr);

signals:
};

#endif // SAVESYSTEM_H
