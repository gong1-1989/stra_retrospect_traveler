#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "SceneManager.h"
#include "BaseScene.h"
#include "InputManager.h"
#include <QDebug>

/**
 * 测试场景类
 * 用于验证场景系统是否正常工作
 */
class TestScene:public BaseScene{
public:
    TestScene():BaseScene("TestScene"){
        setSceneRect(0,0,800,600);
        addText("星溯旅人 - 测试场景\n\n第一阶段：项目初始化完成！");
    }
    void update(float deltaTime)override{
        Q_UNUSED(deltaTime);
    }
};

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
    ,m_gameLoop(new GameLoop(this))
{
    ui->setupUi(this);
    initViewSettings();
    initScenes();
    initGameLoop();
    qDebug()<<"游戏窗口初始化完成！\n窗口大小"<<size()<<"\n目标帧率：60 FPS";

}

GameWindow::~GameWindow()
{
    m_gameLoop->stop();
    delete ui;
    qDebug()<<"游戏窗口已销毁！";
}

void GameWindow::initViewSettings(){
    ui->gameView->setRenderHint(QPainter::Antialiasing);
    ui->gameView->setRenderHint(QPainter::SmoothPixmapTransform);
    ui->gameView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->gameView->setBackgroundBrush(QColor(10,10,26));
    qDebug()<<"视图设置初始化完成！";
}

void GameWindow::initScenes(){
    SceneManager *sceneMgr=SceneManager::instance();
    sceneMgr->setView(ui->gameView);
    TestScene *testScene=new TestScene();
    sceneMgr->registerScene(testScene);
    sceneMgr->switchTo("TestScene");
    qDebug()<<"场景系统初始化完成！\n当前场景：TestScene";
}
void GameWindow::initGameLoop(){
    connect(m_gameLoop,&GameLoop::update,this,&GameWindow::onGameUpdate);
    connect(m_gameLoop,&GameLoop::fpsChanged,this,&GameWindow::onFpsChanged);
    m_gameLoop->start(60);
    qDebug()<<"游戏循环启动，目标 60 FPS！";
}
void GameWindow::onGameUpdate(float deltaTime){
    SceneManager::instance()->updateCurrentScene(deltaTime);
    InputManager::instance()->endFrame();
}
void GameWindow::onFpsChanged(float fps){
    ui->fpsLabel->setText(QString("FPS：%1").arg(fps,0,'f',1));
}
void GameWindow::keyPressEvent(QKeyEvent *event){
    InputManager::instance()->onKeyPress(static_cast<Qt::Key>(event->key()));
    QMainWindow::keyPressEvent(event);
}
void GameWindow::keyReleaseEvent(QKeyEvent *event){
    InputManager::instance()->onKeyRelease(static_cast<Qt::Key>(event->key()));
    QMainWindow::keyReleaseEvent(event);
}
void GameWindow::mousePressEvent(QMouseEvent *event){
    InputManager::instance()->onMousePress(event->button());
    QMainWindow::mousePressEvent(event);
}
void GameWindow::mouseReleaseEvent(QMouseEvent *event){
    InputManager::instance()->onMouseRelease(event->button());
    QMainWindow::mouseReleaseEvent(event);
}
void GameWindow::mouseMoveEvent(QMouseEvent *event){
    InputManager::instance()->onMouseMove(event->pos());
    QMainWindow::mouseMoveEvent(event);
}
void GameWindow::closeEvent(QCloseEvent *event){
    qDebug()<<"窗口正在关闭...";
    m_gameLoop->stop();
    event->accept();
}