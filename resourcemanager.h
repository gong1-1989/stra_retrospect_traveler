/**
 * @file ResourceManager.h
 * @brief 资源管理器类
 * 
 * 资源管理器负责游戏中所有资源的加载、缓存和管理，包括：
 * - 图片资源（QPixmap）
 * - 音效资源（QSoundEffect）
 * - 字体资源（QFont）
 * 
 * 主要特性：
 * 1. 资源缓存：已加载的资源会被缓存，避免重复加载
 * 2. 自动垃圾回收：可卸载未使用的资源释放内存
 * 3. 支持 Qt 资源系统：支持从 qrc 资源文件加载资源
 * 4. 路径管理：统一管理资源基础路径
 * 
 * 使用示例：
 * @code
 * ResourceManager manager;
 * manager.initialize();
 * QPixmap player = manager.loadPixmap("player.png");
 * @endcode
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <QObject>
#include <QPixmap>
#include <QSoundEffect>
#include <QFont>
#include <QMap>
#include <QString>

/**
 * @brief 资源管理器类
 * 
 * 负责游戏资源的加载、缓存和管理。
 * 采用缓存机制避免重复加载相同资源，提高性能。
 * 支持从文件系统和 Qt 资源系统加载资源。
 */
class ResourceManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * 
     * 创建资源管理器实例，但不进行初始化。
     * 必须调用 initialize() 完成初始化后才能使用。
     * 
     * @param parent 父对象，用于 Qt 对象树内存管理
     */
    explicit ResourceManager(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     * 
     * 销毁资源管理器，自动调用 shutdown() 释放所有缓存资源。
     */
    ~ResourceManager() override;

    // ============================================================
    // 生命周期管理
    // ============================================================
    
    /**
     * @brief 初始化资源管理器
     * 
     * 检查资源目录是否存在，如果不存在则创建。
     * 
     * @return true 初始化成功
     * @return false 初始化失败
     */
    bool initialize();
    
    /**
     * @brief 关闭资源管理器
     * 
     * 释放所有缓存的资源，清理内存。
     */
    void shutdown();

    // ============================================================
    // 资源加载接口
    // ============================================================
    
    /**
     * @brief 加载图片资源
     * 
     * 从缓存或文件加载图片。如果资源已在缓存中，直接返回缓存的图片；
     * 否则从文件系统或 Qt 资源系统加载并缓存。
     * 
     * 支持的格式：PNG、JPG、BMP、SVG 等 Qt 支持的图片格式
     * 
     * @param path 资源相对路径（相对于资源基础路径）
     * @return QPixmap 加载的图片，失败时返回占位图
     */
    QPixmap loadPixmap(const QString& path);
    
    /**
     * @brief 加载音效资源
     * 
     * 从缓存或文件加载音效。如果资源已在缓存中，直接返回缓存的音效；
     * 否则从文件系统加载并缓存。
     * 
     * 注意：QSoundEffect 适合播放短音效，不适合播放长音乐。
     * 背景音乐应该使用 QMediaPlayer。
     * 
     * @param path 资源相对路径（相对于资源基础路径）
     * @return QSoundEffect* 音效指针，失败时返回 nullptr
     */
    QSoundEffect* loadSound(const QString& path);
    
    /**
     * @brief 加载字体资源
     * 
     * 从缓存或文件加载字体。如果资源已在缓存中，直接返回缓存的字体；
     * 否则从文件系统加载并缓存。
     * 
     * 缓存键包含字号，不同字号的字体会分别缓存。
     * 
     * @param path 资源相对路径（相对于资源基础路径）
     * @param pointSize 字体大小（磅值）
     * @return QFont 加载的字体，失败时返回默认字体
     */
    QFont loadFont(const QString& path, int pointSize = 12);

    // ============================================================
    // 缓存管理
    // ============================================================
    
    /**
     * @brief 垃圾回收
     * 
     * 卸载未使用的资源，释放内存。
     * 可以在场景切换等时机调用。
     * 
     * 注意：当前实现为占位，后续可实现基于引用计数或
     * LRU（最近最少使用）策略的垃圾回收。
     */
    void collectGarbage();
    
    /**
     * @brief 清空所有缓存
     * 
     * 强制清空所有资源缓存，释放所有内存。
     * 谨慎使用，可能导致后续资源重新加载。
     */
    void clearCache();

    // ============================================================
    // 路径管理
    // ============================================================
    
    /**
     * @brief 获取资源基础路径
     * @return 资源基础路径字符串
     */
    QString getResourcePath() const { return m_resourcePath; }
    
    /**
     * @brief 设置资源基础路径
     * @param path 新的资源基础路径
     */
    void setResourcePath(const QString& path) { m_resourcePath = path; }
    
    /**
     * @brief 检查是否使用 Qt 资源系统
     * @return true 表示使用 qrc 资源
     */
    bool isUsingQrc() const { return m_useQrc; }
    
    /**
     * @brief 设置是否使用 Qt 资源系统
     * @param useQrc true 使用 qrc，false 使用文件系统
     */
    void setUseQrc(bool useQrc) { m_useQrc = useQrc; }

private:
    /**
     * @brief 获取完整资源路径
     * 
     * 根据资源基础路径和相对路径拼接完整路径。
     * 如果使用 Qt 资源系统，则返回 qrc 路径（":/path"）。
     * 
     * @param relativePath 相对路径
     * @return 完整路径字符串
     */
    QString getFullPath(const QString& relativePath) const;

    // ============================================================
    // 成员变量
    // ============================================================
    
    // 资源缓存
    QMap<QString, QPixmap> m_pixmapCache;       ///< 图片缓存（路径 -> 图片）
    QMap<QString, QSoundEffect*> m_soundCache;  ///< 音效缓存（路径 -> 音效）
    QMap<QString, QFont> m_fontCache;           ///< 字体缓存（缓存键 -> 字体）
    
    // 路径设置
    QString m_resourcePath = "assets";  ///< 资源基础路径
    bool m_useQrc = true;               ///< 是否使用 Qt 资源系统
    
    // 状态
    bool m_initialized = false;  ///< 是否已初始化
};

#endif // RESOURCE_MANAGER_H
