#include "resourcemanager.h"
#include <QDebug>
#include <QFileInfo>
ResourceManager* ResourceManager::s_instance=nullptr;
ResourceManager* ResourceManager::instance(){
    if(!s_instance) s_instance=new ResourceManager();
    return s_instance;
}
ResourceManager::ResourceManager(QObject *parent)
    : QObject{parent}
{
    qDebug()<<"资源管理器初始化完成！";
}
ResourceManager::~ResourceManager(){
    qDeleteAll(m_soundCache);
    m_pixmapCache.clear();
    qDebug()<<"资源管理器已销毁";
}

QPixmap ResourceManager::loadPixmap(const QString &path){
    if(m_pixmapCache.contains(path))return m_pixmapCache.value(path);
    QString fullPath=this->fullPath(path);
    QPixmap pixmap(fullPath);
    if(pixmap.isNull()){
        qWarning()<<"ResourceManager::loadPixmap : 加载图片 "<<fullPath<<" 失败!";
        return QPixmap();
    }
    m_pixmapCache.insert(path,pixmap);
    qDebug()<<"图片加载成功："<<path<<"("<<pixmap.width()<<" × "<<pixmap.height()<<")";
    return pixmap;
}
void ResourceManager::preloadPixmap(const QString &path){
    if(m_pixmapCache.contains(path)) return;
    loadPixmap(path);
}
void ResourceManager::unloadPixmap(const QString &path){
    if(m_pixmapCache.contains(path)){
        m_pixmapCache.remove(path);
        qDebug()<<"图片已卸载："<<path;
    }
}

QSoundEffect* ResourceManager::loadSound(const QString &path){
    if(m_soundCache.contains(path))return m_soundCache.value(path);
    QString fullPath=this->fullPath(path);
    QSoundEffect *sound=new QSoundEffect(this);
    sound->setSource(QUrl::fromLocalFile(fullPath));
    if(!sound->isLoaded()) qDebug()<<"音效正在加载...:"<<path;
    m_soundCache.insert(path,sound);
    qDebug()<<"音效加载成功："<<path;
    return sound;
}

void ResourceManager::preloadSound(const QString &path){
    if(m_soundCache.contains(path))return;
    loadSound(path);
}
void ResourceManager::unloadSound(const QString &path){
    if(m_soundCache.contains(path)){
        QSoundEffect *sound=m_soundCache.take(path);
        delete sound;
        qDebug()<<"音效已卸载："<<path;
    }
}
void ResourceManager::clearCache(){
    m_pixmapCache.clear();
    qDeleteAll(m_soundCache);
    m_soundCache.clear();
    qDebug()<<"所有资源缓存已清空！";
}

void ResourceManager::preloadAll(const QStringList &paths){
    qDebug()<<"开始批量预加载"<<paths.size()<<"个资源...";
    for(const QString &path:paths){
        if(path.endsWith(".png",Qt::CaseInsensitive)
            ||path.endsWith(".jpg",Qt::CaseInsensitive)
            ||path.endsWith(".svg",Qt::CaseInsensitive)
            ||path.endsWith(".bmp",Qt::CaseInsensitive)) preloadSound(path);
        else if(path.endsWith(".wav",Qt::CaseInsensitive)) preloadPixmap(path);
        else qWarning()<<"ResourceManager::preloadAll: 未知资源类型："<<path;
    }
    qDebug()<<"批量预加载完成！";
}
QString ResourceManager::fullPath(const QString &path) const{
    if(path.startsWith(":"))return path;
    return ":/assets/"+path;
}