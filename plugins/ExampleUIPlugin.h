/**
 * @file ExampleUIPlugin.h
 * @brief 示例UI插件头文件
 * 
 * 本文件定义了一个示例UI插件，
 * 演示如何实现 IUIContentPlugin 接口。
 * 
 * 这个插件展示了以下功能：
 * - 自定义控件创建
 * - 菜单扩展
 * - HUD扩展
 * - 设置面板
 * - 对话框
 * - 主题和样式
 * 
 * @author 星溯旅人开发团队
 * @version 1.0.0
 * @date 2026
 */
#ifndef EXAMPLE_UI_PLUGIN_H
#define EXAMPLE_UI_PLUGIN_H

#include "IUIContentPlugin.h"
#include <QString>
#include <QWidget>
#include <QAction>

/**
 * @class ExampleUIPlugin
 * @brief 示例UI插件类
 * 
 * 这个类是一个示例UI插件，
 * 演示了如何实现 IUIContentPlugin 接口。
 * 
 * 插件功能：
 * - 提供自定义控件
 * - 扩展游戏菜单
 * - 添加HUD元素
 * - 提供设置面板
 * - 自定义主题样式
 * 
 * 使用场景：
 * - 学习UI插件开发
 * - 测试UI插件系统
 * - 作为UI插件开发模板
 */
class ExampleUIPlugin : public IUIContentPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.starretrospect.IUIContentPlugin" FILE "example_ui_plugin.json")
    Q_INTERFACES(IUIContentPlugin IPlugin)
    
public:
    // ============================================================
    // 构造函数与析构函数
    // ============================================================
    
    /**
     * @brief 构造函数
     */
    ExampleUIPlugin();
    
    /**
     * @brief 析构函数
     */
    virtual ~ExampleUIPlugin();
    
    // ============================================================
    // IPlugin 接口实现
    // ============================================================
    
    QString getName() const override;
    QString getVersion() const override;
    QString getAuthor() const override;
    QString getDescription() const override;
    bool initialize() override;
    void shutdown() override;
    
    // ============================================================
    // 自定义控件
    // ============================================================
    
    /**
     * @brief 创建自定义控件
     * 
     * 根据控件ID创建自定义控件。
     * 
     * @param widgetId 控件ID
     * @param parent 父控件
     * @return 创建的控件，如果不支持返回 nullptr
     */
    QWidget* createCustomWidget(const QString& widgetId, QWidget* parent) override;
    
    /**
     * @brief 获取支持的控件ID列表
     * 
     * @return 控件ID列表
     */
    QStringList getSupportedWidgets() const override;
    
    // ============================================================
    // 菜单扩展
    // ============================================================
    
    /**
     * @brief 获取菜单动作
     * 
     * 获取指定菜单的扩展动作列表。
     * 
     * @param menuId 菜单ID
     * @return 动作列表
     */
    QList<QAction*> getMenuActions(const QString& menuId) override;
    
    /**
     * @brief 创建自定义菜单
     * 
     * 创建自定义菜单控件。
     * 
     * @param menuId 菜单ID
     * @param parent 父控件
     * @return 菜单控件，如果不支持返回 nullptr
     */
    QWidget* createMenu(const QString& menuId, QWidget* parent) override;
    
    /**
     * @brief 获取支持的菜单ID
     * 
     * @return 菜单ID列表
     */
    QStringList getSupportedMenus() const override;
    
    // ============================================================
    // HUD扩展
    // ============================================================
    
    /**
     * @brief 获取HUD元素
     * 
     * 获取指定层的HUD元素。
     * 
     * @param hudLayer HUD层
     * @return HUD元素列表
     */
    QList<QWidget*> getHUDElements(const QString& hudLayer) override;
    
    /**
     * @brief 获取支持的HUD层
     * 
     * @return HUD层列表
     */
    QStringList getSupportedHUDLayers() const override;
    
    // ============================================================
    // 设置面板
    // ============================================================
    
    /**
     * @brief 创建设置面板
     * 
     * 创建插件的设置面板。
     * 
     * @param parent 父控件
     * @return 设置面板控件
     */
    QWidget* createSettingsPanel(QWidget* parent) override;
    
    /**
     * @brief 获取设置分类
     * 
     * @return 设置分类名称
     */
    QString getSettingsCategory() const override;
    
    /**
     * @brief 是否提供设置面板
     * 
     * @return true 提供设置面板
     */
    bool hasSettingsPanel() const override;
    
    // ============================================================
    // 对话框
    // ============================================================
    
    /**
     * @brief 显示对话框
     * 
     * 显示指定ID的对话框。
     * 
     * @param dialogId 对话框ID
     * @param parent 父控件
     * @return 对话框返回值
     */
    int showDialog(const QString& dialogId, QWidget* parent) override;
    
    /**
     * @brief 获取支持的对话框ID
     * 
     * @return 对话框ID列表
     */
    QStringList getSupportedDialogs() const override;
    
    // ============================================================
    // 主题和样式
    // ============================================================
    
    /**
     * @brief 获取全局样式表
     * 
     * 获取插件提供的全局样式表。
     * 
     * @return 样式表字符串
     */
    QString getGlobalStylesheet() const override;
    
    /**
     * @brief 获取主题名称
     * 
     * @return 主题名称
     */
    QString getThemeName() const override;
    
    // ============================================================
    // UI事件回调
    // ============================================================
    
    /**
     * @brief UI初始化完成回调
     */
    void onUIInitialized() override;
    
    /**
     * @brief 主题变化回调
     * 
     * @param themeName 新主题名称
     */
    void onThemeChanged(const QString& themeName) override;
    
    /**
     * @brief 语言变化回调
     * 
     * @param language 新语言
     */
    void onLanguageChanged(const QString& language) override;
    
private:
    // ============================================================
    // 私有成员变量
    // ============================================================
    
    bool m_initialized;          ///< 是否已初始化
    QString m_currentTheme;      ///< 当前主题
};

#endif // EXAMPLE_UI_PLUGIN_H
