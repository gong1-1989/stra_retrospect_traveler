/**
 * @file UIManager.h
 * @brief UI 管理器类
 * 
 * UI 管理器负责游戏中所有用户界面的管理，包括：
 * - UI 界面的创建、显示和隐藏
 * - UI 界面的切换
 * - HUD（抬头显示）的更新
 * - 对话框系统
 * - 通知系统
 * 
 * 主要特性：
 * 1. UI 管理：支持多种 UI 类型，通过枚举标识
 * 2. 界面切换：支持快速切换 UI，自动隐藏其他 UI
 * 3. HUD 更新：支持生命值、灵能值、经验值等状态显示
 * 4. 对话框：支持普通对话和选项对话
 * 5. 通知系统：支持普通通知和成就通知
 * 
 * UI 类型：
 * - MainMenu：主菜单
 * - GameHUD：游戏 HUD
 * - PauseMenu：暂停菜单
 * - Inventory：背包
 * - SkillTree：技能树
 * - Dialog：对话框
 * - Settings：设置
 * - GameOver：游戏结束
 * - Victory：胜利
 * 
 * 使用示例：
 * @code
 * UIManager uiManager;
 * uiManager.initialize(mainWindow);
 * 
 * // 显示主菜单
 * uiManager.showUI(UIManager::MainMenu);
 * 
 * // 更新 HUD
 * uiManager.updateHealth(100, 100);
 * uiManager.updateMana(50, 100);
 * 
 * // 显示对话框
 * uiManager.showDialog("NPC", "你好，旅行者！");
 * 
 * // 显示通知
 * uiManager.showNotification("任务完成！");
 * @endcode
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QWidget>
#include <QPointF>
#include <QStringList>

/**
 * @brief UI 管理器类
 * 
 * 负责游戏 UI 的管理和切换。
 * 基于 Qt Widgets，提供 UI 管理、HUD 更新、对话框、通知系统等功能。
 * 
 * UI 架构：
 * - 每个 UI 是一个 QWidget，包含该界面的所有元素
 * - UI 通过枚举类型标识，支持多个 UI 并存
 * - 同一时间可以显示多个 UI（如 HUD + 对话框）
 * - 支持 UI 的显示、隐藏、切换操作
 * 
 * HUD 系统：
 * - 生命值、灵能值、经验值等状态显示
 * - 时间熵、稳定度等特殊状态显示
 * - 小地图显示
 * 
 * 对话框系统：
 * - 普通对话框（说话者 + 文本）
 * - 选项对话框（问题 + 选项列表）
 * 
 * 通知系统：
 * - 普通通知（消息 + 持续时间）
 * - 成就通知（成就名称 + 特殊样式）
 * 
 * 注意：
 * - UI 对象由 UI 管理器管理，不需要手动删除
 * - UI 切换是即时的，没有过渡动画
 * - 如果需要过渡效果，可以在 uiShown/uiHidden 信号中处理
 */
class UIManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief UI 类型枚举
     * 
     * 定义游戏中所有可用的 UI 类型。
     * 每个类型对应一个 QWidget 界面。
     */
    enum UIType {
        MainMenu = 0,       ///< 主菜单
        GameHUD = 1,        ///< 游戏 HUD（抬头显示）
        PauseMenu = 2,      ///< 暂停菜单
        Inventory = 3,      ///< 背包界面
        SkillTree = 4,      ///< 技能树界面
        Dialog = 5,         ///< 对话框
        Settings = 6,       ///< 设置界面
        GameOver = 7,       ///< 游戏结束界面
        Victory = 8         ///< 胜利界面
    };

    /**
     * @brief 构造函数
     * 
     * 创建 UI 管理器实例，但不进行初始化。
     * 必须调用 initialize() 完成初始化后才能使用。
     * 
     * @param parent 父对象，用于 Qt 对象树内存管理
     */
    explicit UIManager(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     * 
     * 销毁 UI 管理器，自动调用 shutdown() 清理所有 UI。
     */
    ~UIManager() override;

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
    bool initialize(QWidget* parentWidget);
    
    /**
     * @brief 关闭 UI 系统
     * 
     * 清理所有 UI 界面，释放资源。
     * 所有 UI 对象会被自动删除。
     */
    void shutdown();
    
    /**
     * @brief 更新 UI
     * 
     * 每帧调用一次，更新 UI 状态。
     * 可以在这里处理动画、计时器等需要每帧更新的 UI 元素。
     * 
     * @param deltaTime 时间增量（秒）
     */
    void update(float deltaTime);

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
    bool showUI(UIType type);
    
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
    bool hideUI(UIType type);
    
    /**
     * @brief 切换 UI
     * 
     * 切换到指定类型的 UI 界面。
     * 会先隐藏所有 UI，然后显示目标 UI。
     * 
     * @param type UI 类型
     * @return true 切换成功
     * @return false UI 不存在
     */
    bool switchToUI(UIType type);
    
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
    bool isUIVisible(UIType type) const;
    
    /**
     * @brief 隐藏所有 UI
     * 
     * 隐藏所有已创建的 UI 界面。
     * 不会发出 uiHidden 信号（批量操作）。
     */
    void hideAllUI();

    // ============================================================
    // HUD 更新
    // ============================================================
    
    /**
     * @brief 更新生命值显示
     * 
     * 更新 HUD 中的生命值显示。
     * 
     * @param current 当前生命值
     * @param max 最大生命值
     */
    void updateHealth(float current, float max);
    
    /**
     * @brief 更新灵能值显示
     * 
     * 更新 HUD 中的灵能值显示。
     * 
     * @param current 当前灵能值
     * @param max 最大灵能值
     */
    void updateMana(float current, float max);
    
    /**
     * @brief 更新经验值显示
     * 
     * 更新 HUD 中的经验值和等级显示。
     * 
     * @param current 当前经验值
     * @param level 当前等级
     */
    void updateExperience(int current, int level);
    
    /**
     * @brief 更新时间熵显示
     * 
     * 更新 HUD 中的时间熵显示。
     * 
     * @param value 时间熵值
     */
    void updateTimeEntropy(float value);
    
    /**
     * @brief 更新稳定度显示
     * 
     * 更新 HUD 中的稳定度显示。
     * 
     * @param value 稳定度值
     */
    void updateStability(float value);
    
    /**
     * @brief 更新小地图
     * 
     * 更新 HUD 中的小地图显示。
     * 
     * @param playerPos 玩家位置（场景坐标）
     */
    void updateMinimap(const QPointF& playerPos);

    // ============================================================
    // 对话框
    // ============================================================
    
    /**
     * @brief 显示对话框
     * 
     * 显示一个普通对话框，包含说话者名称和对话文本。
     * 
     * @param speaker 说话者名称
     * @param text 对话文本
     */
    void showDialog(const QString& speaker, const QString& text);
    
    /**
     * @brief 隐藏对话框
     * 
     * 隐藏对话框。
     * 隐藏后会发出 dialogClosed 信号。
     */
    void hideDialog();
    
    /**
     * @brief 显示选项对话框
     * 
     * 显示一个选项对话框，包含问题和多个选项。
     * 玩家选择选项后会发出 optionSelected 信号。
     * 
     * @param question 问题文本
     * @param options 选项列表
     */
    void showOptions(const QString& question, const QStringList& options);

    // ============================================================
    // 通知系统
    // ============================================================
    
    /**
     * @brief 显示通知
     * 
     * 显示一个普通通知消息。
     * 通知会在指定时间后自动消失。
     * 
     * @param message 通知消息
     * @param duration 持续时间（秒），默认 3.0 秒
     */
    void showNotification(const QString& message, float duration = 3.0f);
    
    /**
     * @brief 显示成就通知
     * 
     * 显示一个成就解锁通知。
     * 成就通知有特殊的样式和更长的持续时间。
     * 
     * @param achievementName 成就名称
     */
    void showAchievementNotification(const QString& achievementName);

signals:
    /**
     * @brief UI 显示信号
     * 
     * 当 UI 显示后发出，携带 UI 类型。
     * 可以连接此信号来处理 UI 显示逻辑（如播放音效、动画等）。
     * 
     * @param type UI 类型
     */
    void uiShown(UIType type);
    
    /**
     * @brief UI 隐藏信号
     * 
     * 当 UI 隐藏后发出，携带 UI 类型。
     * 可以连接此信号来处理 UI 隐藏逻辑（如播放音效、动画等）。
     * 
     * @param type UI 类型
     */
    void uiHidden(UIType type);
    
    /**
     * @brief 选项选择信号
     * 
     * 当玩家在选项对话框中选择了一个选项后发出。
     * 携带选项的索引（从 0 开始）。
     * 
     * @param optionIndex 选项索引
     */
    void optionSelected(int optionIndex);
    
    /**
     * @brief 对话框关闭信号
     * 
     * 当对话框关闭后发出。
     */
    void dialogClosed();

private:
    /**
     * @brief 创建默认 UI
     * 
     * 创建所有默认的 UI 界面。
     * 包括主菜单、游戏 HUD、暂停菜单、背包等。
     * 
     * 注意：
     * - 大部分 UI 目前是占位实现
     * - 主菜单有基本的按钮布局
     * - 其他 UI 后续需要完善
     */
    void createDefaultUIs();

    // ============================================================
    // 成员变量
    // ============================================================
    
    // 父窗口
    QWidget* m_parentWidget = nullptr;  ///< 父窗口指针（所有 UI 的父窗口）
    
    // UI 管理
    QMap<UIType, QWidget*> m_uis;  ///< UI 映射（类型 -> UI 指针）
    
    // 当前 UI
    UIType m_currentUI = MainMenu;  ///< 当前主要 UI（参考用，实际可以显示多个 UI）
    
    // 状态
    bool m_initialized = false;  ///< 是否已初始化
};

#endif // UI_MANAGER_H
