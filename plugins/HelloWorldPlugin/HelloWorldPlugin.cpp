/**
 * @file HelloWorldPlugin.cpp
 * @brief HelloWorld 示例插件实现
 * 
 * 本文件实现了 HelloWorld 插件的所有功能。
 * 这是一个最简单的插件示例，用于学习和测试插件系统。
 * 
 * @author 星溯旅人开发团队
 * @version 1.0.0
 * @date 2026
 */
#include "HelloWorldPlugin.h"
#include <QDebug>

// ============================================================
// 构造函数与析构函数
// ============================================================

/**
 * @brief 构造函数
 * 
 * 创建 HelloWorld 插件实例。
 * 注意：插件对象创建时还没有被初始化，
 * 真正的初始化工作在 initialize() 中完成。
 * 
 * @param parent 父对象
 */
HelloWorldPlugin::HelloWorldPlugin(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[HelloWorldPlugin] 插件对象创建";
}

/**
 * @brief 析构函数
 * 
 * 销毁 HelloWorld 插件实例。
 * 注意：析构时插件应该已经被关闭（shutdown 已调用）。
 */
HelloWorldPlugin::~HelloWorldPlugin()
{
    qDebug() << "[HelloWorldPlugin] 插件对象销毁";
}

// ============================================================
// IPlugin 接口实现
// ============================================================

/**
 * @brief 获取插件名称
 * 
 * 返回插件的唯一标识符名称。
 * 插件名称用于在插件管理器中查找和识别插件。
 * 
 * @return 插件名称 "HelloWorld"
 */
QString HelloWorldPlugin::getName() const
{
    return "HelloWorld";
}

/**
 * @brief 获取插件版本
 * 
 * 返回插件的版本号。
 * 使用语义化版本（Semantic Versioning）格式。
 * 
 * @return 版本号 "1.0.0"
 */
QString HelloWorldPlugin::getVersion() const
{
    return "1.0.0";
}

/**
 * @brief 获取插件作者
 * 
 * 返回插件的作者信息。
 * 
 * @return 作者 "星溯旅人开发团队"
 */
QString HelloWorldPlugin::getAuthor() const
{
    return "星溯旅人开发团队";
}

/**
 * @brief 获取插件描述
 * 
 * 返回插件的功能描述。
 * 
 * @return 插件描述
 */
QString HelloWorldPlugin::getDescription() const
{
    return "一个简单的 HelloWorld 示例插件，用于学习和测试插件系统";
}

/**
 * @brief 初始化插件
 * 
 * 插件加载后会调用此方法进行初始化。
 * 在这里输出 Hello World 信息，表示插件成功加载。
 * 
 * 执行以下操作：
 * 1. 输出插件加载成功信息
 * 2. 输出 Hello World 问候语
 * 3. 返回 true 表示初始化成功
 * 
 * @return true 初始化成功
 */
bool HelloWorldPlugin::initialize()
{
    qDebug() << "========================================";
    qDebug() << "  Hello World! 👋";
    qDebug() << "  来自 HelloWorld 插件的问候";
    qDebug() << "========================================";
    qDebug() << "[HelloWorldPlugin] 插件初始化成功";
    
    return true;
}

/**
 * @brief 关闭插件
 * 
 * 插件卸载前会调用此方法进行清理。
 * 在这里输出再见信息，表示插件即将卸载。
 * 
 * 执行以下操作：
 * 1. 输出插件关闭信息
 * 2. 输出再见问候语
 */
void HelloWorldPlugin::shutdown()
{
    qDebug() << "[HelloWorldPlugin] 插件关闭，再见！👋";
    qDebug() << "========================================";
    qDebug() << "  Goodbye! 来自 HelloWorld 插件";
    qDebug() << "========================================";
}

// ============================================================
// 插件特有功能
// ============================================================

/**
 * @brief 获取问候语
 * 
 * 这是插件特有的功能，返回一个个性化的问候语。
 * 每次调用都会增加问候次数统计。
 * 
 * @param name 问候对象的名称，默认为 "旅者"
 * @return 个性化的问候语
 */
QString HelloWorldPlugin::getGreeting(const QString& name) const
{
    // 注意：这里用 const_cast 是为了演示，实际项目中应该用 mutable 或其他方式
    const_cast<HelloWorldPlugin*>(this)->m_greetingCount++;
    
    return QString("你好，%1！欢迎来到星溯旅人世界 ✨").arg(name);
}
