#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QCloseEvent>
#include "GameLoop.h"
#include <QGraphicsScene>
namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
protected:
    void keyPressEvent(QKeyEvent *event)override;
    void keyReleaseEvent(QKeyEvent *event)override;
    void mousePressEvent(QMouseEvent *event)override;
    void mouseReleaseEvent(QMouseEvent *event)override;
    void mouseMoveEvent(QMouseEvent *event)override;
    void closeEvent(QCloseEvent *event)override;
private slots:
    void onGameUpdate(float deltaTime);
    void onFpsChanged(float fps);
private:
    Ui::GameWindow *ui;
    GameLoop* m_gameLoop;

    void initGameLoop();
    void initScenes();
    void initViewSenes();
    void initViewSettings();
};

#endif // GAMEWINDOW_H
