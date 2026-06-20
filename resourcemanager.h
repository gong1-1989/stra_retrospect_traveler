#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QObject>
#include <QPixmap>
#include <QSoundEffect>
#include <QHash>
#include <QString>
//====================================
/**
 * ResourceManager类：资源管理器（单例模式）
 * 功能：
 * 1.加载和缓存图片、音效等资源
 * 2.避免重复加载，提高性能
 * 3.同意管理资源生命周期
 */
//=====================================
class ResourceManager : public QObject
{
    Q_OBJECT
public:
    static ResourceManager* instance();
    QPixmap loadPixmap(const QString &path);//加载图片
    void preloadPixmap(const QString &path);//预加载图片
    void unloadPixmap(const QString &path);//卸载图片（释放内存）

    QSoundEffect* loadSound(const QString &path);//加载音效
    void preloadSound(const QString &path);//预加载音效
    void unloadSound(const QString &path);//卸载音效

    void clearCache();//清空所有缓存（切换大场景时）
    void preloadAll(const QStringList &paths);//批量预加载
private:
    explicit ResourceManager(QObject *parent = nullptr);
    ~ResourceManager();
    Q_DISABLE_COPY(ResourceManager)
    static ResourceManager* s_instance;
    QHash<QString,QPixmap> m_pixmapCache;
    QHash<QString,QSoundEffect*> m_soundCache;
    QString fullPath(const QString &path)const;
};

#endif // RESOURCEMANAGER_H
