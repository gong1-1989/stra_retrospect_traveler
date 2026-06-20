/**
 * @file UIManager.cpp
 * @brief UI 管理器类实现
 * 
 * 本文件实现了 UI 管理器的所有功能，包括：
 * - UI 界面的创建、显示和隐藏
 * - UI 界面的切换
 * - HUD（抬头显示）的更新
 * - 对话框系统
 * - 通知系统
 * 
 * UI 管理器基于 Qt Widgets，
 * 使用 QWidget 作为 UI 容器，管理游戏中的所有用户界面。
 * 
 * 主菜单实现：
 * - 游戏标题（星溯旅人）
 * - 开始游戏按钮
 * - 继续游戏按钮
 * - 设置按钮
 * - 退出游戏按钮
 * 
 * 其他 UI 目前是占位实现，后续需要完善：
 * - 游戏 HUD
 * - 暂停菜单
 * - 背包界面
 * - 技能树界面
 * - 对话框
 * - 设置界面
 * - 游戏结束界面
 * - 胜利界面
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#include "UIManager.h"

#include <QDebug>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStringList>

// ============================================================
// 构造函数与析构函数
// ============================================================

/**
 * @brief 构造函数
 * 
 * 创建 UI 管理器实例，但不进行任何初始化操作。
 * 所有的初始化工作都在 initialize() 中完成。
 * 
 * @param parent 父对象，用于 Qt 对象树内存管理
 */
UIManager::UIManager(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[UIManager] 创建 UI 管理器";
}

/**
 * @brief 析构函数
 * 
 * 销毁 UI 管理器，自动调用 shutdown() 清理所有 UI。
 */
UIManager::~UIManager()
{
    qDebug() << "[UIManager] 销毁 UI 管理器";
    shutdown();
}

// ============================================================
// 生命周期管理
// ============================================================

/**
 * @brief 初始化 UI 系统
 * 
 * 执行以下初始化操作：
 * 1. 检查是否已经初始化（防止重复初始化）
 * 2. 验证父窗口指针是否有效
 * 3. 创建所有默认 UI 界面
 * 
 * @param parentWidget 父窗口指针（所有 UI 的父窗口）
 * @return true 初始化成功
 * @return false 初始化失败（父窗口指针无效）
 */
bool UIManager::initialize(QWidget* parentWidget)
{
    // 防止重复初始化
    if (m_initialized) {
        qWarning() << "[UIManager] UI 管理器已经初始化，跳过重复初始化";
        return true;
    }

    // 验证父窗口指针
    if (!parentWidget) {
        qCritical() << "[UIManager] 无效的父窗口指针";
        return false;
    }

    qDebug() << "[UIManager] 初始化 UI 管理器...";

    // 1. 保存父窗口指针
    m_parentWidget = parentWidget;

    // 2. 创建所有默认 UI 界面
    qDebug() << "[UIManager] 创建默认 UI 界面";
    createDefaultUIs();

    // 初始化完成
    m_initialized = true;
    
    qDebug() << "[UIManager] UI 管理器初始化完成";
    return true;
}

/**
 * @brief 关闭 UI 系统
 * 
 * 执行以下清理操作：
 * 1. 清理所有 UI 界面（自动删除 QWidget 对象）
 * 2. 重置父窗口指针（不删除父窗口，因为不是我们创建的）
 * 3. 重置状态
 * 
 * 注意：不会删除父窗口，因为它不是 UI 管理器创建的。
 * 
 * 此函数是幂等的，可以安全地多次调用。
 */
void UIManager::shutdown()
{
    // 如果没有初始化，直接返回
    if (!m_initialized) {
        return;
    }

    qDebug() << "[UIManager] 关闭 UI 管理器...";

    // 1. 清理所有 UI 界面
    qDebug() << "[UIManager] 清理所有 UI 界面，数量:" << m_uis.size();
    qDeleteAll(m_uis);
    m_uis.clear();

    // 2. 重置父窗口指针（不删除父窗口）
    m_parentWidget = nullptr;

    // 3. 重置状态
    m_initialized = false;
    
    qDebug() << "[UIManager] UI 管理器已关闭";
}

/**
 * @brief 更新 UI
 * 
 * 每帧调用一次，更新 UI 状态。
 * 可以在这里处理动画、计时器等需要每帧更新的 UI 元素。
 * 
 * 当前实现是空的，因为大部分 UI 不需要每帧更新。
 * 如果需要添加动画效果，可以在这里实现。
 * 
 * @param deltaTime 时间增量（秒）
 */
void UIManager::update(float deltaTime)
{
    Q_UNUSED(deltaTime);
    
    // 如果没有初始化，直接返回
    if (!m_initialized) {
        return;
    }

    // UI 更新逻辑
    // 可以在这里处理动画、计时器等需要每帧更新的 UI 元素
}

// ============================================================
// UI 管理
// ============================================================

/**
 * @brief 显示 UI
 * 
 * 显示指定类型的 UI 界面。
 * 如果 UI 不存在，返回 false。
 * 
 * 显示成功后会发出 uiShown 信号。
 * 
 * @param type UI 类型
 * @return true 显示成功
 * @return false UI 不存在
 */
bool UIManager::showUI(UIType type)
{
    // 检查 UI 是否存在
    if (!m_uis.contains(type)) {
        qWarning() << "[UIManager] UI 不存在:" << type;
        return false;
    }

    QWidget* ui = m_uis[type];
    if (ui) {
        // 显示 UI
        ui->show();
        
        // 发出 UI 显示信号
        emit uiShown(type);
        
        qDebug() << "[UIManager] 显示 UI:" << type;
    }
    
    return true;
}

/**
 * @brief 隐藏 UI
 * 
 * 隐藏指定类型的 UI 界面。
 * 如果 UI 不存在，返回 false。
 * 
 * 隐藏成功后会发出 uiHidden 信号。
 * 
 * @param type UI 类型
 * @return true 隐藏成功
 * @return false UI 不存在
 */
bool UIManager::hideUI(UIType type)
{
    // 检查 UI 是否存在
    if (!m_uis.contains(type)) {
        qWarning() << "[UIManager] UI 不存在:" << type;
        return false;
    }

    QWidget* ui = m_uis[type];
    if (ui) {
        // 隐藏 UI
        ui->hide();
        
        // 发出 UI 隐藏信号
        emit uiHidden(type);
        
        qDebug() << "[UIManager] 隐藏 UI:" << type;
    }
    
    return true;
}

/**
 * @brief 切换 UI
 * 
 * 切换到指定类型的 UI 界面。
 * 会先隐藏所有 UI，然后显示目标 UI。
 * 
 * 这是一个便捷方法，相当于先调用 hideAllUI() 再调用 showUI()。
 * 
 * @param type UI 类型
 * @return true 切换成功
 * @return false UI 不存在
 */
bool UIManager::switchToUI(UIType type)
{
    qDebug() << "[UIManager] 切换到 UI:" << type;

    // 1. 隐藏所有 UI
    hideAllUI();
    
    // 2. 显示目标 UI
    return showUI(type);
}

/**
 * @brief 检查 UI 是否可见
 * 
 * 检查指定类型的 UI 是否可见。
 * 如果 UI 不存在，返回 false。
 * 
 * @param type UI 类型
 * @return true UI 可见
 * @return false UI 不可见或不存在
 */
bool UIManager::isUIVisible(UIType type) const
{
    // 检查 UI 是否存在
    if (!m_uis.contains(type)) {
        return false;
    }
    
    QWidget* ui = m_uis[type];
    return ui && ui->isVisible();
}

/**
 * @brief 隐藏所有 UI
 * 
 * 隐藏所有已创建的 UI 界面。
 * 不会发出 uiHidden 信号（批量操作，避免信号风暴）。
 * 
 * 注意：
 * - 批量隐藏 UI 时不会发出 uiHidden 信号
 * - 如果需要监听每个 UI 的隐藏，请使用 hideUI() 逐个隐藏
 */
void UIManager::hideAllUI()
{
    // 遍历所有 UI 并隐藏
    for (QWidget* ui : m_uis) {
        if (ui) {
            ui->hide();
        }
    }
}

// ============================================================
// HUD 更新
// ============================================================

/**
 * @brief 更新生命值显示
 * 
 * 更新 HUD 中的生命值显示。
 * 
 * 当前实现是空的，因为 HUD 界面还没有完善。
 * 后续需要实现具体的生命值 UI 更新逻辑。
 * 
 * @param current 当前生命值
 * @param max 最大生命值
 */
void UIManager::updateHealth(float current, float max)
{
    Q_UNUSED(current);
    Q_UNUSED(max);
    
    // 更新生命值显示
    // TODO: 实现具体的生命值 UI 更新逻辑
}

/**
 * @brief 更新灵能值显示
 * 
 * 更新 HUD 中的灵能值显示。
 * 
 * 当前实现是空的，因为 HUD 界面还没有完善。
 * 后续需要实现具体的灵能值 UI 更新逻辑。
 * 
 * @param current 当前灵能值
 * @param max 最大灵能值
 */
void UIManager::updateMana(float current, float max)
{
    Q_UNUSED(current);
    Q_UNUSED(max);
    
    // 更新灵能值显示
    // TODO: 实现具体的灵能值 UI 更新逻辑
}

/**
 * @brief 更新经验值显示
 * 
 * 更新 HUD 中的经验值和等级显示。
 * 
 * 当前实现是空的，因为 HUD 界面还没有完善。
 * 后续需要实现具体的经验值 UI 更新逻辑。
 * 
 * @param current 当前经验值
 * @param level 当前等级
 */
void UIManager::updateExperience(int current, int level)
{
    Q_UNUSED(current);
    Q_UNUSED(level);
    
    // 更新经验值显示
    // TODO: 实现具体的经验值 UI 更新逻辑
}

/**
 * @brief 更新时间熵显示
 * 
 * 更新 HUD 中的时间熵显示。
 * 
 * 当前实现是空的，因为 HUD 界面还没有完善。
 * 后续需要实现具体的时间熵 UI 更新逻辑。
 * 
 * @param value 时间熵值
 */
void UIManager::updateTimeEntropy(float value)
{
    Q_UNUSED(value);
    
    // 更新时间熵显示
    // TODO: 实现具体的时间熵 UI 更新逻辑
}

/**
 * @brief 更新稳定度显示
 * 
 * 更新 HUD 中的稳定度显示。
 * 
 * 当前实现是空的，因为 HUD 界面还没有完善。
 * 后续需要实现具体的稳定度 UI 更新逻辑。
 * 
 * @param value 稳定度值
 */
void UIManager::updateStability(float value)
{
    Q_UNUSED(value);
    
    // 更新稳定度显示
    // TODO: 实现具体的稳定度 UI 更新逻辑
}

/**
 * @brief 更新小地图
 * 
 * 更新 HUD 中的小地图显示。
 * 
 * 当前实现是空的，因为 HUD 界面还没有完善。
 * 后续需要实现具体的小地图 UI 更新逻辑。
 * 
 * @param playerPos 玩家位置（场景坐标）
 */
void UIManager::updateMinimap(const QPointF& playerPos)
{
    Q_UNUSED(playerPos);
    
    // 更新小地图
    // TODO: 实现具体的小地图 UI 更新逻辑
}

// ============================================================
// 对话框
// ============================================================

/**
 * @brief 显示对话框
 * 
 * 显示一个普通对话框，包含说话者名称和对话文本。
 * 
 * 当前实现只是显示 Dialog UI，没有实际的对话内容更新。
 * 后续需要完善对话框的内容显示逻辑。
 * 
 * @param speaker 说话者名称
 * @param text 对话文本
 */
void UIManager::showDialog(const QString& speaker, const QString& text)
{
    Q_UNUSED(speaker);
    Q_UNUSED(text);
    
    qDebug() << "[UIManager] 显示对话框:" << speaker << "-" << text;
    
    // 显示对话框 UI
    showUI(Dialog);
    
    // TODO: 更新对话框内容（说话者名称和对话文本）
}

/**
 * @brief 隐藏对话框
 * 
 * 隐藏对话框。
 * 隐藏后会发出 dialogClosed 信号。
 */
void UIManager::hideDialog()
{
    qDebug() << "[UIManager] 隐藏对话框";
    
    // 隐藏对话框 UI
    hideUI(Dialog);
    
    // 发出对话框关闭信号
    emit dialogClosed();
}

/**
 * @brief 显示选项对话框
 * 
 * 显示一个选项对话框，包含问题和多个选项。
 * 玩家选择选项后会发出 optionSelected 信号。
 * 
 * 当前实现只是显示 Dialog UI，没有实际的选项内容更新。
 * 后续需要完善选项对话框的内容显示逻辑。
 * 
 * @param question 问题文本
 * @param options 选项列表
 */
void UIManager::showOptions(const QString& question, const QStringList& options)
{
    Q_UNUSED(question);
    Q_UNUSED(options);
    
    qDebug() << "[UIManager] 显示选项对话框:" << question << "，选项数量:" << options.size();
    
    // 显示对话框 UI
    showUI(Dialog);
    
    // TODO: 更新选项对话框内容（问题和选项列表）
}

// ============================================================
// 通知系统
// ============================================================

/**
 * @brief 显示通知
 * 
 * 显示一个普通通知消息。
 * 通知会在指定时间后自动消失。
 * 
 * 当前实现只是输出调试信息，没有实际的 UI 通知。
 * 后续需要实现具体的通知 UI 显示逻辑。
 * 
 * @param message 通知消息
 * @param duration 持续时间（秒），默认 3.0 秒
 */
void UIManager::showNotification(const QString& message, float duration)
{
    Q_UNUSED(duration);
    
    qDebug() << "[UIManager] 显示通知:" << message << "，持续时间:" << duration << "秒";
    
    // TODO: 实现具体的通知 UI 显示逻辑
    // 可以使用 QTimer 来控制通知的自动消失
}

/**
 * @brief 显示成就通知
 * 
 * 显示一个成就解锁通知。
 * 成就通知有特殊的样式和更长的持续时间。
 * 
 * 当前实现只是调用 showNotification()，没有特殊样式。
 * 后续需要实现成就通知的特殊样式。
 * 
 * @param achievementName 成就名称
 */
void UIManager::showAchievementNotification(const QString& achievementName)
{
    qDebug() << "[UIManager] 显示成就通知:" << achievementName;
    
    // 成就通知有更长的持续时间（5 秒）
    showNotification("成就解锁: " + achievementName, 5.0f);
    
    // TODO: 实现成就通知的特殊样式（如金色边框、特殊动画等）
}

// ============================================================
// 私有方法
// ============================================================

/**
 * @brief 创建默认 UI
 * 
 * 创建所有默认的 UI 界面。
 * 包括主菜单、游戏 HUD、暂停菜单、背包等。
 * 
 * 主菜单实现：
 * - 游戏标题（星溯旅人）
 * - 开始游戏按钮
 * - 继续游戏按钮
 * - 设置按钮
 * - 退出游戏按钮
 * 
 * 其他 UI 目前是占位实现，只有空的 QWidget。
 * 
 * 注意：
 * - 大部分 UI 目前是占位实现
 * - 主菜单有基本的按钮布局
 * - 其他 UI 后续需要完善
 */
void UIManager::createDefaultUIs()
{
    // 检查父窗口是否有效
    if (!m_parentWidget) {
        return;
    }

    qDebug() << "[UIManager] 创建默认 UI 界面...";

    // ============================================================
    // 1. 创建主菜单
    // ============================================================
    qDebug() << "[UIManager] 创建主菜单";
    QWidget* mainMenu = new QWidget(m_parentWidget);
    mainMenu->setObjectName("MainMenu");
    
    // 创建主菜单布局
    QVBoxLayout* mainLayout = new QVBoxLayout(mainMenu);
    
    // 添加游戏标题
    QLabel* titleLabel = new QLabel("星溯旅人", mainMenu);
    titleLabel->setStyleSheet("font-size: 48px; font-weight: bold; color: #6DD5FA;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    // 添加弹性空间（将按钮推到中间）
    mainLayout->addStretch();
    
    // 添加开始游戏按钮
    QPushButton* startButton = new QPushButton("开始游戏", mainMenu);
    startButton->setMinimumHeight(50);
    mainLayout->addWidget(startButton);
    
    // 添加继续游戏按钮
    QPushButton* continueButton = new QPushButton("继续游戏", mainMenu);
    continueButton->setMinimumHeight(50);
    mainLayout->addWidget(continueButton);
    
    // 添加设置按钮
    QPushButton* settingsButton = new QPushButton("设置", mainMenu);
    settingsButton->setMinimumHeight(50);
    mainLayout->addWidget(settingsButton);
    
    // 添加退出游戏按钮
    QPushButton* exitButton = new QPushButton("退出游戏", mainMenu);
    exitButton->setMinimumHeight(50);
    mainLayout->addWidget(exitButton);
    
    // 添加弹性空间（将按钮推到中间）
    mainLayout->addStretch();
    
    // 设置布局和大小
    mainMenu->setLayout(mainLayout);
    mainMenu->resize(m_parentWidget->size());
    
    // 添加到 UI 映射
    m_uis[MainMenu] = mainMenu;

    // ============================================================
    // 2. 创建游戏 HUD（占位）
    // ============================================================
    qDebug() << "[UIManager] 创建游戏 HUD（占位）";
    QWidget* gameHUD = new QWidget(m_parentWidget);
    gameHUD->setObjectName("GameHUD");
    // 设置鼠标事件穿透（HUD 不阻挡游戏操作）
    gameHUD->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_uis[GameHUD] = gameHUD;

    // ============================================================
    // 3. 创建暂停菜单（占位）
    // ============================================================
    qDebug() << "[UIManager] 创建暂停菜单（占位）";
    QWidget* pauseMenu = new QWidget(m_parentWidget);
    pauseMenu->setObjectName("PauseMenu");
    m_uis[PauseMenu] = pauseMenu;

    // ============================================================
    // 4. 创建其他 UI 占位
    // ============================================================
    qDebug() << "[UIManager] 创建其他 UI 占位";
    
    // 背包界面
    m_uis[Inventory] = new QWidget(m_parentWidget);
    m_uis[Inventory]->setObjectName("Inventory");
    
    // 技能树界面
    m_uis[SkillTree] = new QWidget(m_parentWidget);
    m_uis[SkillTree]->setObjectName("SkillTree");
    
    // 对话框
    m_uis[Dialog] = new QWidget(m_parentWidget);
    m_uis[Dialog]->setObjectName("Dialog");
    
    // 设置界面
    m_uis[Settings] = new QWidget(m_parentWidget);
    m_uis[Settings]->setObjectName("Settings");
    
    // 游戏结束界面
    m_uis[GameOver] = new QWidget(m_parentWidget);
    m_uis[GameOver]->setObjectName("GameOver");
    
    // 胜利界面
    m_uis[Victory] = new QWidget(m_parentWidget);
    m_uis[Victory]->setObjectName("Victory");

    // ============================================================
    // 5. 初始化 UI 状态
    // ============================================================
    
    // 默认隐藏所有 UI
    hideAllUI();
    
    // 显示主菜单（默认显示主菜单）
    showUI(MainMenu);

    qDebug() << "[UIManager] 默认 UI 界面创建完成，数量:" << m_uis.size();
}
