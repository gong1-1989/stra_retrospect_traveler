# 插件开发指南

## 目录

1. [概述](#概述)
2. [插件类型](#插件类型)
3. [开发环境搭建](#开发环境搭建)
4. [第一个插件](#第一个插件)
5. [插件接口详解](#插件接口详解)
6. [插件配置文件](#插件配置文件)
7. [插件依赖管理](#插件依赖管理)
8. [插件打包发布](#插件打包发布)
9. [最佳实践](#最佳实践)
10. [常见问题](#常见问题)

---

## 概述

星溯旅人拥有强大的插件系统，允许开发者扩展游戏的各个方面。
通过插件系统，你可以：

- 添加新的游戏玩法机制
- 创建自定义UI界面
- 添加新的场景、角色、物品、任务等内容
- 修改游戏平衡和数值
- 集成第三方功能

插件系统基于 Qt 插件框架，使用 C++ 开发，支持动态加载和卸载。

---

## 插件类型

### 1. 玩法插件 (Gameplay Plugin)

继承 `IGameplayPlugin` 接口，用于扩展游戏玩法。

**适用场景：**
- 新的游戏机制
- 战斗系统修改
- 玩家属性调整
- 任务系统扩展
- 物品效果实现

**主要回调：**
- 游戏生命周期：`onGameStart()`, `onGameEnd()`, `onGamePause()`, `onGameResume()`
- 玩家相关：`onPlayerLevelUp()`, `onPlayerDeath()`, `onPlayerRespawn()`
- 战斗相关：`onCombatStart()`, `onCombatEnd()`, `onDamageDealt()`, `onDamageTaken()`
- 场景相关：`onSceneEnter()`, `onSceneLeave()`
- 物品相关：`onItemAcquired()`, `onItemUsed()`
- 任务相关：`onQuestAccepted()`, `onQuestCompleted()`
- 游戏更新：`onGameUpdate()`

### 2. UI插件 (UI Plugin)

继承 `IUIContentPlugin` 接口，用于扩展游戏UI。

**适用场景：**
- 自定义UI控件
- 菜单扩展
- HUD元素添加
- 设置面板
- 自定义对话框
- 主题和样式

**主要功能：**
- 自定义控件：`createCustomWidget()`, `getSupportedWidgets()`
- 菜单扩展：`getMenuActions()`, `createMenu()`
- HUD扩展：`getHUDElements()`, `getSupportedHUDLayers()`
- 设置面板：`createSettingsPanel()`, `getSettingsCategory()`
- 对话框：`showDialog()`, `getSupportedDialogs()`
- 主题样式：`getGlobalStylesheet()`, `getThemeName()`
- UI事件：`onUIInitialized()`, `onThemeChanged()`, `onLanguageChanged()`

### 3. 内容插件 (Content Plugin)

继承 `IContentPlugin` 接口，用于添加游戏内容。

**适用场景：**
- 新场景
- 新角色
- 新物品
- 新任务
- 新技能
- 新敌人
- 新音频

**主要内容类型：**
- 场景：`getScenes()`, `getSceneInfo()`
- 角色：`getCharacters()`, `getCharacterInfo()`
- 物品：`getItems()`, `getItemInfo()`
- 任务：`getQuests()`, `getQuestInfo()`
- 技能：`getSkills()`, `getSkillInfo()`
- 敌人：`getEnemies()`, `getEnemyInfo()`
- 音频：`getBackgroundMusic()`, `getSoundEffects()`

---

## 开发环境搭建

### 系统要求

- Qt 6.10 或更高版本
- C++17 兼容编译器
- CMake 3.16 或更高版本
- MinGW-w64 (Windows) 或 GCC (Linux) 或 Clang (macOS)

### 安装Qt

1. 下载 Qt Online Installer
2. 安装 Qt 6.10（选择 MinGW 或对应编译器）
3. 安装 Qt Creator（可选但推荐）

### 获取游戏源码

```bash
git clone <游戏仓库地址>
cd star_retrospect_traveler
```

### 配置开发环境

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

---

## 第一个插件

### 步骤1：创建插件目录

在 `plugins/` 目录下创建你的插件目录：

```
plugins/
└── MyFirstPlugin/
    ├── CMakeLists.txt
    ├── MyFirstPlugin.h
    ├── MyFirstPlugin.cpp
    └── my_first_plugin.json
```

### 步骤2：创建插件头文件

```cpp
#ifndef MY_FIRST_PLUGIN_H
#define MY_FIRST_PLUGIN_H

#include "IGameplayPlugin.h"

class MyFirstPlugin : public IGameplayPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.starretrospect.IGameplayPlugin" FILE "my_first_plugin.json")
    Q_INTERFACES(IGameplayPlugin IPlugin)
    
public:
    MyFirstPlugin();
    virtual ~MyFirstPlugin();
    
    // IPlugin 接口
    QString getName() const override;
    QString getVersion() const override;
    QString getAuthor() const override;
    QString getDescription() const override;
    bool initialize() override;
    void shutdown() override;
    
    // 游戏回调
    void onGameStart() override;
    void onPlayerLevelUp(int newLevel, int oldLevel) override;
};

#endif // MY_FIRST_PLUGIN_H
```

### 步骤3：创建插件实现文件

```cpp
#include "MyFirstPlugin.h"
#include <QDebug>

MyFirstPlugin::MyFirstPlugin()
{
    qDebug() << "[MyFirstPlugin] 创建插件";
}

MyFirstPlugin::~MyFirstPlugin()
{
    qDebug() << "[MyFirstPlugin] 销毁插件";
}

QString MyFirstPlugin::getName() const
{
    return "MyFirstPlugin";
}

QString MyFirstPlugin::getVersion() const
{
    return "1.0.0";
}

QString MyFirstPlugin::getAuthor() const
{
    return "你的名字";
}

QString MyFirstPlugin::getDescription() const
{
    return "我的第一个插件";
}

bool MyFirstPlugin::initialize()
{
    qDebug() << "[MyFirstPlugin] 初始化插件";
    return true;
}

void MyFirstPlugin::shutdown()
{
    qDebug() << "[MyFirstPlugin] 关闭插件";
}

void MyFirstPlugin::onGameStart()
{
    qDebug() << "[MyFirstPlugin] 游戏开始了！";
}

void MyFirstPlugin::onPlayerLevelUp(int newLevel, int oldLevel)
{
    qDebug() << "[MyFirstPlugin] 玩家升级了！" << oldLevel << "->" << newLevel;
}
```

### 步骤4：创建插件配置文件

```json
{
    "id": "com.yourname.myfirstplugin",
    "name": "我的第一个插件",
    "version": "1.0.0",
    "author": "你的名字",
    "description": "这是我的第一个插件",
    "type": "gameplay",
    "enabled": true,
    "priority": 10,
    "dependencies": []
}
```

### 步骤5：创建CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyFirstPlugin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_add_plugin(MyFirstPlugin
    MyFirstPlugin.h
    MyFirstPlugin.cpp
    my_first_plugin.json
)

target_link_libraries(MyFirstPlugin PRIVATE
    Qt6::Core
    Qt6::Widgets
)

# 包含游戏头文件
target_include_directories(MyFirstPlugin PRIVATE
    ${CMAKE_SOURCE_DIR}/..
)

set_target_properties(MyFirstPlugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)
```

### 步骤6：编译插件

```bash
cd plugins/MyFirstPlugin
mkdir build
cd build
cmake ..
cmake --build .
```

### 步骤7：安装插件

将编译生成的插件文件（.dll / .so / .dylib）复制到游戏的 `plugins/` 目录。

---

## 插件接口详解

### IPlugin 基础接口

所有插件都必须实现 `IPlugin` 接口，它定义了插件的基本信息和生命周期。

#### 基本信息方法

```cpp
// 获取插件名称（唯一标识）
virtual QString getName() const = 0;

// 获取插件版本号（语义化版本）
virtual QString getVersion() const = 0;

// 获取插件作者
virtual QString getAuthor() const = 0;

// 获取插件描述
virtual QString getDescription() const = 0;
```

#### 生命周期方法

```cpp
// 初始化插件，加载资源、注册回调等
virtual bool initialize() = 0;

// 关闭插件，清理资源、保存数据等
virtual void shutdown() = 0;
```

### IGameplayPlugin 玩法接口

玩法插件通过回调机制参与游戏逻辑。

#### 伤害修改示例

```cpp
void MyPlugin::onDamageDealt(int& damage, const QString& source, const QString& target)
{
    // 增加 20% 伤害
    damage = static_cast<int>(damage * 1.2);
}
```

#### 物品拦截示例

```cpp
bool MyPlugin::onItemUsed(const QString& itemId)
{
    if (itemId == "special_item") {
        // 自定义处理逻辑
        doSomethingSpecial();
        return true;  // 返回 true 阻止默认行为
    }
    return false;  // 返回 false 继续默认行为
}
```

### IUIContentPlugin UI接口

UI插件可以向游戏添加自定义UI元素。

#### 自定义控件示例

```cpp
QWidget* MyUIPlugin::createCustomWidget(const QString& widgetId, QWidget* parent)
{
    if (widgetId == "my_widget") {
        QLabel* label = new QLabel("自定义控件", parent);
        label->setStyleSheet("color: red; font-size: 20px;");
        return label;
    }
    return nullptr;
}
```

#### 设置面板示例

```cpp
QWidget* MyUIPlugin::createSettingsPanel(QWidget* parent)
{
    QWidget* panel = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(panel);
    
    QCheckBox* enableFeature = new QCheckBox("启用我的功能", panel);
    layout->addWidget(enableFeature);
    
    QSlider* intensitySlider = new QSlider(Qt::Horizontal, panel);
    intensitySlider->setRange(0, 100);
    layout->addWidget(intensitySlider);
    
    layout->addStretch();
    
    return panel;
}
```

### IContentPlugin 内容接口

内容插件向游戏添加新的内容数据。

#### 物品内容示例

```cpp
ContentItemInfo MyContentPlugin::getItemInfo(const QString& itemId) const
{
    if (itemId == "my_sword") {
        ContentItemInfo info;
        info.id = "my_sword";
        info.name = "我的宝剑";
        info.description = "一把来自插件的神奇宝剑";
        info.category = "武器";
        info.iconPath = ":/myplugin/images/sword.png";
        info.properties["attack"] = 50;
        info.properties["price"] = 1000;
        return info;
    }
    return ContentItemInfo();
}
```

---

## 插件配置文件

### 配置文件格式

插件配置文件使用 JSON 格式，文件名必须与 `Q_PLUGIN_METADATA` 宏中指定的一致。

### 完整配置示例

```json
{
    "id": "com.example.myplugin",
    "name": "我的插件",
    "version": "1.2.3",
    "author": "作者名",
    "description": "插件详细描述",
    "website": "https://example.com",
    "email": "contact@example.com",
    "license": "MIT",
    "type": "gameplay",
    "category": "功能扩展",
    "tags": ["combat", "balance"],
    "enabled": true,
    "priority": 50,
    "autoLoad": true,
    "icon": "icon.png",
    "dependencies": [
        {
            "id": "core-plugin",
            "minVersion": "1.0.0",
            "maxVersion": "2.0.0",
            "required": true
        },
        {
            "id": "optional-plugin",
            "minVersion": "0.5.0",
            "required": false
        }
    ],
    "provides": [
        "custom_api"
    ],
    "minEngineVersion": "0.1.0",
    "maxEngineVersion": "1.0.0",
    "resourcePrefix": "myplugin"
}
```

### 配置字段说明

| 字段 | 类型 | 必填 | 说明 |
|------|------|------|------|
| id | string | 是 | 插件唯一标识符，建议使用反向域名格式 |
| name | string | 是 | 插件显示名称 |
| version | string | 是 | 版本号，遵循语义化版本规范 |
| author | string | 是 | 作者名称 |
| description | string | 否 | 插件详细描述 |
| website | string | 否 | 插件网站 |
| email | string | 否 | 联系邮箱 |
| license | string | 否 | 开源协议 |
| type | string | 是 | 插件类型：core, gameplay, content, ui, render, audio, tool, mod |
| category | string | 否 | 插件分类 |
| tags | array | 否 | 标签列表 |
| enabled | bool | 否 | 是否启用，默认 true |
| priority | int | 否 | 加载优先级，数值越大越先加载，默认 0 |
| autoLoad | bool | 否 | 是否自动加载，默认 true |
| icon | string | 否 | 图标文件路径 |
| dependencies | array | 否 | 依赖列表 |
| provides | array | 否 | 提供的功能列表 |
| minEngineVersion | string | 否 | 最低引擎版本要求 |
| maxEngineVersion | string | 否 | 最高引擎版本要求 |
| resourcePrefix | string | 否 | 资源前缀 |

---

## 插件依赖管理

### 声明依赖

在插件配置文件的 `dependencies` 字段中声明依赖：

```json
{
    "dependencies": [
        {
            "id": "required-plugin",
            "minVersion": "1.0.0",
            "required": true
        },
        {
            "id": "optional-plugin",
            "minVersion": "0.5.0",
            "required": false
        }
    ]
}
```

### 依赖类型

- **必需依赖 (required: true)**：插件加载前必须满足，否则插件无法加载
- **可选依赖 (required: false)**：如果存在则加载，不存在也不影响

### 版本范围

可以通过 `minVersion` 和 `maxVersion` 指定依赖的版本范围：

```json
{
    "id": "some-plugin",
    "minVersion": "1.0.0",
    "maxVersion": "2.0.0",
    "required": true
}
```

### 依赖解析顺序

插件按以下顺序加载：
1. 按优先级从高到低排序
2. 按依赖关系拓扑排序
3. 确保依赖的插件先加载

---

## 插件打包发布

### 插件目录结构

```
MyPlugin/
├── MyPlugin.dll          # 插件二进制文件
├── my_plugin.json        # 插件配置文件
├── icon.png              # 插件图标
├── README.md             # 说明文档
├── LICENSE               # 许可证
├── assets/               # 资源文件
│   ├── images/
│   ├── audio/
│   └── data/
└── docs/                 # 文档
    └── manual.md
```

### 打包格式

推荐使用 ZIP 格式打包插件：

```bash
zip -r MyPlugin_v1.0.0.zip MyPlugin/
```

### 版本号规范

使用语义化版本（Semantic Versioning）：

- **主版本号**：不兼容的 API 修改
- **次版本号**：向下兼容的功能性新增
- **修订号**：向下兼容的问题修正

示例：`1.2.3`
- 主版本：1
- 次版本：2
- 修订号：3

---

## 最佳实践

### 1. 性能优化

- 避免在 `onGameUpdate()` 中执行耗时操作
- 使用信号槽机制代替轮询
- 合理使用缓存
- 注意内存泄漏

### 2. 错误处理

- 初始化失败时返回 `false`
- 使用 `qWarning()` 和 `qCritical()` 输出错误信息
- 提供降级方案

### 3. 兼容性

- 遵循语义化版本
- 保持 API 向后兼容
- 提供版本迁移指南

### 4. 安全性

- 不要执行未验证的代码
- 验证用户输入
- 注意文件操作安全

### 5. 代码规范

- 遵循项目代码风格
- 添加详细注释
- 使用 Doxygen 格式注释
- 编写单元测试

### 6. 调试技巧

- 使用 `qDebug()` 输出调试信息
- 在 Qt Creator 中设置断点
- 使用插件管理器查看插件状态
- 检查日志文件

---

## 常见问题

### Q1: 插件加载失败怎么办？

**A:** 检查以下几点：
1. 插件文件是否在正确的目录
2. 插件依赖是否都已安装
3. 插件版本是否与游戏版本兼容
4. 查看日志文件中的错误信息
5. 确认插件实现了正确的接口

### Q2: 如何调试插件？

**A:** 
1. 在 Qt Creator 中以调试模式启动游戏
2. 附加到游戏进程
3. 在插件代码中设置断点
4. 使用 `qDebug()` 输出调试信息

### Q3: 插件之间如何通信？

**A:** 
1. 使用信号槽机制
2. 通过插件管理器获取其他插件
3. 使用共享数据结构
4. 通过游戏事件系统

### Q4: 如何添加自定义资源？

**A:** 
1. 创建 Qt 资源文件 (.qrc)
2. 在 CMakeLists.txt 中添加资源
3. 使用 `:/prefix/path/to/resource` 访问资源
4. 在插件配置中设置 `resourcePrefix`

### Q5: 插件可以修改游戏存档吗？

**A:** 
- 玩法插件可以通过回调影响游戏数据
- 内容插件添加的内容会自动保存在存档中
- 建议使用插件配置系统保存插件自己的数据
- 不要直接修改存档文件

### Q6: 如何处理多语言？

**A:** 
- 使用 Qt 的国际化系统 (i18n)
- 提供 .ts 翻译文件
- 使用 `tr()` 函数标记可翻译文本
- 监听 `onLanguageChanged()` 回调

---

## 相关文件

- `IPlugin.h` - 基础插件接口
- `IGameplayPlugin.h` - 玩法插件接口
- `IUIContentPlugin.h` - UI插件接口
- `IContentPlugin.h` - 内容插件接口
- `PluginManager.h` - 插件管理器
- `PluginMetadata.h` - 插件元数据

---

## 示例插件

项目中包含三个示例插件：
- `ExampleGameplayPlugin` - 玩法插件示例
- `ExampleUIPlugin` - UI插件示例
- `ExampleContentPlugin` - 内容插件示例

可以参考这些示例来开发自己的插件。

---

**文档版本**: 1.0.0  
**最后更新**: 2026年
