#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>

class UIManager : public QObject
{
    Q_OBJECT
public:
    explicit UIManager(QObject *parent = nullptr);

signals:
};

#endif // UIMANAGER_H
