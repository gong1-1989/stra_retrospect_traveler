#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QObject>

class ResourceManager : public QObject
{
    Q_OBJECT
public:
    explicit ResourceManager(QObject *parent = nullptr);

signals:
};

#endif // RESOURCEMANAGER_H
