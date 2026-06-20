#include "gameloop.h"

GameLoop::GameLoop(QObject *parent)
    : QObject{parent}
    ,m_timer(new QTimer(this))
    ,m_elapsedTimer(new QElapsedTimer())
{
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer,&QTimer::timeout,this,&GameLoop::onTick);
}
GameLoop::~GameLoop(){
    stop();
    delete m_elapsedTimer;
}
void GameLoop::start(int targetFps){
    if(m_isRunning) return;
    m_targetFps=targetFps;
    m_isRunning=true;
    m_isPaused=false;
    m_frameCount=0;
    m_fpsTimer=0.0f;
    int interval=1000/targetFps;
    m_timer->start(interval);
    m_elapsedTimer->start();
}
void GameLoop::stop(){
    if(!m_isRunning)return;
    m_timer->stop();
    m_isRunning=false;
    m_isPaused=false;
}
void GameLoop::pause(){
    if(!m_isRunning||m_isPaused)return;
    m_isPaused=true;
}
void GameLoop::resume(){
    if(!m_isRunning||!m_isPaused)return;
    m_isPaused=false;
    m_elapsedTimer->restart();
}
void GameLoop::onTick(){
    if(m_isPaused)return;
    qint64 elapsed=m_elapsedTimer->restart();
    m_deltaTime=elapsed/1000.0f;
    calculateFps(m_deltaTime);
    emit update(m_deltaTime);
    emit render();
}
void GameLoop::calculateFps(float deltaTime){
    m_frameCount++;
    m_fpsTimer+=deltaTime;
    if(m_fpsTimer>=1.0f){
        m_fps=m_frameCount/m_fpsTimer;
        m_frameCount=0;
        m_fpsTimer=0.0f;
        emit fpsChanged(m_fps);
    }
}