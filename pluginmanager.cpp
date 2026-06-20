/**
 * @file PluginManager.cpp
 * @brief 插件管理器类实现
 * 
 * 本文件实现了插件管理器的所有功能，包括：
 * - 插件的发现、加载、卸载和管理
 * - 插件目录扫描
 * - 插件生命周期管理
 * 
 * 插件管理器是游戏插件系统的核心，
 * 统一管理所有插件，支持动态加载和卸载。
 * 
 * 插件加载流程：
 * 1. 扫描插件目录，查找插件文件
 * 2. 创建 QPluginLoader 加载插件动态库
 * 3. 获取插件实例
 * 4. 转换为 IPlugin 接口
 * 5. 调用 initialize() 初始化插件
 * 6. 保存插件信息
 * 7. 发出 pluginLoaded 信号
 * 
 * 插件卸载流程：
 * 1. 查找插件
 * 2. 调用 shutdown() 关闭插件
 * 3. 卸载插件动态库
 * 4. 删除 QPluginLoader
 * 5. 从映射表中移除
 * 6. 发出 pluginUnloaded 信号
 * 
 * 跨平台支持：
 * - Windows：加载 .dll 文件
 * - macOS：加载 .dylib 和 .so 文件
 * - Linux：加载 .so 文件
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#include "PluginManager.h"

#include <QDebug>
#include <QFileInfo>
#include <QFileInfoList>

// ============================================================
// 构造函数与析构函数
// ============================================================

/**
 * @brief 构造函数
 * 
 * 创建插件管理器实例，但不进行任何初始化操作。
 * 所有的初始化工作都在 initialize() 中完成。
 * 
 * @param parent 父对象，用于 Qt 对象树内存管理
 */
PluginManager::PluginManager(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[PluginManager] 创建插件管理器";
}

/**
 * @brief 析构函数
 * 
 * 销毁插件管理器，自动调用 shutdown() 卸载所有插件。
 */
PluginManager::~PluginManager()
{
    qDebug() << "[PluginManager] 销毁插件管理器";
    shutdown();
}

// ============================================================
// 生命周期管理
// ============================================================

/**
 * @brief 初始化插件管理器
 * 
 * 初始化插件管理器，准备加载插件。
 * 
 * 执行以下操作：
 * 1. 检查是否已经初始化（防止重复初始化）
 * 2. 记录插件目录
 * 3. 设置初始化标志
 * 
 * 注意：
 * - 此方法不会自动加载插件
 * - 需要调用 loadAllPlugins() 或 loadPlugin() 来加载插件
 * - 可以通过 setPluginDirectory() 设置插件目录
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
bool PluginManager::initialize()
{
    // 防止重复初始化
    if (m_initialized) {
        qWarning() << "[PluginManager] 插件管理器已经初始化，跳过重复初始化";
        return true;
    }

    qDebug() << "[PluginManager] 初始化插件管理器...";
    qDebug() << "[PluginManager] 插件目录:" << m_pluginDirectory;
    
    // 设置初始化标志
    m_initialized = true;
    
    qDebug() << "[PluginManager] 插件管理器初始化完成";
    return true;
}

/**
 * @brief 关闭插件管理器
 * 
 * 关闭插件管理器，卸载所有插件，清理资源。
 * 
 * 执行以下操作：
 * 1. 检查是否已经初始化
 * 2. 获取所有已加载插件的名称
 * 3. 逐个卸载插件
 * 4. 清空插件映射表
 * 5. 清空插件加载器映射表
 * 6. 重置初始化标志
 * 
 * 注意：
 * - 插件按名称排序后卸载，卸载顺序可能与加载顺序不同
 * - 每个插件卸载前都会调用 shutdown()
 * - 卸载失败的插件会被跳过，但映射表会被清空
 * 
 * 此函数是幂等的，可以安全地多次调用。
 */
void PluginManager::shutdown()
{
    // 如果没有初始化，直接返回
    if (!m_initialized) {
        return;
    }

    qDebug() << "[PluginManager] 关闭插件管理器...";
    
    // 1. 获取所有已加载插件的名称
    QStringList pluginNames = m_plugins.keys();
    
    // 2. 逐个卸载插件
    qDebug() << "[PluginManager] 卸载" << pluginNames.size() << "个插件";
    for (const QString& name : pluginNames) {
        unloadPlugin(name);
    }
    
    // 3. 清空映射表
    m_plugins.clear();
    m_pluginLoaders.clear();
    
    // 4. 重置初始化标志
    m_initialized = false;
    
    qDebug() << "[PluginManager] 插件管理器已关闭";
}

// ============================================================
// 插件加载与卸载
// ============================================================

/**
 * @brief 加载所有插件
 * 
 * 扫描指定目录，加载所有可用的插件。
 * 
 * 执行以下操作：
 * 1. 设置插件目录
 * 2. 检查目录是否存在
 * 3. 扫描目录，查找插件文件
 * 4. 逐个加载插件
 * 5. 返回加载成功的插件数量
 * 
 * 注意：
 * - 如果目录不存在，返回 0
 * - 加载失败的插件会被跳过，不会影响其他插件
 * - 可以通过 pluginLoaded 信号跟踪每个插件的加载状态
 * - 插件加载顺序与文件系统返回的顺序一致
 * 
 * @param pluginDir 插件目录，默认为 "plugins"
 * @return 加载成功的插件数量
 */
int PluginManager::loadAllPlugins(const QString& pluginDir)
{
    qDebug() << "[PluginManager] 加载所有插件，目录:" << pluginDir;

    // 1. 设置插件目录
    m_pluginDirectory = pluginDir;
    
    // 2. 检查目录是否存在
    QDir dir(pluginDir);
    if (!dir.exists()) {
        qWarning() << "[PluginManager] 插件目录不存在:" << pluginDir;
        return 0;
    }
    
    // 3. 扫描目录，查找插件文件
    QStringList pluginFiles = scanPluginDirectory(dir);
    qDebug() << "[PluginManager] 发现" << pluginFiles.size() << "个插件文件";
    
    // 4. 逐个加载插件
    int loadedCount = 0;
    for (const QString& pluginFile : pluginFiles) {
        if (loadPlugin(pluginFile)) {
            loadedCount++;
        }
    }
    
    qDebug() << "[PluginManager] 成功加载" << loadedCount << "个插件";
    return loadedCount;
}

/**
 * @brief 加载单个插件
 * 
 * 加载指定路径的插件。
 * 
 * 执行以下操作：
 * 1. 检查插件是否已加载（按路径检查）
 * 2. 创建插件加载器
 * 3. 加载插件动态库，获取插件实例
 * 4. 检查插件实例是否有效
 * 5. 转换为 IPlugin 接口
 * 6. 检查接口转换是否成功
 * 7. 初始化插件
 * 8. 检查初始化是否成功
 * 9. 保存插件加载器和插件接口
 * 10. 发出 pluginLoaded 信号
 * 
 * 如果任何一步失败，都会清理资源并返回 false。
 * 
 * 错误处理：
 * - 插件已加载：返回 false，输出警告
 * - 加载失败：删除加载器，返回 false，输出错误信息
 * - 接口不匹配：卸载插件，删除加载器，返回 false
 * - 初始化失败：卸载插件，删除加载器，返回 false
 * 
 * @param pluginPath 插件文件路径
 * @return true 加载成功
 * @return false 加载失败
 */
bool PluginManager::loadPlugin(const QString& pluginPath)
{
    qDebug() << "[PluginManager] 加载插件:" << pluginPath;
    
    // 1. 检查插件是否已加载（按路径检查）
    if (m_pluginLoaders.contains(pluginPath)) {
        qWarning() << "[PluginManager] 插件已加载:" << pluginPath;
        return false;
    }
    
    // 2. 创建插件加载器
    QPluginLoader* loader = new QPluginLoader(pluginPath, this);
    
    // 3. 加载插件动态库，获取插件实例
    QObject* pluginInstance = loader->instance();
    if (!pluginInstance) {
        qWarning() << "[PluginManager] 插件加载失败:" << pluginPath;
        qWarning() << "[PluginManager] 错误信息:" << loader->errorString();
        delete loader;
        return false;
    }
    
    // 4. 转换为 IPlugin 接口
    IPlugin* plugin = qobject_cast<IPlugin*>(pluginInstance);
    if (!plugin) {
        qWarning() << "[PluginManager] 插件未实现 IPlugin 接口:" << pluginPath;
        loader->unload();
        delete loader;
        return false;
    }
    
    // 5. 初始化插件
    if (!plugin->initialize()) {
        qWarning() << "[PluginManager] 插件初始化失败:" << plugin->getName();
        loader->unload();
        delete loader;
        return false;
    }
    
    // 6. 保存插件信息
    m_pluginLoaders[pluginPath] = loader;
    m_plugins[plugin->getName()] = plugin;
    
    // 7. 发出插件加载信号
    emit pluginLoaded(plugin->getName());
    
    qDebug() << "[PluginManager] 插件加载成功:" << plugin->getName() 
             << "v" << plugin->getVersion()
             << "作者:" << plugin->getAuthor();
    
    return true;
}

/**
 * @brief 卸载插件
 * 
 * 卸载指定名称的插件。
 * 
 * 执行以下操作：
 * 1. 检查插件是否存在
 * 2. 获取插件指针
 * 3. 关闭插件（调用 shutdown()）
 * 4. 遍历插件加载器映射，找到对应的加载器
 * 5. 卸载插件动态库
 * 6. 删除插件加载器
 * 7. 从插件加载器映射表中移除
 * 8. 从插件映射表中移除
 * 9. 发出 pluginUnloaded 信号
 * 
 * 注意：
 * - 查找加载器时需要遍历映射表，因为映射表是按路径索引的
 * - 可以优化为按名称索引加载器，提高查找效率
 * - 卸载插件后，插件对象会被 QPluginLoader 删除
 * - 不要保留插件指针的副本，卸载后指针会失效
 * 
 * @param pluginName 插件名称
 * @return true 卸载成功
 * @return false 插件不存在
 */
bool PluginManager::unloadPlugin(const QString& pluginName)
{
    // 1. 检查插件是否存在
    if (!m_plugins.contains(pluginName)) {
        qWarning() << "[PluginManager] 插件不存在:" << pluginName;
        return false;
    }
    
    // 2. 获取插件指针
    IPlugin* plugin = m_plugins[pluginName];
    qDebug() << "[PluginManager] 卸载插件:" << pluginName;
    
    // 3. 关闭插件
    plugin->shutdown();
    
    // 4. 找到对应的插件加载器
    // 注意：这里需要遍历映射表，因为映射表是按路径索引的
    // 可以优化为按名称索引加载器，提高查找效率
    QString pluginPath;
    QMap<QString, QPluginLoader*>::iterator it;
    for (it = m_pluginLoaders.begin(); it != m_pluginLoaders.end(); ++it) {
        if (qobject_cast<IPlugin*>(it.value()->instance()) == plugin) {
            pluginPath = it.key();
            break;
        }
    }
    
    // 5. 卸载插件动态库
    if (!pluginPath.isEmpty()) {
        QPluginLoader* loader = m_pluginLoaders[pluginPath];
        
        // 卸载插件动态库
        loader->unload();
        
        // 从映射表中移除
        m_pluginLoaders.remove(pluginPath);
        
        // 删除加载器
        delete loader;
    }
    
    // 6. 从插件映射表中移除
    m_plugins.remove(pluginName);
    
    // 7. 发出插件卸载信号
    emit pluginUnloaded(pluginName);
    
    qDebug() << "[PluginManager] 插件卸载成功:" << pluginName;
    return true;
}

// ============================================================
// 插件查询
// ============================================================

/**
 * @brief 获取插件
 * 
 * 根据插件名称获取插件指针。
 * 
 * 如果插件不存在，返回 nullptr。
 * 
 * @param pluginName 插件名称
 * @return 插件指针，如果不存在返回 nullptr
 */
IPlugin* PluginManager::getPlugin(const QString& pluginName) const
{
    return m_plugins.value(pluginName, nullptr);
}

/**
 * @brief 获取所有已加载的插件
 * 
 * 返回所有已加载插件的列表。
 * 
 * 注意：
 * - 返回的是插件指针列表
 * - 不要删除这些指针，它们由插件管理器管理
 * - 插件卸载后，对应的指针会失效
 * 
 * @return 插件列表
 */
QList<IPlugin*> PluginManager::getLoadedPlugins() const
{
    return m_plugins.values();
}

/**
 * @brief 检查插件是否已加载
 * 
 * 检查指定名称的插件是否已加载。
 * 
 * @param pluginName 插件名称
 * @return true 已加载
 * @return false 未加载
 */
bool PluginManager::isPluginLoaded(const QString& pluginName) const
{
    return m_plugins.contains(pluginName);
}

// ============================================================
// 插件目录扫描
// ============================================================

/**
 * @brief 扫描插件目录
 * 
 * 扫描指定目录，查找所有插件文件。
 * 
 * 根据操作系统筛选不同的文件扩展名：
 * - Windows：*.dll
 * - macOS：*.dylib, *.so
 * - Linux：*.so
 * 
 * 执行以下操作：
 * 1. 根据操作系统设置文件过滤器
 * 2. 获取目录中所有匹配的文件
 * 3. 收集文件的绝对路径
 * 4. 返回文件路径列表
 * 
 * 注意：
 * - 只扫描当前目录，不递归扫描子目录
 * - 只返回文件，不返回目录
 * - 返回的是绝对路径
 * 
 * @param dir 目录
 * @return 插件文件绝对路径列表
 */
QStringList PluginManager::scanPluginDirectory(const QDir& dir)
{
    QStringList result;
    
    // 1. 根据操作系统设置文件过滤器
    QStringList filters;
#ifdef Q_OS_WIN
    // Windows: DLL 文件
    filters << "*.dll";
#elif defined(Q_OS_MAC)
    // macOS: dylib 和 so 文件
    filters << "*.dylib" << "*.so";
#else
    // Linux: so 文件
    filters << "*.so";
#endif
    
    // 2. 获取目录中所有匹配的文件
    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files);
    
    // 3. 收集文件的绝对路径
    for (const QFileInfo& fileInfo : fileInfoList) {
        result.append(fileInfo.absoluteFilePath());
    }
    
    qDebug() << "[PluginManager] 在目录中找到" << result.size() << "个插件文件:" << dir.absolutePath();
    
    return result;
}
