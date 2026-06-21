#ifndef INVENTORYSYSTEM_H
#define INVENTORYSYSTEM_H

#include <QObject>

class InventorySystem : public QObject
{
    Q_OBJECT
public:
    explicit InventorySystem(QObject *parent = nullptr);

signals:
};

#endif // INVENTORYSYSTEM_H
