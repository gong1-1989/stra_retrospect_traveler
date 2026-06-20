#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

class GameLoop : public QObject
{
    Q_OBJECT
public:
    explicit GameLoop(QObject *parent = nullptr);
    ~GameLoop();
    void start(int targetFps=60);   //启动游戏循环，默认60FPS
    void stop();                    //停止游戏循环
    void pause();                   //暂停游戏
    void resume();                  //恢复游戏
    bool isRunning()const{return m_isRunning;}
    bool isPause()const {return m_isPaused;}
    float deltaTime()const{return m_deltaTime;}
    float fps() const{return m_fps;}
signals:
    void update(float deltaTime);
    void render();
    void fpsChanged(float fps);
private slots:
    void onTick();
private:
    QTimer* m_timer;
    QElapsedTimer* m_elapsedTimer;
    bool m_isRunning=false;
    bool m_isPaused=false;
    float m_deltaTime=0.0f;
    float m_fps=0.0f;
    int m_targetFps=60;
    int m_frameCount=0;
    float m_fpsTimer=0.0f;
    void calculateFps(float deltaTime);
};

#endif // GAMELOOP_H
