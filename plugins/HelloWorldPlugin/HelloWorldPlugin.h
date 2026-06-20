/**
 * @file HelloWorldPlugin.h
 * @brief HelloWorld 示例插件
 * 
 * 这是一个最简单的插件示例，展示如何编写一个星溯旅人插件。
 * 
 * 插件功能：
 * - 插件加载时输出 Hello World 信息
 * - 提供一个简单的问候功能
 * - 展示插件的基本结构和生命周期
 * 
 * 这个插件主要用于学习和测试插件系统。
 * 
 * @author 星溯旅人开发团队
 * @version 1.0.0
 * @date 2026
 */
#ifndef HELLO_WORLD_PLUGIN_H
#define HELLO_WORLD_PLUGIN_H

#include <QObject>
#include <QString>
#include "../PluginManager.h"  // 引入 IPlugin 接口

/**
 * @brief HelloWorld 插件类
 * 
 * 这是一个最简单的插件示例，实现了 IPlugin 接口。
 * 
 * 功能说明：
 * - 插件加载时输出 Hello World 信息
 * - 提供 getGreeting() 方法返回问候语
 * - 展示插件的完整生命周期
 * 
 * 使用示例：
 * @code
 * // 插件会被 PluginManager 自动加载
 * // 加载后可以通过插件管理器获取
 * IPlugin* plugin = pluginManager.getPlugin("HelloWorld");
 * if (plugin) {
 *     qDebug() << plugin->getName(); // 输出 "HelloWorld"
 * }
 * @endcode
 * 
 * 注意：
 * - 插件类必须继承 QObject 和 IPlugin
 * - 必须添加 Q_OBJECT 宏
 * - 必须添加 Q_INTERFACES(IPlugin) 宏
 * - 必须添加 Q_PLUGIN_METADATA 宏
 */
class HelloWorldPlugin : public QObject, public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)
    Q_PLUGIN_METADATA(IID "com.starretrospect.IPlugin/1.0" FILE "helloworld.json")
    
public:
    /**
     * @brief 构造函数
     */
    explicit HelloWorldPlugin(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~HelloWorldPlugin() override;
    
    // ============================================================
    // IPlugin 接口实现
    // ============================================================
    
    /**
     * @brief 获取插件名称
     * @return 插件名称
     */
    QString getName() const override;
    
    /**
     * @brief 获取插件版本
     * @return 版本号
     */
    QString getVersion() const override;
    
    /**
     * @brief 获取插件作者
     * @return 作者
     */
    QString getAuthor() const override;
    
    /**
     * @brief 获取插件描述
     * @return 描述
     */
    QString getDescription() const override;
    
    /**
     * @brief 初始化插件
     * 
     * 插件加载后会调用此方法。
     * 在这里输出 Hello World 信息。
     * 
     * @return true 初始化成功
     */
    bool initialize() override;
    
    /**
     * @brief 关闭插件
     * 
     * 插件卸载前会调用此方法。
     * 在这里输出再见信息。
     */
    void shutdown() override;
    
    // ============================================================
    // 插件特有功能
    // ============================================================
    
    /**
     * @brief 获取问候语
     * 
     * 这是插件特有的功能，返回一个问候语。
     * 
     * @param name 问候对象的名称
     * @return 问候语
     */
    QString getGreeting(const QString& name = "旅者") const;
    
private:
    // 插件内部状态
    int m_greetingCount = 0;  ///< 问候次数统计
};

#endif // HELLO_WORLD_PLUGIN_H
