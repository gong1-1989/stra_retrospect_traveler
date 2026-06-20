/**
 * @file ResourceManager.cpp
 * @brief 资源管理器类实现
 * 
 * 本文件实现了资源管理器的所有功能，包括：
 * - 资源的加载和缓存
 * - 图片、音效、字体等资源的管理
 * - 缓存的清理和垃圾回收
 * - Qt 资源系统支持
 * 
 * 资源管理器采用缓存机制，已加载的资源会被保存在内存中，
 * 下次请求相同资源时直接从缓存返回，避免重复加载的开销。
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#include "ResourceManager.h"

#include <QDebug>
#include <QDir>
#include <QSoundEffect>
#include <QFontDatabase>
#include <QUrl>

// ============================================================
// 构造函数与析构函数
// ============================================================

/**
 * @brief 构造函数
 * 
 * 创建资源管理器实例，但不进行任何初始化操作。
 * 所有的初始化工作都在 initialize() 中完成。
 * 
 * @param parent 父对象，用于 Qt 对象树内存管理
 */
ResourceManager::ResourceManager(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[ResourceManager] 创建资源管理器";
}

/**
 * @brief 析构函数
 * 
 * 销毁资源管理器，自动调用 shutdown() 确保所有缓存资源
 * 都被正确释放。
 */
ResourceManager::~ResourceManager()
{
    qDebug() << "[ResourceManager] 销毁资源管理器";
    shutdown();
}

// ============================================================
// 生命周期管理
// ============================================================

/**
 * @brief 初始化资源管理器
 * 
 * 执行以下初始化操作：
 * 1. 检查是否已经初始化（防止重复初始化）
 * 2. 检查资源目录是否存在
 * 3. 如果目录不存在且不使用 qrc，则创建目录
 * 
 * 注意：使用 Qt 资源系统（qrc）时不需要文件系统目录。
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
bool ResourceManager::initialize()
{
    // 防止重复初始化
    if (m_initialized) {
        qWarning() << "[ResourceManager] 资源管理器已经初始化，跳过重复初始化";
        return true;
    }

    qDebug() << "[ResourceManager] 初始化资源管理器...";
    qDebug() << "[ResourceManager] 资源基础路径:" << m_resourcePath;
    qDebug() << "[ResourceManager] 使用 Qt 资源系统:" << (m_useQrc ? "是" : "否");

    // 如果不使用 qrc，检查资源目录是否存在
    if (!m_useQrc) {
        QDir dir(m_resourcePath);
        if (!dir.exists()) {
            qWarning() << "[ResourceManager] 资源目录不存在:" << m_resourcePath;
            qWarning() << "[ResourceManager] 正在创建资源目录...";
            
            // 创建资源目录
            if (!dir.mkpath(".")) {
                qCritical() << "[ResourceManager] 创建资源目录失败！";
                return false;
            }
            
            qDebug() << "[ResourceManager] 资源目录创建成功";
        }
    }

    // 初始化完成
    m_initialized = true;
    
    qDebug() << "[ResourceManager] 资源管理器初始化完成";
    return true;
}

/**
 * @brief 关闭资源管理器
 * 
 * 释放所有缓存的资源，清理内存。
 * 包括：
 * - 释放所有音效资源（QSoundEffect 需要手动删除）
 * - 清空图片缓存
 * - 清空字体缓存
 * 
 * 此函数是幂等的，可以安全地多次调用。
 */
void ResourceManager::shutdown()
{
    // 如果没有初始化，直接返回
    if (!m_initialized) {
        return;
    }

    qDebug() << "[ResourceManager] 关闭资源管理器...";

    // 1. 清理音效缓存（需要手动删除）
    qDebug() << "[ResourceManager] 清理音效缓存，数量:" << m_soundCache.size();
    qDeleteAll(m_soundCache);
    m_soundCache.clear();

    // 2. 清理图片缓存（QPixmap 会自动管理内存）
    qDebug() << "[ResourceManager] 清理图片缓存，数量:" << m_pixmapCache.size();
    m_pixmapCache.clear();

    // 3. 清理字体缓存
    qDebug() << "[ResourceManager] 清理字体缓存，数量:" << m_fontCache.size();
    m_fontCache.clear();

    // 重置状态
    m_initialized = false;
    
    qDebug() << "[ResourceManager] 资源管理器已关闭";
}

// ============================================================
// 资源加载接口
// ============================================================

/**
 * @brief 加载图片资源
 * 
 * 加载流程：
 * 1. 计算完整路径
 * 2. 检查缓存中是否已存在该图片
 * 3. 如果存在，直接返回缓存的图片
 * 4. 如果不存在，从文件系统或 qrc 加载
 * 5. 加载成功则缓存并返回
 * 6. 加载失败则返回灰色占位图
 * 
 * @param path 资源相对路径（相对于资源基础路径）
 * @return QPixmap 加载的图片，失败时返回 100x100 的灰色占位图
 */
QPixmap ResourceManager::loadPixmap(const QString& path)
{
    // 计算完整路径
    QString fullPath = getFullPath(path);
    
    // 1. 检查缓存
    if (m_pixmapCache.contains(fullPath)) {
        qDebug() << "[ResourceManager] 从缓存加载图片:" << path;
        return m_pixmapCache[fullPath];
    }
    
    // 2. 加载图片
    qDebug() << "[ResourceManager] 从文件加载图片:" << path;
    QPixmap pixmap;
    
    if (pixmap.load(fullPath)) {
        // 加载成功，加入缓存
        m_pixmapCache[fullPath] = pixmap;
        qDebug() << "[ResourceManager] 图片加载成功，尺寸:" 
                 << pixmap.width() << "x" << pixmap.height();
    } else {
        // 加载失败，创建占位图
        qWarning() << "[ResourceManager] 图片加载失败:" << fullPath;
        
        // 创建 100x100 的灰色占位图
        const int PLACEHOLDER_SIZE = 100;
        pixmap = QPixmap(PLACEHOLDER_SIZE, PLACEHOLDER_SIZE);
        pixmap.fill(Qt::gray);
    }
    
    return pixmap;
}

/**
 * @brief 加载音效资源
 * 
 * 加载流程：
 * 1. 计算完整路径
 * 2. 检查缓存中是否已存在该音效
 * 3. 如果存在，直接返回缓存的音效
 * 4. 如果不存在，从文件系统加载
 * 5. 加载成功则缓存并返回
 * 6. 加载失败则返回 nullptr
 * 
 * 注意：
 * - QSoundEffect 适合播放短音效（如点击声、爆炸声等）
 * - 不适合播放长音乐（如背景音乐），背景音乐应使用 QMediaPlayer
 * - 音效对象由资源管理器管理，不需要手动删除
 * 
 * @param path 资源相对路径（相对于资源基础路径）
 * @return QSoundEffect* 音效指针，失败时返回 nullptr
 */
QSoundEffect* ResourceManager::loadSound(const QString& path)
{
    // 计算完整路径
    QString fullPath = getFullPath(path);
    
    // 1. 检查缓存
    if (m_soundCache.contains(fullPath)) {
        qDebug() << "[ResourceManager] 从缓存加载音效:" << path;
        return m_soundCache[fullPath];
    }
    
    // 2. 加载音效
    qDebug() << "[ResourceManager] 从文件加载音效:" << path;
    QSoundEffect* sound = new QSoundEffect(this);
    sound->setSource(QUrl::fromLocalFile(fullPath));
    
    // 检查是否加载成功
    if (sound->status() == QSoundEffect::Error) {
        qWarning() << "[ResourceManager] 音效加载失败:" << fullPath;
        delete sound;
        return nullptr;
    }
    
    // 加载成功，加入缓存
    m_soundCache[fullPath] = sound;
    qDebug() << "[ResourceManager] 音效加载成功";
    
    return sound;
}

/**
 * @brief 加载字体资源
 * 
 * 加载流程：
 * 1. 生成缓存键（路径 + 字号）
 * 2. 检查缓存中是否已存在该字体
 * 3. 如果存在，直接返回缓存的字体
 * 4. 如果不存在，从文件系统加载字体文件
 * 5. 加载成功则创建指定字号的字体并缓存
 * 6. 加载失败则返回默认 Arial 字体
 * 
 * 缓存策略：
 * - 不同字号的字体会分别缓存
 * - 缓存键格式："路径_字号"
 * 
 * @param path 资源相对路径（相对于资源基础路径）
 * @param pointSize 字体大小（磅值）
 * @return QFont 加载的字体，失败时返回默认 Arial 字体
 */
QFont ResourceManager::loadFont(const QString& path, int pointSize)
{
    // 生成缓存键（包含字号）
    QString cacheKey = QString("%1_%2").arg(path).arg(pointSize);
    
    // 1. 检查缓存
    if (m_fontCache.contains(cacheKey)) {
        qDebug() << "[ResourceManager] 从缓存加载字体:" << path << "(" << pointSize << "pt)";
        return m_fontCache[cacheKey];
    }
    
    // 2. 加载字体
    QString fullPath = getFullPath(path);
    qDebug() << "[ResourceManager] 从文件加载字体:" << path;
    
    QFont font;
    int fontId = QFontDatabase::addApplicationFont(fullPath);
    
    if (fontId != -1) {
        // 加载成功
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
        font = QFont(fontFamily, pointSize);
        qDebug() << "[ResourceManager] 字体加载成功，字体族:" << fontFamily;
    } else {
        // 加载失败，使用默认字体
        qWarning() << "[ResourceManager] 字体加载失败:" << fullPath;
        qWarning() << "[ResourceManager] 使用默认字体 Arial";
        font = QFont("Arial", pointSize);
    }
    
    // 加入缓存
    m_fontCache[cacheKey] = font;
    
    return font;
}

// ============================================================
// 缓存管理
// ============================================================

/**
 * @brief 垃圾回收
 * 
 * 卸载未使用的资源，释放内存。
 * 可以在场景切换、内存不足等时机调用。
 * 
 * 注意：当前实现为占位版本，仅输出日志。
 * 后续可实现更复杂的垃圾回收策略，如：
 * - 基于引用计数的回收
 * - 基于 LRU（最近最少使用）的回收
 * - 按资源类型分级回收
 */
void ResourceManager::collectGarbage()
{
    qDebug() << "[ResourceManager] 执行资源垃圾回收...";
    
    // TODO: 实现完整的垃圾回收逻辑
    // 目前仅输出统计信息
    qDebug() << "[ResourceManager] 当前缓存统计：";
    qDebug() << "[ResourceManager]   图片缓存:" << m_pixmapCache.size() << "个";
    qDebug() << "[ResourceManager]   音效缓存:" << m_soundCache.size() << "个";
    qDebug() << "[ResourceManager]   字体缓存:" << m_fontCache.size() << "个";
    
    qDebug() << "[ResourceManager] 垃圾回收完成";
}

/**
 * @brief 清空所有缓存
 * 
 * 强制清空所有资源缓存，释放所有内存。
 * 
 * 警告：
 * - 此操作会释放所有缓存的资源
 * - 后续访问相同资源需要重新加载
 * - 正在使用的资源指针可能失效
 * 
 * 建议在以下场景使用：
 * - 游戏退出时
 * - 切换大型场景前
 * - 内存严重不足时
 */
void ResourceManager::clearCache()
{
    qDebug() << "[ResourceManager] 清空所有资源缓存...";
    
    // 清空图片缓存
    m_pixmapCache.clear();
    
    // 清空音效缓存（需要手动删除）
    qDeleteAll(m_soundCache);
    m_soundCache.clear();
    
    // 清空字体缓存
    m_fontCache.clear();
    
    qDebug() << "[ResourceManager] 所有缓存已清空";
}

// ============================================================
// 私有辅助方法
// ============================================================

/**
 * @brief 获取完整资源路径
 * 
 * 根据当前配置拼接完整的资源路径：
 * - 如果使用 Qt 资源系统：返回 ":/相对路径"
 * - 如果使用文件系统：返回 "基础路径/相对路径"
 * 
 * @param relativePath 相对路径
 * @return QString 完整的资源路径
 */
QString ResourceManager::getFullPath(const QString& relativePath) const
{
    if (m_useQrc) {
        // 使用 Qt 资源系统，格式为 ":/path/to/resource"
        return QString(":/%1").arg(relativePath);
    } else {
        // 使用文件系统，拼接基础路径和相对路径
        return QString("%1/%2").arg(m_resourcePath, relativePath);
    }
}
