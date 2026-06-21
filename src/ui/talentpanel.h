#ifndef TALENTPANEL_H
#define TALENTPANEL_H

#include <QObject>

class TalentPanel : public QObject
{
    Q_OBJECT
public:
    explicit TalentPanel(QObject *parent = nullptr);

signals:
};

#endif // TALENTPANEL_H
