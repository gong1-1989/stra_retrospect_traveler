#ifndef TILEMAP_H
#define TILEMAP_H

#include <QObject>

class TileMap : public QObject
{
    Q_OBJECT
public:
    explicit TileMap(QObject *parent = nullptr);

signals:
};

#endif // TILEMAP_H
