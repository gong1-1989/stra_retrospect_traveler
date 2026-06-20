/**
 * @file ExampleUIPlugin.cpp
 * @brief 示例UI插件实现
 * 
 * 本文件实现了示例UI插件的所有功能。
 * 
 * 这个插件演示了如何：
 * - 实现 IUIContentPlugin 接口
 * - 创建自定义UI控件
 * - 扩展游戏菜单和HUD
 * - 提供设置面板
 * - 自定义主题样式
 * 
 * @author 星溯旅人开发团队
 * @version 1.0.0
 * @date 2026
 */
#include "ExampleUIPlugin.h"
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

// ============================================================
// 构造函数与析构函数
// ============================================================

ExampleUIPlugin::ExampleUIPlugin()
    : m_initialized(false)
    , m_currentTheme("default")
{
    qDebug() << "[ExampleUIPlugin] 创建示例UI插件";
}

ExampleUIPlugin::~ExampleUIPlugin()
{
    qDebug() << "[ExampleUIPlugin] 销毁示例UI插件";
}

// ============================================================
// IPlugin 接口实现
// ============================================================

QString ExampleUIPlugin::getName() const
{
    return "ExampleUIPlugin";
}

QString ExampleUIPlugin::getVersion() const
{
    return "1.0.0";
}

QString ExampleUIPlugin::getAuthor() const
{
    return "星溯旅人开发团队";
}

QString ExampleUIPlugin::getDescription() const
{
    return "示例UI插件，演示UI内容插件的开发方法。"
           "提供自定义控件、菜单扩展、HUD元素、设置面板等功能。";
}

bool ExampleUIPlugin::initialize()
{
    if (m_initialized) {
        return true;
    }
    
    qDebug() << "[ExampleUIPlugin] 初始化示例UI插件...";
    
    m_initialized = true;
    
    qDebug() << "[ExampleUIPlugin] 示例UI插件初始化完成";
    return true;
}

void ExampleUIPlugin::shutdown()
{
    if (!m_initialized) {
        return;
    }
    
    qDebug() << "[ExampleUIPlugin] 关闭示例UI插件...";
    
    m_initialized = false;
    
    qDebug() << "[ExampleUIPlugin] 示例UI插件已关闭";
}

// ============================================================
// 自定义控件
// ============================================================

QWidget* ExampleUIPlugin::createCustomWidget(const QString& widgetId, QWidget* parent)
{
    if (widgetId == "example_label") {
        QLabel* label = new QLabel("示例控件 - 来自 ExampleUIPlugin", parent);
        label->setStyleSheet("color: blue; font-size: 16px; font-weight: bold;");
        return label;
    }
    
    if (widgetId == "example_button") {
        QPushButton* button = new QPushButton("示例按钮", parent);
        button->setStyleSheet("background-color: #4CAF50; color: white; padding: 10px;");
        return button;
    }
    
    qWarning() << "[ExampleUIPlugin] 不支持的控件ID:" << widgetId;
    return nullptr;
}

QStringList ExampleUIPlugin::getSupportedWidgets() const
{
    return QStringList() << "example_label" << "example_button";
}

// ============================================================
// 菜单扩展
// ============================================================

QList<QAction*> ExampleUIPlugin::getMenuActions(const QString& menuId)
{
    QList<QAction*> actions;
    
    if (menuId == "help_menu") {
        QAction* aboutAction = new QAction("关于示例插件", nullptr);
        QObject::connect(aboutAction, &QAction::triggered, []() {
            QMessageBox::information(nullptr, "关于示例插件", 
                "示例UI插件 v1.0.0\n"
                "演示UI插件开发方法\n"
                "作者：星溯旅人开发团队");
        });
        actions.append(aboutAction);
    }
    
    return actions;
}

QWidget* ExampleUIPlugin::createMenu(const QString& menuId, QWidget* parent)
{
    Q_UNUSED(menuId);
    Q_UNUSED(parent);
    
    // 示例：不创建自定义菜单
    return nullptr;
}

QStringList ExampleUIPlugin::getSupportedMenus() const
{
    return QStringList();
}

// ============================================================
// HUD扩展
// ============================================================

QList<QWidget*> ExampleUIPlugin::getHUDElements(const QString& hudLayer)
{
    QList<QWidget*> elements;
    
    if (hudLayer == "top_layer") {
        QLabel* fpsLabel = new QLabel("FPS: --");
        fpsLabel->setStyleSheet("color: white; background-color: rgba(0,0,0,150); padding: 5px;");
        fpsLabel->setObjectName("plugin_fps_label");
        elements.append(fpsLabel);
    }
    
    return elements;
}

QStringList ExampleUIPlugin::getSupportedHUDLayers() const
{
    return QStringList() << "top_layer";
}

// ============================================================
// 设置面板
// ============================================================

QWidget* ExampleUIPlugin::createSettingsPanel(QWidget* parent)
{
    QWidget* panel = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(panel);
    
    QLabel* titleLabel = new QLabel("示例UI插件设置", panel);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    layout->addWidget(titleLabel);
    
    QLabel* descLabel = new QLabel("这是一个示例设置面板，演示如何在插件中提供设置界面。", panel);
    descLabel->setWordWrap(true);
    layout->addWidget(descLabel);
    
    QPushButton* testButton = new QPushButton("测试按钮", panel);
    layout->addWidget(testButton);
    
    layout->addStretch();
    
    return panel;
}

QString ExampleUIPlugin::getSettingsCategory() const
{
    return "插件设置";
}

bool ExampleUIPlugin::hasSettingsPanel() const
{
    return true;
}

// ============================================================
// 对话框
// ============================================================

int ExampleUIPlugin::showDialog(const QString& dialogId, QWidget* parent)
{
    if (dialogId == "example_about") {
        QMessageBox::information(parent, "关于示例插件", 
            "示例UI插件 v1.0.0\n"
            "演示UI插件开发方法\n"
            "作者：星溯旅人开发团队");
        return QMessageBox::Ok;
    }
    
    qWarning() << "[ExampleUIPlugin] 不支持的对话框ID:" << dialogId;
    return -1;
}

QStringList ExampleUIPlugin::getSupportedDialogs() const
{
    return QStringList() << "example_about";
}

// ============================================================
// 主题和样式
// ============================================================

QString ExampleUIPlugin::getGlobalStylesheet() const
{
    // 示例样式表
    return QString(
        "QPushButton#plugin_button {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "    padding: 10px 20px;"
        "}"
        "QPushButton#plugin_button:hover {"
        "    background-color: #45a049;"
        "}"
    );
}

QString ExampleUIPlugin::getThemeName() const
{
    return "示例主题";
}

// ============================================================
// UI事件回调
// ============================================================

void ExampleUIPlugin::onUIInitialized()
{
    qDebug() << "[ExampleUIPlugin] UI初始化完成";
}

void ExampleUIPlugin::onThemeChanged(const QString& themeName)
{
    m_currentTheme = themeName;
    qDebug() << "[ExampleUIPlugin] 主题变化:" << themeName;
}

void ExampleUIPlugin::onLanguageChanged(const QString& language)
{
    qDebug() << "[ExampleUIPlugin] 语言变化:" << language;
}
