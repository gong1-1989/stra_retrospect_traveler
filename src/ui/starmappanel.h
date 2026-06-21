#ifndef STARMAPPANEL_H
#define STARMAPPANEL_H

#include <QObject>

class StarMapPanel : public QObject
{
    Q_OBJECT
public:
    explicit StarMapPanel(QObject *parent = nullptr);

signals:
};

#endif // STARMAPPANEL_H
