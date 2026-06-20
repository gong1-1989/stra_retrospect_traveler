/**
 * @file ContentPlugin.h
 * @brief 内容插件示例 - 时空裂隙
 * 
 * 这是一个内容插件示例，展示如何通过插件扩展游戏内容。
 * 
 * 插件功能：
 * - 添加一个新场景：时空裂隙场景
 * - 添加一个新角色：时空守护者
 * - 添加新的游戏物品：时空碎片
 * - 注册新的游戏事件
 * 
 * 这个插件展示了如何通过插件系统扩展游戏的内容。
 * 
 * @author 星溯旅人开发团队
 * @version 1.2.0
 * @date 2026
 */
#ifndef CONTENT_PLUGIN_H
#define CONTENT_PLUGIN_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QList>
#include "../PluginManager.h"

/**
 * @brief 物品信息结构
 * 
 * 定义游戏物品的基本信息。
 */
struct ItemInfo {
    QString id;           ///< 物品ID
    QString name;         ///< 物品名称
    QString description;  ///< 物品描述
    QString type;         ///< 物品类型
    int rarity;           ///< 稀有度 1-5
    float value;          ///< 价值
};

/**
 * @brief 角色信息结构
 * 
 * 定义游戏角色的基本信息。
 */
struct CharacterInfo {
    QString id;           ///< 角色ID
    QString name;         ///< 角色名称
    QString description;  ///< 角色描述
    QString role;         ///< 角色定位
    int level;            ///< 等级
};

/**
 * @brief 场景信息结构
 * 
 * 定义游戏场景的基本信息。
 */
struct SceneInfo {
    QString id;           ///< 场景ID
    QString name;         ///< 场景名称
    QString description;  ///< 场景描述
    QString theme;        ///< 场景主题
    int difficulty;       ///< 难度等级
};

/**
 * @brief 内容插件类 - 时空裂隙
 * 
 * 这是一个内容插件示例，展示如何通过插件扩展游戏内容。
 * 
 * 功能说明：
 * - 添加新场景：时空裂隙
 * - 添加新角色：时空守护者
 * - 添加新物品：时空碎片、时空罗盘、时间沙漏
 * - 注册新的游戏事件
 * 
 * 使用示例：
 * @code
 * // 插件加载后，可以通过插件管理器获取
 * ContentPlugin* plugin = qobject_cast<ContentPlugin*>(
 *     pluginManager.getPlugin("TimeRiftContent")
 * );
 * 
 * if (plugin) {
 *     // 获取新场景列表
 *     QList<SceneInfo> scenes = plugin->getNewScenes();
 *     
 *     // 获取新物品
 *     ItemInfo item = plugin->getItem("time_shard");
 * }
 * @endcode
 */
class ContentPlugin : public QObject, public IPlugin
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)
    Q_PLUGIN_METADATA(IID "com.starretrospect.IPlugin/1.0" FILE "content.json")
    
public:
    /**
     * @brief 构造函数
     */
    explicit ContentPlugin(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~ContentPlugin() override;
    
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
    // 内容插件特有功能
    // ============================================================
    
    /**
     * @brief 获取所有新场景
     * @return 场景信息列表
     */
    QList<SceneInfo> getNewScenes() const;
    
    /**
     * @brief 获取所有新角色
     * @return 角色信息列表
     */
    QList<CharacterInfo> getNewCharacters() const;
    
    /**
     * @brief 获取所有新物品
     * @return 物品信息列表
     */
    QList<ItemInfo> getNewItems() const;
    
    /**
     * @brief 根据ID获取物品信息
     * @param itemId 物品ID
     * @return 物品信息，不存在时返回空
     */
    ItemInfo getItem(const QString& itemId) const;
    
    /**
     * @brief 获取场景数量
     * @return 场景数量
     */
    int getSceneCount() const { return m_scenes.size(); }
    
    /**
     * @brief 获取物品数量
     * @return 物品数量
     */
    int getItemCount() const { return m_items.size(); }
    
    /**
     * @brief 获取角色数量
     * @return 角色数量
     */
    int getCharacterCount() const { return m_characters.size(); }
    
private:
    /**
     * @brief 初始化场景内容
     * 
     * 注册所有新场景。
     */
    void initializeScenes();
    
    /**
     * @brief 初始化角色内容
     * 
     * 注册所有新角色。
     */
    void initializeCharacters();
    
    /**
     * @brief 初始化物品内容
     * 
     * 注册所有新物品。
     */
    void initializeItems();
    
    // ============================================================
    // 成员变量
    // ============================================================
    
    // 场景映射（ID -> 场景信息）
    QMap<QString, SceneInfo> m_scenes;
    
    // 角色映射（ID -> 角色信息）
    QMap<QString, CharacterInfo> m_characters;
    
    // 物品映射（ID -> 物品信息）
    QMap<QString, ItemInfo> m_items;
};

#endif // CONTENT_PLUGIN_H
