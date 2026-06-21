#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include <QObject>

class SpriteAnimation : public QObject
{
    Q_OBJECT
public:
    explicit SpriteAnimation(QObject *parent = nullptr);

signals:
};

#endif // SPRITEANIMATION_H
