/**
 * @file UIPlugin.h
 * @brief UI 插件示例 - 星空主题 HUD
 * 
 * 这是一个 UI 插件示例，展示如何通过插件自定义游戏界面。
 * 
 * 插件功能：
 * - 提供星空主题的 HUD 样式
 * - 添加自定义的状态栏
 * - 添加快捷操作面板
 * - 自定义通知样式
 * 
 * 这个插件展示了如何通过插件系统扩展游戏的 UI 界面。
 * 
 * @author 星溯旅人开发团队
 * @version 2.0.0
 * @date 2026
 */
#ifndef UI_PLUGIN_H
#define UI_PLUGIN_H

#include <QObject>
#include <QString>
#include <QColor>
#include <QMap>
#include <QList>
#include "../PluginManager.h"

/**
 * @brief UI 主题结构
 * 
 * 定义 UI 主题的颜色和样式。
 */
struct UITheme {
    QString id;           ///< 主题ID
    QString name;         ///< 主题名称
    QString description;  ///< 主题描述
    
    // 颜色配置
    QColor primaryColor;    ///< 主色调
    QColor secondaryColor;  ///< 辅助色
    QColor backgroundColor; ///< 背景色
    QColor textColor;       ///< 文字颜色
    QColor accentColor;     ///< 强调色
    
    // 字体配置
    QString fontFamily;   ///< 字体族
    int fontSize;         ///< 基础字号
    
    // 其他配置
    float opacity;        ///< 透明度
    int borderRadius;     ///< 圆角半径
};

/**
 * @brief HUD 元素结构
 * 
 * 定义 HUD 上的自定义元素。
 */
struct HUDElement {
    QString id;           ///< 元素ID
    QString name;         ///< 元素名称
    QString type;         ///< 元素类型（文本/进度条/图标等）
    QString position;     ///< 位置（top-left/top-right/bottom-left/bottom-right）
    bool visible;         ///< 是否可见
    int priority;         ///< 显示优先级
};

/**
 * @brief UI 插件类 - 星空主题 HUD
 * 
 * 这是一个 UI 插件示例，展示如何通过插件自定义游戏界面。
 * 
 * 功能说明：
 * - 提供星空主题的 UI 样式
 * - 添加自定义的 HUD 元素
 * - 自定义通知系统样式
 * - 提供快捷操作面板
 * 
 * 使用示例：
 * @code
 * // 插件加载后，可以通过插件管理器获取
 * UIPlugin* plugin = qobject_cast<UIPlugin*>(
 *     pluginManager.getPlugin("StarryUI")
 * );
 * 
 * if (plugin) {
 *     // 应用星空主题
 *     UITheme theme = plugin->getTheme("starry");
 *     
 *     // 获取自定义 HUD 元素
 *     QList<HUDElement> elements = plugin->getHUDElements();
 * }
 * @endcode
 */
class UIPlugin : public QObject, public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)
    Q_PLUGIN_METADATA(IID "com.starretrospect.IPlugin/1.0" FILE "ui.json")
    
public:
    /**
     * @brief 构造函数
     */
    explicit UIPlugin(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~UIPlugin() override;
    
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
    // UI 插件特有功能
    // ============================================================
    
    /**
     * @brief 获取所有可用主题
     * @return 主题列表
     */
    QList<UITheme> getThemes() const;
    
    /**
     * @brief 根据ID获取主题
     * @param themeId 主题ID
     * @return 主题信息，不存在时返回空
     */
    UITheme getTheme(const QString& themeId) const;
    
    /**
     * @brief 获取当前激活的主题
     * @return 当前主题
     */
    UITheme getCurrentTheme() const;
    
    /**
     * @brief 切换主题
     * @param themeId 主题ID
     * @return true 切换成功
     */
    bool switchTheme(const QString& themeId);
    
    /**
     * @brief 获取所有 HUD 元素
     * @return HUD 元素列表
     */
    QList<HUDElement> getHUDElements() const;
    
    /**
     * @brief 显示自定义通知
     * @param title 标题
     * @param message 消息
     * @param type 类型（info/warning/error/success）
     */
    void showNotification(const QString& title, const QString& message, 
                         const QString& type = "info");
    
    /**
     * @brief 获取主题数量
     * @return 主题数量
     */
    int getThemeCount() const { return m_themes.size(); }
    
    /**
     * @brief 获取 HUD 元素数量
     * @return HUD 元素数量
     */
    int getHUDElementCount() const { return m_hudElements.size(); }
    
private:
    /**
     * @brief 初始化主题
     * 
     * 注册所有 UI 主题。
     */
    void initializeThemes();
    
    /**
     * @brief 初始化 HUD 元素
     * 
     * 注册所有自定义 HUD 元素。
     */
    void initializeHUDElements();
    
    // ============================================================
    // 成员变量
    // ============================================================
    
    // 主题映射（ID -> 主题信息）
    QMap<QString, UITheme> m_themes;
    
    // HUD 元素映射（ID -> 元素信息）
    QMap<QString, HUDElement> m_hudElements;
    
    // 当前激活的主题ID
    QString m_currentThemeId;
    
    // 通知计数
    int m_notificationCount = 0;
};

#endif // UI_PLUGIN_H
