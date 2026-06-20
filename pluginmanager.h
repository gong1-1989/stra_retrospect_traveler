/**
 * @file PluginManager.h
 * @brief 插件管理器类
 * 
 * 本文件定义了插件系统的核心接口和管理器类，包括：
 * - IPlugin：插件接口基类，所有插件必须实现此接口
 * - PluginManager：插件管理器类，负责插件的发现、加载、卸载和管理
 * 
 * 插件系统是游戏模块化+插件化混合架构的重要组成部分，
 * 支持通过插件扩展游戏功能，如内容插件、玩法插件、UI插件、MOD插件等。
 * 
 * 主要特性：
 * 1. 统一接口：所有插件都实现 IPlugin 接口，提供一致的插件管理方式
 * 2. 动态加载：支持运行时动态加载和卸载插件
 * 3. 插件发现：自动扫描插件目录，发现可用插件
 * 4. 跨平台：支持 Windows (dll)、macOS (dylib/so)、Linux (so)
 * 5. 信号通知：插件加载/卸载时发出信号，便于其他系统响应
 * 
 * 插件类型：
 * - 内容插件：添加新的关卡、角色、物品等游戏内容
 * - 玩法插件：添加新的游戏玩法和机制
 * - UI插件：添加新的UI界面和功能
 * - 渲染插件：添加新的渲染效果和技术
 * - 工具插件：添加开发和调试工具
 * - MOD插件：玩家创建的游戏修改模块
 * 
 * 使用示例：
 * @code
 * PluginManager pluginManager;
 * pluginManager.initialize();
 * 
 * // 加载所有插件
 * int loadedCount = pluginManager.loadAllPlugins("plugins");
 * qDebug() << "加载了" << loadedCount << "个插件";
 * 
 * // 获取插件
 * IPlugin* plugin = pluginManager.getPlugin("MyPlugin");
 * if (plugin) {
 *     qDebug() << "插件名称:" << plugin->getName();
 *     qDebug() << "插件版本:" << plugin->getVersion();
 * }
 * 
 * // 卸载插件
 * pluginManager.unloadPlugin("MyPlugin");
 * 
 * // 关闭插件管理器
 * pluginManager.shutdown();
 * @endcode
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <QObject>
#include <QPluginLoader>
#include <QList>
#include <QMap>
#include <QString>
#include <QDir>
#include <QStringList>

/**
 * @brief 插件接口基类
 * 
 * 所有插件必须实现此接口。
 * 插件接口定义了插件的基本属性和生命周期方法。
 * 
 * 插件生命周期：
 * 1. 加载：插件被动态加载到内存中
 * 2. 初始化：调用 initialize() 初始化插件
 * 3. 运行：插件正常运行，提供功能
 * 4. 关闭：调用 shutdown() 关闭插件，清理资源
 * 5. 卸载：插件从内存中卸载
 * 
 * 插件属性：
 * - 名称：插件的唯一标识符
 * - 版本：插件的版本号
 * - 作者：插件的作者
 * - 描述：插件的功能描述
 * 
 * 注意：
 * - 插件名称应该是唯一的，用于标识和查找插件
 * - 版本号建议使用语义化版本（如 1.0.0）
 * - initialize() 和 shutdown() 必须成对调用
 * - 插件应该在 shutdown() 中清理所有资源
 * 
 * 实现插件的步骤：
 * 1. 创建一个类，继承 QObject 和 IPlugin
 * 2. 实现所有纯虚函数
 * 3. 在类声明中添加 Q_OBJECT 宏
 * 4. 在类声明中添加 Q_INTERFACES(IPlugin) 宏
 * 5. 在类声明中添加 Q_PLUGIN_METADATA 宏
 * 6. 编译为动态库
 */
class IPlugin
{
public:
    /**
     * @brief 虚析构函数
     * 
     * 确保通过基类指针删除派生类对象时，
     * 派生类的析构函数被正确调用。
     */
    virtual ~IPlugin() = default;
    
    /**
     * @brief 获取插件名称
     * 
     * 返回插件的名称。
     * 插件名称应该是唯一的，用于标识和查找插件。
     * 
     * @return 插件名称
     */
    virtual QString getName() const = 0;
    
    /**
     * @brief 获取插件版本
     * 
     * 返回插件的版本号。
     * 建议使用语义化版本（如 1.0.0）。
     * 
     * @return 版本号
     */
    virtual QString getVersion() const = 0;
    
    /**
     * @brief 获取插件作者
     * 
     * 返回插件的作者。
     * 
     * @return 作者
     */
    virtual QString getAuthor() const = 0;
    
    /**
     * @brief 获取插件描述
     * 
     * 返回插件的功能描述。
     * 
     * @return 描述
     */
    virtual QString getDescription() const = 0;
    
    /**
     * @brief 初始化插件
     * 
     * 初始化插件，执行必要的初始化操作。
     * 插件加载后会调用此方法。
     * 
     * 注意：
     * - 在此方法中执行插件的初始化操作
     * - 如果初始化失败，返回 false
     * - 初始化失败的插件会被卸载
     * 
     * @return true 初始化成功
     * @return false 初始化失败
     */
    virtual bool initialize() = 0;
    
    /**
     * @brief 关闭插件
     * 
     * 关闭插件，清理所有资源。
     * 插件卸载前会调用此方法。
     * 
     * 注意：
     * - 在此方法中清理插件分配的所有资源
     * - 确保没有内存泄漏
     * - 此方法应该是幂等的
     */
    virtual void shutdown() = 0;
};

/**
 * @brief 声明插件接口
 * 
 * 使用 Qt 的插件系统声明 IPlugin 接口。
 * 这是 Qt 插件系统的要求，用于识别插件接口。
 * 
 * 接口标识符：com.starretrospect.IPlugin/1.0
 * - 命名空间：com.starretrospect
 * - 接口名：IPlugin
 * - 版本：1.0
 */
Q_DECLARE_INTERFACE(IPlugin, "com.starretrospect.IPlugin/1.0")

/**
 * @brief 插件管理器类
 * 
 * 负责插件的发现、加载、卸载和管理。
 * 插件管理器是游戏插件系统的核心，统一管理所有插件。
 * 
 * 架构说明：
 * - 插件管理器使用 QPluginLoader 加载动态库插件
 * - 插件通过 IPlugin 接口提供统一的访问方式
 * - 插件加载后会被保存在映射表中，便于快速查找
 * - 插件管理器支持跨平台插件加载
 * 
 * 插件管理功能：
 * - 插件发现：扫描插件目录，发现可用插件
 * - 插件加载：加载单个插件或批量加载所有插件
 * - 插件卸载：卸载指定插件或所有插件
 * - 插件查询：获取插件信息，检查插件是否已加载
 * - 信号通知：插件加载/卸载时发出信号
 * 
 * 插件目录结构：
 * - plugins/：插件根目录
 *   - plugin1.dll：插件动态库（Windows）
 *   - plugin2.so：插件动态库（Linux/macOS）
 * 
 * 注意：
 * - 插件管理器不拥有插件对象的所有权吗？
 * - 不，插件对象由 QPluginLoader 管理，插件管理器只是保存指针
 * - 卸载插件时，QPluginLoader 会负责删除插件对象
 * - 不要手动删除插件对象
 * 
 * 线程安全：
 * - 目前插件管理器不是线程安全的
 * - 所有操作应该在主线程中执行
 * - 后续可以添加线程安全支持
 */
class PluginManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * 
     * 创建插件管理器实例，但不进行初始化。
     * 必须调用 initialize() 完成初始化后才能使用。
     * 
     * @param parent 父对象，用于 Qt 对象树内存管理
     */
    explicit PluginManager(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     * 
     * 销毁插件管理器，自动调用 shutdown() 卸载所有插件。
     */
    ~PluginManager() override;

    // ============================================================
    // 生命周期管理
    // ============================================================
    
    /**
     * @brief 初始化插件管理器
     * 
     * 初始化插件管理器，准备加载插件。
     * 
     * 注意：
     * - 此方法不会自动加载插件
     * - 需要调用 loadAllPlugins() 或 loadPlugin() 来加载插件
     * - 可以通过 setPluginDirectory() 设置插件目录
     * 
     * @return true 初始化成功
     * @return false 初始化失败
     */
    bool initialize();
    
    /**
     * @brief 关闭插件管理器
     * 
     * 关闭插件管理器，卸载所有插件，清理资源。
     * 
     * 执行以下操作：
     * 1. 卸载所有已加载的插件
     * 2. 清空插件映射表
     * 3. 重置状态
     * 
     * 此函数是幂等的，可以安全地多次调用。
     */
    void shutdown();

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
     * 
     * @param pluginDir 插件目录，默认为 "plugins"
     * @return 加载成功的插件数量
     */
    int loadAllPlugins(const QString& pluginDir = "plugins");
    
    /**
     * @brief 加载单个插件
     * 
     * 加载指定路径的插件。
     * 
     * 执行以下操作：
     * 1. 检查插件是否已加载
     * 2. 创建插件加载器
     * 3. 加载插件动态库
     * 4. 获取插件实例
     * 5. 转换为 IPlugin 接口
     * 6. 初始化插件
     * 7. 保存插件信息
     * 8. 发出 pluginLoaded 信号
     * 
     * 如果任何一步失败，都会清理资源并返回 false。
     * 
     * @param pluginPath 插件文件路径
     * @return true 加载成功
     * @return false 加载失败
     */
    bool loadPlugin(const QString& pluginPath);
    
    /**
     * @brief 卸载插件
     * 
     * 卸载指定名称的插件。
     * 
     * 执行以下操作：
     * 1. 检查插件是否存在
     * 2. 关闭插件（调用 shutdown()）
     * 3. 找到对应的插件加载器
     * 4. 卸载插件动态库
     * 5. 删除插件加载器
     * 6. 从映射表中移除插件
     * 7. 发出 pluginUnloaded 信号
     * 
     * @param pluginName 插件名称
     * @return true 卸载成功
     * @return false 插件不存在
     */
    bool unloadPlugin(const QString& pluginName);

    // ============================================================
    // 插件查询
    // ============================================================
    
    /**
     * @brief 获取插件
     * 
     * 根据插件名称获取插件指针。
     * 
     * @param pluginName 插件名称
     * @return 插件指针，如果不存在返回 nullptr
     */
    IPlugin* getPlugin(const QString& pluginName) const;
    
    /**
     * @brief 获取所有已加载的插件
     * 
     * 返回所有已加载插件的列表。
     * 
     * @return 插件列表
     */
    QList<IPlugin*> getLoadedPlugins() const;
    
    /**
     * @brief 检查插件是否已加载
     * 
     * 检查指定名称的插件是否已加载。
     * 
     * @param pluginName 插件名称
     * @return true 已加载
     * @return false 未加载
     */
    bool isPluginLoaded(const QString& pluginName) const;

    // ============================================================
    // 插件目录管理
    // ============================================================
    
    /**
     * @brief 获取插件目录
     * @return 插件目录
     */
    QString getPluginDirectory() const { return m_pluginDirectory; }
    
    /**
     * @brief 设置插件目录
     * 
     * 设置插件目录。
     * 注意：这不会自动重新加载插件，
     * 需要手动调用 loadAllPlugins() 重新加载。
     * 
     * @param dir 插件目录
     */
    void setPluginDirectory(const QString& dir) { m_pluginDirectory = dir; }

signals:
    /**
     * @brief 插件加载信号
     * 
     * 当插件加载成功后发出，携带插件名称。
     * 可以连接此信号来响应插件加载事件。
     * 
     * @param pluginName 插件名称
     */
    void pluginLoaded(const QString& pluginName);
    
    /**
     * @brief 插件卸载信号
     * 
     * 当插件卸载成功后发出，携带插件名称。
     * 可以连接此信号来响应插件卸载事件。
     * 
     * @param pluginName 插件名称
     */
    void pluginUnloaded(const QString& pluginName);

private:
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
     * @param dir 目录
     * @return 插件文件绝对路径列表
     */
    QStringList scanPluginDirectory(const QDir& dir);

    // ============================================================
    // 成员变量
    // ============================================================
    
    // 插件加载器映射（路径 -> 加载器）
    QMap<QString, QPluginLoader*> m_pluginLoaders;  ///< 插件加载器映射
    
    // 插件映射（名称 -> 插件接口）
    QMap<QString, IPlugin*> m_plugins;  ///< 插件映射
    
    // 插件目录
    QString m_pluginDirectory = "plugins";  ///< 插件目录
    
    // 初始化状态
    bool m_initialized = false;  ///< 是否已初始化
};

#endif // PLUGIN_MANAGER_H
