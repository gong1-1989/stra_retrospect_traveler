/**
 * @file UIPlugin.cpp
 * @brief UI 插件示例实现 - 星空主题 HUD
 * 
 * 本文件实现了 UI 插件的所有功能。
 * 展示如何通过插件系统自定义游戏界面。
 * 
 * @author 星溯旅人开发团队
 * @version 2.0.0
 * @date 2026
 */
#include "UIPlugin.h"
#include <QDebug>

// ============================================================
// 构造函数与析构函数
// ============================================================

UIPlugin::UIPlugin(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[UIPlugin] UI 插件对象创建";
}

UIPlugin::~UIPlugin()
{
    qDebug() << "[UIPlugin] UI 插件对象销毁";
}

// ============================================================
// IPlugin 接口实现
// ============================================================

QString UIPlugin::getName() const
{
    return "StarryUI";
}

QString UIPlugin::getVersion() const
{
    return "2.0.0";
}

QString UIPlugin::getAuthor() const
{
    return "星溯旅人开发团队";
}

QString UIPlugin::getDescription() const
{
    return "星空主题 UI 扩展：自定义 HUD、主题切换、通知系统";
}

bool UIPlugin::initialize()
{
    qDebug() << "========================================";
    qDebug() << "  🌟 星空主题 UI 插件加载中...";
    qDebug() << "========================================";
    
    // 初始化主题
    initializeThemes();
    
    // 初始化 HUD 元素
    initializeHUDElements();
    
    // 设置默认主题
    m_currentThemeId = "starry";
    
    // 输出加载统计
    qDebug() << "[UIPlugin] 主题加载完成: " << m_themes.size() << "个";
    qDebug() << "[UIPlugin] HUD元素加载完成: " << m_hudElements.size() << "个";
    qDebug() << "[UIPlugin] 当前主题: " << getCurrentTheme().name;
    
    qDebug() << "========================================";
    qDebug() << "  ✅ 星空主题 UI 插件加载完成！";
    qDebug() << "========================================";
    
    return true;
}

void UIPlugin::shutdown()
{
    qDebug() << "[UIPlugin] 卸载星空主题 UI 插件...";
    
    // 清理所有数据
    m_themes.clear();
    m_hudElements.clear();
    m_currentThemeId.clear();
    m_notificationCount = 0;
    
    qDebug() << "[UIPlugin] UI 插件已卸载";
}

// ============================================================
// UI 插件特有功能
// ============================================================

QList<UITheme> UIPlugin::getThemes() const
{
    return m_themes.values();
}

UITheme UIPlugin::getTheme(const QString& themeId) const
{
    return m_themes.value(themeId, UITheme());
}

UITheme UIPlugin::getCurrentTheme() const
{
    return m_themes.value(m_currentThemeId, UITheme());
}

bool UIPlugin::switchTheme(const QString& themeId)
{
    if (!m_themes.contains(themeId)) {
        qWarning() << "[UIPlugin] 主题不存在:" << themeId;
        return false;
    }
    
    m_currentThemeId = themeId;
    qDebug() << "[UIPlugin] 切换到主题:" << m_themes[themeId].name;
    
    return true;
}

QList<HUDElement> UIPlugin::getHUDElements() const
{
    return m_hudElements.values();
}

void UIPlugin::showNotification(const QString& title, const QString& message, 
                                const QString& type)
{
    m_notificationCount++;
    
    qDebug() << "========================================";
    qDebug() << "  🔔 通知 #" << m_notificationCount;
    qDebug() << "  类型: " << type;
    qDebug() << "  标题: " << title;
    qDebug() << "  内容: " << message;
    qDebug() << "========================================";
}

// ============================================================
// 私有初始化方法
// ============================================================

void UIPlugin::initializeThemes()
{
    qDebug() << "[UIPlugin] 初始化 UI 主题...";
    
    // 主题1：星空主题（默认）
    UITheme starry;
    starry.id = "starry";
    starry.name = "星空主题";
    starry.description = "深邃的星空背景，搭配闪烁的星光效果，营造神秘的太空氛围。";
    starry.primaryColor = QColor(100, 150, 255);    // 星空蓝
    starry.secondaryColor = QColor(200, 100, 255);  // 星云紫
    starry.backgroundColor = QColor(10, 15, 40);    // 深夜蓝
    starry.textColor = QColor(240, 240, 255);       // 浅白
    starry.accentColor = QColor(255, 215, 0);       // 星光金
    starry.fontFamily = "Microsoft YaHei";
    starry.fontSize = 14;
    starry.opacity = 0.9f;
    starry.borderRadius = 8;
    m_themes[starry.id] = starry;
    
    // 主题2：晨曦主题
    UITheme dawn;
    dawn.id = "dawn";
    dawn.name = "晨曦主题";
    dawn.description = "温暖的晨曦色调，给人希望和新生的感觉。";
    dawn.primaryColor = QColor(255, 180, 100);      // 晨曦橙
    dawn.secondaryColor = QColor(255, 150, 180);    // 朝霞粉
    dawn.backgroundColor = QColor(40, 30, 50);      // 黎明灰
    dawn.textColor = QColor(255, 250, 240);         // 暖白
    dawn.accentColor = QColor(255, 200, 100);       // 金光
    dawn.fontFamily = "Microsoft YaHei";
    dawn.fontSize = 14;
    dawn.opacity = 0.9f;
    dawn.borderRadius = 8;
    m_themes[dawn.id] = dawn;
    
    // 主题3：深海主题
    UITheme deepsea;
    deepsea.id = "deepsea";
    deepsea.name = "深海主题";
    deepsea.description = "神秘的深海色调，带来宁静和深邃的感觉。";
    deepsea.primaryColor = QColor(0, 180, 200);     // 深海青
    deepsea.secondaryColor = QColor(0, 120, 180);   // 海洋蓝
    deepsea.backgroundColor = QColor(5, 20, 30);    // 深海黑
    deepsea.textColor = QColor(200, 240, 255);      // 浅青
    deepsea.accentColor = QColor(0, 255, 200);      // 荧光绿
    deepsea.fontFamily = "Microsoft YaHei";
    deepsea.fontSize = 14;
    deepsea.opacity = 0.9f;
    deepsea.borderRadius = 8;
    m_themes[deepsea.id] = deepsea;
    
    // 主题4：极简主题
    UITheme minimal;
    minimal.id = "minimal";
    minimal.name = "极简主题";
    minimal.description = "简洁的黑白灰配色，专注于游戏内容本身。";
    minimal.primaryColor = QColor(200, 200, 200);   // 浅灰
    minimal.secondaryColor = QColor(150, 150, 150); // 中灰
    minimal.backgroundColor = QColor(20, 20, 20);   // 深黑
    minimal.textColor = QColor(230, 230, 230);      // 浅白
    minimal.accentColor = QColor(255, 255, 255);    // 纯白
    minimal.fontFamily = "Microsoft YaHei";
    minimal.fontSize = 14;
    minimal.opacity = 0.85f;
    minimal.borderRadius = 4;
    m_themes[minimal.id] = minimal;
    
    qDebug() << "[UIPlugin] 已添加" << m_themes.size() << "个 UI 主题";
}

void UIPlugin::initializeHUDElements()
{
    qDebug() << "[UIPlugin] 初始化 HUD 元素...";
    
    // 元素1：星象指示器
    HUDElement starIndicator;
    starIndicator.id = "star_indicator";
    starIndicator.name = "星象指示器";
    starIndicator.type = "icon";
    starIndicator.position = "top-right";
    starIndicator.visible = true;
    starIndicator.priority = 10;
    m_hudElements[starIndicator.id] = starIndicator;
    
    // 元素2：时间熵显示
    HUDElement timeEntropy;
    timeEntropy.id = "time_entropy_bar";
    timeEntropy.name = "时间熵进度条";
    timeEntropy.type = "progress";
    timeEntropy.position = "top-left";
    timeEntropy.visible = true;
    timeEntropy.priority = 8;
    m_hudElements[timeEntropy.id] = timeEntropy;
    
    // 元素3：共鸣状态
    HUDElement resonanceStatus;
    resonanceStatus.id = "resonance_status";
    resonanceStatus.name = "星核共鸣状态";
    resonanceStatus.type = "text";
    resonanceStatus.position = "bottom-left";
    resonanceStatus.visible = true;
    resonanceStatus.priority = 7;
    m_hudElements[resonanceStatus.id] = resonanceStatus;
    
    // 元素4：快捷操作面板
    HUDElement quickActions;
    quickActions.id = "quick_actions";
    quickActions.name = "快捷操作面板";
    quickActions.type = "panel";
    quickActions.position = "bottom-right";
    quickActions.visible = true;
    quickActions.priority = 5;
    m_hudElements[quickActions.id] = quickActions;
    
    // 元素5：生态指数
    HUDElement ecoIndex;
    ecoIndex.id = "eco_index";
    ecoIndex.name = "生态指数";
    ecoIndex.type = "gauge";
    ecoIndex.position = "bottom-left";
    ecoIndex.visible = false;
    ecoIndex.priority = 6;
    m_hudElements[ecoIndex.id] = ecoIndex;
    
    qDebug() << "[UIPlugin] 已添加" << m_hudElements.size() << "个 HUD 元素";
}
