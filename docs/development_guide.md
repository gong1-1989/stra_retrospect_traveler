# 星溯旅人 - 完整开发指南

## 目录
1. [项目概述](#项目概述)
2. [环境搭建](#环境搭建)
3. [项目结构](#项目结构)
4. [核心模块详解](#核心模块详解)
5. [游戏系统详解](#游戏系统详解)
6. [渲染系统详解](#渲染系统详解)
7. [UI系统详解](#ui系统详解)
8. [编译与运行](#编译与运行)
9. [开发流程](#开发流程)
10. [常见问题](#常见问题)

---

## 项目概述

### 游戏简介
**星溯旅人（Star Retrospect Traveler）** 是一款 2.5D 箱庭探索策略 RPG 游戏，主打时空解谜与因果叙事。玩家将扮演一名拥有时间操控能力的旅者，在各个星球间穿梭，探索时间褶皱，与星核共鸣，与生态共生，揭开宇宙的终极秘密。

### 技术栈
- **开发框架**：Qt 6.10.1
- **编程语言**：C++ 17
- **构建系统**：CMake 3.20+
- **编译器**：MinGW-w64 / MSVC / GCC
- **图形渲染**：Qt Graphics View Framework（2.5D 等距视角）
- **音频系统**：Qt Multimedia
- **数据存储**：加密 JSON + QSettings
- **架构模式**：模块化 + 插件化混合架构

### 核心特色
1. **时间褶皱系统**：多层时间线、时空切换、时间悖论、时间织网
2. **星核共鸣系统**：星核人格化、双向共鸣、星核关系网络
3. **生态共生系统**：生态意识网络、基因融合、共生体智能进化
4. **三大系统联动**：三位一体终极玩法
5. **因果叙事分支**：多时间线叙事、10+ 种结局

---

## 环境搭建

### 1. 安装 Qt
1. 下载 Qt 在线安装器：https://www.qt.io/download
2. 运行安装器，选择 Qt 6.10.1
3. 选择组件：
   - MinGW 13.1.0 64-bit（编译器）
   - Qt Charts (Deprecated)
   - Qt Multimedia
   - Qt Quick 3D
   - Qt Quick Timeline
   - Qt Shader Tools
   - Qt Svg
4. 安装路径建议：`D:/Qt/6.10.1/mingw_64/`

### 2. 安装 CMake
- Qt Creator 自带 CMake，无需额外安装
- 版本：CMake 3.30.5 (Qt Default)

### 3. 安装 Qt Creator
- Qt 安装时会自动安装 Qt Creator
- 这是官方 IDE，集成调试、设计、性能分析工具

### 4. 验证环境
打开 Qt Creator，检查：
- 工具 → 选项 → Kits → Desktop Qt 6.10.1 MinGW 64-bit
- 确保编译器、调试器、CMake 都正确配置

---

## 项目结构

```
star_retrospect_traveler/
├── CMakeLists.txt              # 主 CMake 配置
├── README.md                   # 项目说明
├── docs/                       # 文档目录
│   └── mingw_build_guide.md    # MinGW 编译指南
├── include/                    # 头文件目录
│   ├── core/                   # 核心模块头文件
│   │   ├── GameEngine.h        # 游戏引擎核心
│   │   ├── GameConfig.h        # 游戏配置
│   │   ├── SceneManager.h      # 场景管理器
│   │   ├── ResourceManager.h   # 资源管理器
│   │   ├── InputManager.h      # 输入管理器
│   │   ├── AudioManager.h      # 音频管理器
│   │   └── PluginManager.h     # 插件管理器
│   ├── game/                   # 游戏模块头文件
│   │   ├── Player.h            # 玩家类
│   │   ├── TimeSystem.h        # 时间系统
│   │   ├── ResonanceSystem.h   # 星核共鸣系统
│   │   ├── EcosystemSystem.h   # 生态共生系统
│   │   ├── GameManager.h       # 游戏管理器
│   │   └── ExampleScene.h      # 示例场景
│   ├── render/                 # 渲染模块头文件
│   │   └── RenderManager.h     # 渲染管理器
│   ├── ui/                     # UI 模块头文件
│   │   └── UIManager.h         # UI 管理器
│   └── plugin/                 # 插件模块头文件
├── src/                        # 源代码目录
│   ├── main.cpp                # 主程序入口
│   ├── CMakeLists.txt          # 主程序 CMake 配置
│   ├── core/                   # 核心模块源码
│   ├── game/                   # 游戏模块源码
│   ├── render/                 # 渲染模块源码
│   ├── ui/                     # UI 模块源码
│   ├── audio/                  # 音频模块源码
│   ├── input/                  # 输入模块源码
│   ├── resource/               # 资源模块源码
│   └── plugin/                 # 插件模块源码
├── assets/                     # 资源目录
│   ├── images/                 # 图片资源
│   ├── audio/                  # 音频资源
│   ├── fonts/                  # 字体资源
│   ├── maps/                   # 地图资源
│   └── config/                 # 配置文件
│       └── game.json           # 游戏配置示例
├── tests/                      # 测试目录
├── plugins/                    # 插件目录
└── cmake/                      # CMake 模块
```

---

## 核心模块详解

### 1. GameEngine（游戏引擎核心）

**文件位置**：
- 头文件：`include/core/GameEngine.h`
- 实现文件：`src/core/GameEngine.cpp`

**功能概述**：
游戏引擎的核心类，负责游戏的初始化、主循环、子系统管理等核心功能。

**主要方法**：

```cpp
// 构造函数
explicit GameEngine(const GameConfig& config, QObject* parent = nullptr);

// 初始化游戏引擎
bool initialize();

// 运行游戏主循环
int run();

// 关闭游戏引擎
void shutdown();

// 获取子系统
SceneManager* getSceneManager() const;
ResourceManager* getResourceManager() const;
InputManager* getInputManager() const;
AudioManager* getAudioManager() const;
PluginManager* getPluginManager() const;
RenderManager* getRenderManager() const;
UIManager* getUIManager() const;
GameManager* getGameManager() const;

// 获取主窗口
QMainWindow* getMainWindow() const;

// 获取当前 FPS
float getCurrentFPS() const;

// 获取游戏运行时间
float getGameTime() const;
```

**工作流程**：
1. `initialize()` - 初始化主窗口和所有子系统
2. `run()` - 启动游戏定时器，进入 Qt 事件循环
3. 定时器每帧触发 `update()` 和 `render()`
4. `shutdown()` - 按逆序关闭所有子系统

**使用示例**：

```cpp
// 创建游戏配置
GameConfig config;
config.gameTitle = "星溯旅人";
config.windowSize = QSize(1280, 720);
config.targetFPS = 60;
config.fullscreen = false;
config.debugMode = true;

// 创建游戏引擎
GameEngine engine(config);

// 初始化
if (engine.initialize()) {
    // 运行游戏
    return engine.run();
}

return -1;
```

---

### 2. GameConfig（游戏配置）

**文件位置**：
- 头文件：`include/core/GameConfig.h`
- 实现文件：`src/core/GameConfig.cpp`

**功能概述**：
游戏配置结构体，包含窗口、图形、音频、游戏、控制等配置项。

**配置项**：

```cpp
struct GameConfig {
    // 窗口设置
    QString gameTitle = "星溯旅人";
    QSize windowSize = QSize(1280, 720);
    bool fullscreen = false;
    bool vsync = true;
    
    // 图形设置
    int targetFPS = 60;
    bool antialiasing = true;
    int textureQuality = 2; // 0=低, 1=中, 2=高
    
    // 音频设置
    float masterVolume = 1.0f;
    float musicVolume = 0.8f;
    float soundVolume = 1.0f;
    bool muted = false;
    
    // 游戏设置
    bool debugMode = false;
    bool showFPS = false;
    QString language = "zh_CN";
    
    // 控制设置
    // ... 按键映射等
};
```

**主要方法**：

```cpp
// 从 JSON 文件加载配置
bool loadFromFile(const QString& filePath);

// 保存配置到 JSON 文件
bool saveToFile(const QString& filePath) const;

// 重置为默认配置
void resetToDefaults();
```

---

### 3. SceneManager（场景管理器）

**文件位置**：
- 头文件：`include/core/SceneManager.h`
- 实现文件：`src/core/SceneManager.cpp`

**功能概述**：
负责游戏场景的管理、切换和渲染。

**主要方法**：

```cpp
// 创建新场景
QGraphicsScene* createScene(const QString& name);

// 获取场景
QGraphicsScene* getScene(const QString& name) const;

// 切换场景
bool switchToScene(const QString& name);

// 获取当前场景
QGraphicsScene* getCurrentScene() const;

// 更新场景
void update(float deltaTime);
```

**信号**：
- `sceneChanged(const QString& sceneName)` - 场景切换信号

---

### 4. ResourceManager（资源管理器）

**文件位置**：
- 头文件：`include/core/ResourceManager.h`
- 实现文件：`src/core/ResourceManager.cpp`

**功能概述**：
负责游戏资源的加载、缓存和管理。

**主要方法**：

```cpp
// 加载图片资源
QPixmap loadPixmap(const QString& filePath);

// 加载音效资源
QSoundEffect* loadSound(const QString& filePath);

// 加载字体资源
QFont loadFont(const QString& filePath, int pointSize = 12);

// 卸载未使用的资源
void collectGarbage();

// 清空所有缓存
void clearCache();
```

**资源缓存**：
- `QPixmap` 缓存
- `QSoundEffect` 缓存
- `QFont` 缓存

---

### 5. InputManager（输入管理器）

**文件位置**：
- 头文件：`include/core/InputManager.h`
- 实现文件：`src/core/InputManager.cpp`

**功能概述**：
负责键盘、鼠标等输入的管理。

**按键状态**：
- `KeyReleased` - 已释放
- `KeyPressed` - 按下中
- `KeyJustPressed` - 刚按下
- `KeyJustReleased` - 刚释放

**主要方法**：

```cpp
// 检查按键是否按下
bool isKeyPressed(Qt::Key key) const;

// 检查按键是否刚按下
bool isKeyJustPressed(Qt::Key key) const;

// 检查按键是否刚释放
bool isKeyJustReleased(Qt::Key key) const;

// 获取鼠标位置
QPoint getMousePosition() const;

// 获取鼠标移动增量
QPoint getMouseDelta() const;

// 检查鼠标按钮是否按下
bool isMouseButtonPressed(Qt::MouseButton button) const;
```

**信号**：
- `keyPressed(Qt::Key key)`
- `keyReleased(Qt::Key key)`
- `mouseClicked(Qt::MouseButton button, const QPoint& pos)`

---

### 6. AudioManager（音频管理器）

**文件位置**：
- 头文件：`include/core/AudioManager.h`
- 实现文件：`src/core/AudioManager.cpp`

**功能概述**：
负责游戏音频的播放、控制和管理。

**主要方法**：

```cpp
// 播放音乐
void playMusic(const QString& filePath);
void pauseMusic();
void resumeMusic();
void stopMusic();

// 播放音效
void playSound(const QString& filePath);

// 设置音量
void setMasterVolume(float volume);
void setMusicVolume(float volume);
void setSoundVolume(float volume);

// 设置静音
void setMuted(bool muted);
```

---

### 7. PluginManager（插件管理器）

**文件位置**：
- 头文件：`include/core/PluginManager.h`
- 实现文件：`src/core/PluginManager.cpp`

**功能概述**：
负责插件的发现、加载、卸载和管理。

**IPlugin 接口**：
所有插件必须实现的接口基类。

**主要方法**：

```cpp
// 加载所有插件
bool loadAllPlugins(const QString& pluginDir);

// 加载单个插件
bool loadPlugin(const QString& pluginPath);

// 卸载插件
bool unloadPlugin(const QString& pluginName);

// 获取插件
IPlugin* getPlugin(const QString& pluginName) const;

// 获取所有已加载的插件
QList<IPlugin*> getLoadedPlugins() const;
```

---

## 游戏系统详解

### 1. Player（玩家类）

**文件位置**：
- 头文件：`include/game/Player.h`
- 实现文件：`src/game/Player.cpp`

**功能概述**：
玩家角色类，包含玩家的属性、状态和行为。

**玩家属性**：

```cpp
// 基本属性
QString name;              // 名称
int level = 1;             // 等级
int experience = 0;        // 经验值

// 生命值
float maxHealth = 100.0f;  // 最大生命
float currentHealth = 100.0f; // 当前生命

// 灵能值
float maxMana = 50.0f;     // 最大灵能
float currentMana = 50.0f; // 当前灵能

// 位置和移动
QPointF position;          // 位置
float moveSpeed = 200.0f;  // 移动速度

// 稳定度
float stability = 100.0f;  // 稳定度（0-100）
```

**玩家状态**：
- `Idle` - 空闲
- `Walking` - 行走
- `Running` - 奔跑
- `Jumping` - 跳跃
- `Attacking` - 攻击
- `Hurt` - 受伤
- `Dead` - 死亡

**主要方法**：

```cpp
// 初始化
bool initialize();

// 关闭
void shutdown();

// 更新
void update(float deltaTime);

// 生命值相关
void takeDamage(float damage);
void heal(float amount);
void setMaxHealth(float max);

// 灵能值相关
void consumeMana(float amount);
void restoreMana(float amount);
void setMaxMana(float max);

// 经验和等级
void addExperience(int amount);
void setLevel(int level);
bool checkLevelUp();

// 移动相关
void move(float dx, float dy);
void setPosition(const QPointF& pos);

// 状态相关
PlayerState getState() const;
void setState(PlayerState state);

// 稳定度相关
void addStability(float amount);
void reduceStability(float amount);
void setStability(float value);
```

**信号**：
- `healthChanged(float current, float max)`
- `manaChanged(float current, float max)`
- `experienceChanged(int current, int level)`
- `levelUp(int newLevel)`
- `playerDied()`
- `stateChanged(PlayerState state)`
- `stabilityChanged(float value)`

**使用示例**：

```cpp
// 创建玩家
Player player;
player.initialize();

// 设置玩家属性
player.setName("旅者");
player.setLevel(1);
player.setMaxHealth(100.0f);
player.heal(100.0f);
player.setMaxMana(50.0f);
player.restoreMana(50.0f);

// 玩家受伤
player.takeDamage(20.0f);

// 玩家恢复
player.heal(10.0f);

// 消耗灵能
player.consumeMana(15.0f);

// 获得经验
player.addExperience(100);

// 移动玩家
player.move(10.0f, 5.0f);
```

---

### 2. TimeSystem（时间系统）

**文件位置**：
- 头文件：`include/game/TimeSystem.h`
- 实现文件：`src/game/TimeSystem.cpp`

**功能概述**：
时间褶皱系统，包含时间线切换、时间熵、悖论系统、时间能力等。

**时间线类型**：
- `Present` - 现在
- `PastProsperity` - 过去·繁荣期
- `PastDisaster` - 过去·灾难期
- `FutureDecay` - 未来·衰败期
- `FutureMutation` - 未来·变异期

**时间褶皱类型**：
- `Stable` - 稳定褶皱
- `Unstable` - 不稳定褶皱
- `Fractured` - 断裂褶皱
- `Mirror` - 镜像褶皱
- `Loop` - 循环褶皱

**悖论等级**：
- `None` - 无
- `Minor` - 轻微
- `Moderate` - 中度
- `Severe` - 重度
- `Critical` - 致命

**主要方法**：

```cpp
// 初始化
bool initialize();

// 关闭
void shutdown();

// 更新
void update(float deltaTime);

// 时间线管理
TimelineType getCurrentTimeline() const;
bool switchTimeline(TimelineType timeline);
float getTimelineSwitchCost() const;

// 时间褶皱
FoldType getCurrentFoldType() const;
void setFoldType(FoldType type);
bool isInFold() const;

// 时间熵
float getEntropy() const;
void addEntropy(float amount);
void reduceEntropy(float amount);
void resetEntropy();

// 悖论系统
ParadoxLevel getParadoxLevel() const;
void increaseParadox();
void reduceParadox();
void resetParadox();

// 时间能力
bool activateTimeSlow(float duration, float slowFactor = 0.3f);
bool isTimeSlowActive() const;
bool activateTimeRewind(float duration);
bool isRewinding() const;
```

**信号**：
- `timelineChanged(TimelineType newTimeline)`
- `entropyChanged(float value)`
- `paradoxLevelChanged(ParadoxLevel level)`
- `timeSlowStarted(float duration, float slowFactor)`
- `timeSlowEnded()`

**使用示例**：

```cpp
// 创建时间系统
TimeSystem timeSystem;
timeSystem.initialize();

// 切换时间线
if (timeSystem.switchTimeline(TimeSystem::PastProsperity)) {
    qDebug() << "切换到过去·繁荣期";
}

// 增加时间熵
timeSystem.addEntropy(10.0f);

// 增加悖论等级
timeSystem.increaseParadox();

// 激活时间减速
timeSystem.activateTimeSlow(5.0f, 0.3f);
```

---

### 3. ResonanceSystem（星核共鸣系统）

**文件位置**：
- 头文件：`include/game/ResonanceSystem.h`
- 实现文件：`src/game/ResonanceSystem.cpp`

**功能概述**：
星核共鸣系统，包含星核管理、共鸣等级、环境操控、反噬系统、共鸣组合技等。

**星核类型**：
- `Earth` - 大地星核
- `Ocean` - 海洋星核
- `Flame` - 烈焰星核
- `Storm` - 风暴星核
- `Life` - 生命星核
- `Void` - 虚空星核

**共鸣等级**：
- `None` - 无共鸣
- `Shallow` - 浅层共鸣
- `Deep` - 深层共鸣
- `Full` - 完全共鸣

**反噬等级**：
- `None` - 无反噬
- `Mild` - 轻度反噬
- `Moderate` - 中度反噬
- `Severe` - 重度反噬
- `Permanent` - 永久反噬

**星核信息结构**：

```cpp
struct StarCoreInfo {
    StarCoreType type;           // 类型
    QString name;                // 名称
    QString description;         // 描述
    ResonanceLevel resonanceLevel; // 共鸣等级
    float resonanceDegree;       // 共鸣度
    float stability;             // 稳定度
    bool unlocked;               // 是否解锁
    QString personality;         // 人格
};
```

**主要方法**：

```cpp
// 初始化
bool initialize();

// 关闭
void shutdown();

// 更新
void update(float deltaTime);

// 星核管理
bool unlockStarCore(StarCoreType type);
StarCoreInfo getStarCoreInfo(StarCoreType type) const;
bool isStarCoreUnlocked(StarCoreType type) const;
QList<StarCoreType> getUnlockedCores() const;

// 共鸣系统
bool addResonance(StarCoreType type, float amount);
ResonanceLevel getResonanceLevel(StarCoreType type) const;
bool activateResonance(StarCoreType type);
void deactivateResonance(StarCoreType type);
bool isResonanceActive(StarCoreType type) const;

// 环境操控
bool manipulateTerrain(StarCoreType type, const QPointF& position);
bool manipulateWeather(StarCoreType type, const QString& weatherType);
bool manipulateLife(StarCoreType type, const QPointF& position);

// 反噬系统
BacklashLevel getBacklashLevel() const;
void increaseBacklash(float amount);
void recoverBacklash(float amount);

// 共鸣组合技
bool releaseCombo(const QList<StarCoreType>& cores);
bool isComboAvailable(const QList<StarCoreType>& cores) const;
```

**信号**：
- `starCoreUnlocked(StarCoreType type)`
- `resonanceLevelChanged(StarCoreType type, ResonanceLevel level)`
- `resonanceActivated(StarCoreType type)`
- `backlashLevelChanged(BacklashLevel level)`
- `environmentManipulated(const QString& type, const QPointF& position)`

**使用示例**：

```cpp
// 创建共鸣系统
ResonanceSystem resonanceSystem;
resonanceSystem.initialize();

// 解锁星核
if (resonanceSystem.unlockStarCore(ResonanceSystem::Earth)) {
    qDebug() << "解锁大地星核";
}

// 增加共鸣度
resonanceSystem.addResonance(ResonanceSystem::Earth, 20.0f);

// 激活共鸣
resonanceSystem.activateResonance(ResonanceSystem::Earth);

// 环境操控
resonanceSystem.manipulateTerrain(ResonanceSystem::Earth, QPointF(100, 100));

// 释放组合技
QList<ResonanceSystem::StarCoreType> combo;
combo << ResonanceSystem::Storm << ResonanceSystem::Ocean;
resonanceSystem.releaseCombo(combo);
```

---

### 4. EcosystemSystem（生态共生系统）

**文件位置**：
- 头文件：`include/game/EcosystemSystem.h`
- 实现文件：`src/game/EcosystemSystem.cpp`

**功能概述**：
生态共生系统，包含共生生物管理、共生关系、共生进化、生态平衡、生态事件等。

**共生生物类型**：
- `FlyingType` - 飞行类
- `AquaticType` - 水生类
- `CombatType` - 战斗类
- `PerceptionType` - 感知类
- `SupportType` - 辅助类

**共生进化路线**：
- `NaturalEvolution` - 自然进化
- `PsionicEvolution` - 灵能进化
- `SymbioticEvolution` - 共生进化

**生态事件类型**：
- `Migration` - 生物迁徙
- `Disaster` - 生态灾难
- `Invasion` - 物种入侵
- `PsionicSurge` - 灵能潮涌

**共生生物信息结构**：

```cpp
struct SymbiontInfo {
    QString id;                  // ID
    QString name;                // 名称
    SymbiontType type;           // 类型
    QString description;         // 描述
    float trustLevel;            // 信任度
    int symbiosisLevel;          // 共生等级
    EvolutionPath evolutionPath; // 进化路线
    bool unlocked;               // 是否解锁
    bool active;                 // 是否活跃
    QStringList abilities;       // 能力列表
};
```

**主要方法**：

```cpp
// 初始化
bool initialize();

// 关闭
void shutdown();

// 更新
void update(float deltaTime);

// 共生生物管理
bool unlockSymbiont(const QString& id);
SymbiontInfo getSymbiontInfo(const QString& id) const;
bool isSymbiontUnlocked(const QString& id) const;
QList<QString> getUnlockedSymbionts() const;

// 共生关系
bool establishSymbiosis(const QString& symbiontId);
bool breakSymbiosis(const QString& symbiontId);
bool addTrust(const QString& symbiontId, float amount);
float getTrustLevel(const QString& symbiontId) const;

// 共生进化
bool evolveSymbiont(const QString& symbiontId);
int getSymbiosisLevel(const QString& symbiontId) const;

// 共生能力
bool activateAbility(const QString& symbiontId, const QString& abilityId);
QStringList getAbilities(const QString& symbiontId) const;

// 生态平衡
float getEcoIndex() const;
void setEcoIndex(float value);
void addEcoIndex(float amount);
void reduceEcoIndex(float amount);

// 生态事件
bool triggerEcoEvent(EcoEventType type);
EcoEventType getCurrentEcoEvent() const;

// 生物多样性
void discoverCreature(const QString& creatureId);
int getDiscoveredCreatureCount() const;
```

**信号**：
- `symbiontUnlocked(const QString& id)`
- `symbiosisEstablished(const QString& symbiontId)`
- `symbiosisBroken(const QString& symbiontId)`
- `trustLevelChanged(const QString& symbiontId, float level)`
- `symbiontEvolved(const QString& symbiontId, int newLevel)`
- `ecoIndexChanged(float value)`
- `ecoEventTriggered(EcoEventType type)`
- `creatureDiscovered(const QString& creatureId)`

**使用示例**：

```cpp
// 创建生态系统
EcosystemSystem ecosystemSystem;
ecosystemSystem.initialize();

// 解锁共生生物
if (ecosystemSystem.unlockSymbiont("时空蝶")) {
    qDebug() << "解锁时空蝶";
}

// 建立共生关系
ecosystemSystem.establishSymbiosis("时空蝶");

// 增加信任度
ecosystemSystem.addTrust("时空蝶", 15.0f);

// 共生进化
ecosystemSystem.evolveSymbiont("时空蝶");

// 激活能力
ecosystemSystem.activateAbility("时空蝶", "时间感知");

// 改变生态指数
ecosystemSystem.addEcoIndex(10.0f);

// 触发生态事件
ecosystemSystem.triggerEcoEvent(EcosystemSystem::Migration);
```

---

### 5. GameManager（游戏管理器）

**文件位置**：
- 头文件：`include/game/GameManager.h`
- 实现文件：`src/game/GameManager.cpp`

**功能概述**：
游戏管理器，负责游戏状态管理、系统集成、场景切换、存档等。

**游戏状态**：
- `MainMenu` - 主菜单
- `Loading` - 加载中
- `Playing` - 游戏中
- `Paused` - 暂停
- `GameOver` - 游戏结束
- `Victory` - 胜利

**主要方法**：

```cpp
// 初始化
bool initialize();

// 关闭
void shutdown();

// 更新
void update(float deltaTime);

// 游戏状态
GameState getGameState() const;
void setGameState(GameState state);

// 游戏控制
bool startNewGame();
void pauseGame();
void resumeGame();
void returnToMainMenu();

// 系统访问
Player* getPlayer() const;
TimeSystem* getTimeSystem() const;
ResonanceSystem* getResonanceSystem() const;
EcosystemSystem* getEcosystemSystem() const;

// 游戏信息
float getGameTime() const;
int getGameDay() const;
QString getCurrentSceneName() const;

// 场景切换
bool switchScene(const QString& sceneName);

// 存档
bool saveGame(int slot);
bool loadGame(int slot);
bool quickSave();
bool quickLoad();
```

**信号**：
- `gameStateChanged(GameState state)`
- `gameStarted()`
- `gamePaused()`
- `gameResumed()`
- `gameOver()`
- `sceneChanged(const QString& sceneName)`
- `gameSaved(int slot)`
- `gameLoaded(int slot)`

**使用示例**：

```cpp
// 创建游戏管理器
GameManager gameManager;
gameManager.initialize();

// 开始新游戏
if (gameManager.startNewGame()) {
    qDebug() << "新游戏开始";
}

// 暂停游戏
gameManager.pauseGame();

// 恢复游戏
gameManager.resumeGame();

// 切换场景
gameManager.switchScene("new_eden");

// 快速保存
gameManager.quickSave();

// 快速加载
gameManager.quickLoad();

// 返回主菜单
gameManager.returnToMainMenu();
```

---

### 6. ExampleScene（示例场景）

**文件位置**：
- 头文件：`include/game/ExampleScene.h`
- 实现文件：`src/game/ExampleScene.cpp`

**功能概述**：
示例场景类，展示如何创建一个游戏场景，包含玩家、地形、UI 等元素。

**主要方法**：

```cpp
// 初始化场景
bool initialize();

// 更新场景
void update(float deltaTime);

// 设置玩家位置
void setPlayerPosition(float x, float y);

// 获取玩家位置
QPointF getPlayerPosition() const;
```

**场景内容**：
- 等距视角地形网格（20×15）
- 玩家角色图形
- 装饰性星星
- 场景标题

**使用示例**：

```cpp
// 创建示例场景
ExampleScene scene;
scene.initialize();

// 设置玩家位置
scene.setPlayerPosition(100.0f, 50.0f);

// 更新场景
scene.update(deltaTime);
```

---

## 渲染系统详解

### RenderManager（渲染管理器）

**文件位置**：
- 头文件：`include/render/RenderManager.h`
- 实现文件：`src/render/RenderManager.cpp`

**功能概述**：
负责 2.5D 箱庭场景的渲染管理，基于 Qt Graphics View Framework。

**主要方法**：

```cpp
// 初始化渲染系统
bool initialize(QGraphicsView* view);

// 关闭渲染系统
void shutdown();

// 更新渲染
void update(float deltaTime);

// 场景管理
QGraphicsScene* getCurrentScene() const;
QGraphicsScene* createScene(const QString& name);
QGraphicsScene* getScene(const QString& name) const;
bool switchScene(const QString& name);
bool removeScene(const QString& name);

// 渲染设置
void setBackgroundColor(const QColor& color);
void setScale(float scale);
float getScale() const;
void setAntialiasing(bool enabled);
void setVSync(bool enabled);

// 相机控制
void setCameraPosition(float x, float y);
QPointF getCameraPosition() const;
void moveCamera(float dx, float dy);

// 等距视角
void setIsometricView(bool enabled);
bool isIsometricView() const;
```

**信号**：
- `sceneChanged(const QString& sceneName)`
- `renderError(const QString& error)`

**等距视角实现**：
通过压缩 Y 轴（scale 0.5）模拟等距视角效果。

**使用示例**：

```cpp
// 创建渲染管理器
RenderManager renderManager;

// 初始化（需要 QGraphicsView）
QGraphicsView* view = ...;
if (renderManager.initialize(view)) {
    qDebug() << "渲染系统初始化成功";
}

// 创建场景
QGraphicsScene* scene = renderManager.createScene("main_scene");

// 切换场景
renderManager.switchScene("main_scene");

// 设置缩放
renderManager.setScale(1.5f);

// 移动相机
renderManager.moveCamera(10.0f, 5.0f);

// 设置等距视角
renderManager.setIsometricView(true);

// 设置背景颜色
renderManager.setBackgroundColor(QColor(20, 20, 40));
```

---

## UI系统详解

### UIManager（UI 管理器）

**文件位置**：
- 头文件：`include/ui/UIManager.h`
- 实现文件：`src/ui/UIManager.cpp`

**功能概述**：
负责游戏 UI 的管理和切换。

**UI 类型**：
- `MainMenu` - 主菜单
- `GameHUD` - 游戏 HUD
- `PauseMenu` - 暂停菜单
- `Inventory` - 背包
- `SkillTree` - 技能树
- `Dialog` - 对话框
- `Settings` - 设置
- `GameOver` - 游戏结束
- `Victory` - 胜利

**主要方法**：

```cpp
// 初始化 UI 系统
bool initialize(QWidget* parentWidget);

// 关闭 UI 系统
void shutdown();

// 更新 UI
void update(float deltaTime);

// UI 管理
bool showUI(UIType type);
bool hideUI(UIType type);
bool switchToUI(UIType type);
bool isUIVisible(UIType type) const;
void hideAllUI();

// HUD 更新
void updateHealth(float current, float max);
void updateMana(float current, float max);
void updateExperience(int current, int level);
void updateTimeEntropy(float value);
void updateStability(float value);
void updateMinimap(const QPointF& playerPos);

// 对话框
void showDialog(const QString& speaker, const QString& text);
void hideDialog();
void showOptions(const QString& question, const QStringList& options);

// 通知系统
void showNotification(const QString& message, float duration = 3.0f);
void showAchievementNotification(const QString& achievementName);
```

**信号**：
- `uiShown(UIType type)`
- `uiHidden(UIType type)`
- `optionSelected(int optionIndex)`
- `dialogClosed()`

**默认 UI**：
- 主菜单（带标题和按钮）
- 游戏 HUD（占位）
- 暂停菜单（占位）
- 其他 UI（占位）

**使用示例**：

```cpp
// 创建 UI 管理器
UIManager uiManager;

// 初始化（需要父窗口）
QWidget* parent = ...;
if (uiManager.initialize(parent)) {
    qDebug() << "UI 系统初始化成功";
}

// 显示主菜单
uiManager.showUI(UIManager::MainMenu);

// 切换到游戏 HUD
uiManager.switchToUI(UIManager::GameHUD);

// 更新 HUD
uiManager.updateHealth(80.0f, 100.0f);
uiManager.updateMana(35.0f, 50.0f);
uiManager.updateExperience(150, 3);
uiManager.updateTimeEntropy(25.0f);
uiManager.updateStability(80.0f);

// 显示对话框
uiManager.showDialog("艾拉", "欢迎来到星溯旅人！");

// 显示通知
uiManager.showNotification("获得新道具", 3.0f);

// 显示成就通知
uiManager.showAchievementNotification("初次探索");
```

---

## 编译与运行

### Qt Creator 编译步骤

1. **打开项目**
   - 启动 Qt Creator
   - 文件 → 打开文件或项目
   - 选择项目根目录下的 `CMakeLists.txt`
   - 点击"打开"

2. **配置项目**
   - 选择构建套件：Desktop Qt 6.10.1 MinGW 64-bit
   - 选择构建类型：Debug 或 Release
   - 点击"配置项目"

3. **构建项目**
   - 点击左下角的锤子图标（构建项目）
   - 或按快捷键 Ctrl+B
   - 等待编译完成

4. **运行项目**
   - 点击左下角的绿色三角形图标（运行项目）
   - 或按快捷键 Ctrl+R
   - 游戏窗口将弹出

### 命令行编译步骤

1. **打开命令行**
   - 打开 Qt 命令行工具（Qt 6.10.1 (MinGW 13.1.0 64-bit)）
   - 或打开普通命令行，设置 Qt 环境变量

2. **进入项目目录**
   ```bash
   cd /path/to/star_retrospect_traveler
   ```

3. **创建构建目录**
   ```bash
   mkdir build
   cd build
   ```

4. **配置 CMake**
   ```bash
   cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
   ```

5. **编译**
   ```bash
   mingw32-make -j4
   ```

6. **运行**
   ```bash
   cd bin
   StarRetrospectTraveler.exe
   ```

### MinGW 专用优化

项目已配置以下 MinGW 专用优化：

- **`-Wa,-mbig-obj`**：支持大目标文件，避免 Debug 模式下文件过大报错
- **`-static-libgcc -static-libstdc++`**：静态链接运行库，避免依赖 dll
- **`-O3`**：Release 模式最高级别优化
- **`-s`**：Release 模式剥离符号信息，减小可执行文件体积

### 常见编译错误

1. **找不到 Qt 模块**
   - 确保 Qt 路径正确配置
   - 检查 CMakeLists.txt 中的 find_package

2. **undefined reference**
   - 检查是否所有源文件都添加到 CMakeLists.txt
   - 检查链接库是否正确

3. **big-obj 错误**
   - 已配置 `-Wa,-mbig-obj`，应该不会出现
   - 如果出现，检查是否有超大源文件

---

## 开发流程

### 1. 添加新功能的步骤

1. **设计功能**
   - 明确功能需求
   - 设计类结构和接口
   - 考虑与现有系统的交互

2. **创建头文件**
   - 在 `include/` 对应模块目录下创建头文件
   - 声明类和方法
   - 添加必要的注释

3. **创建实现文件**
   - 在 `src/` 对应模块目录下创建实现文件
   - 实现所有方法
   - 添加调试输出

4. **更新 CMakeLists.txt**
   - 在对应模块的 CMakeLists.txt 中添加源文件
   - 添加必要的链接库

5. **测试功能**
   - 编译项目
   - 测试新功能
   - 修复 bug

6. **优化和完善**
   - 性能优化
   - 代码优化
   - 添加更多注释

### 2. 添加新场景的步骤

1. **创建场景类**
   - 继承 QGraphicsScene
   - 实现 initialize() 和 update() 方法
   - 创建场景元素

2. **注册场景**
   - 在 SceneManager 中注册
   - 或在 GameManager 中创建

3. **添加场景切换逻辑**
   - 在合适的地方调用场景切换
   - 处理场景切换时的数据传递

### 3. 添加新系统的步骤

1. **设计系统**
   - 明确系统的职责
   - 设计系统的接口
   - 考虑与其他系统的交互

2. **创建系统类**
   - 在 `include/game/` 和 `src/game/` 中创建
   - 实现初始化、更新、关闭方法
   - 添加信号和槽

3. **集成到 GameManager**
   - 在 GameManager 中添加系统实例
   - 在 initialize() 中初始化
   - 在 update() 中更新
   - 在 shutdown() 中关闭

4. **添加访问接口**
   - 在 GameManager 中添加 getter 方法
   - 让其他系统可以访问

### 4. 代码规范

**命名规范**：
- 类名：大驼峰命名法（PascalCase）
- 方法名：小驼峰命名法（camelCase）
- 变量名：小驼峰命名法（camelCase）
- 成员变量：m_ 前缀 + 小驼峰
- 常量：全大写 + 下划线
- 枚举：大驼峰

**格式规范**：
- 缩进：4 个空格
- 大括号：独占一行
- 行宽：不超过 120 字符
- 空行：逻辑块之间加空行

**注释规范**：
- 文件头注释：说明文件用途
- 类注释：说明类的功能
- 方法注释：说明方法的用途、参数、返回值
- 复杂逻辑：添加行内注释

---

## 常见问题

### 1. 游戏窗口不显示？
- 检查是否正确调用了 `show()`
- 检查窗口大小是否合理
- 检查是否有错误输出

### 2. 场景内容不显示？
- 检查场景是否正确创建
- 检查图形项是否添加到场景
- 检查视图是否设置了场景
- 检查缩放和相机位置

### 3. 编译错误：undefined reference to vtable？
- 检查类是否继承了 QObject
- 检查是否添加了 Q_OBJECT 宏
- 检查是否运行了 moc（CMake 会自动处理）

### 4. 运行时崩溃？
- 检查空指针
- 检查数组越界
- 使用 Debug 模式和调试器定位问题
- 启用 AddressSanitizer（仅 MSVC/GCC）

### 5. 如何添加新的 Qt 模块？
- 在主 CMakeLists.txt 的 find_package 中添加
- 在对应模块的 CMakeLists.txt 的 target_link_libraries 中添加

### 6. 如何添加资源文件？
- 将资源文件放到 `assets/` 对应目录
- 使用 ResourceManager 加载
- 或使用 Qt 资源系统（.qrc 文件）

### 7. 如何创建插件？
- 继承 IPlugin 接口
- 实现必要的方法
- 编译为动态库
- 放到 plugins 目录

---

## 下一步开发计划

### 第二阶段：核心引擎开发（第 2-4 个月）

1. **完善图形渲染**
   - 实现 2.5D 等距视角渲染
   - 添加精灵动画系统
   - 实现粒子效果
   - 添加光照和阴影

2. **完善音频系统**
   - 实现背景音乐播放
   - 实现音效管理
   - 添加音频淡入淡出
   - 实现 3D 音效（可选）

3. **完善数据系统**
   - 实现加密 JSON 存档
   - 实现配置管理
   - 添加数据校验
   - 实现多槽位存档

4. **完善 UI 系统**
   - 实现主菜单
   - 实现游戏 HUD
   - 实现背包界面
   - 实现技能树界面

### 第三阶段：游戏系统开发（第 5-9 个月）

1. **完善时间褶皱系统**
   - 实现时间线切换
   - 实现时间悖论
   - 添加时间织网
   - 实现时间克隆

2. **完善星核共鸣系统**
   - 实现星核对话
   - 实现双向共鸣
   - 添加星核关系网络
   - 实现共鸣组合技

3. **完善生态共生系统**
   - 实现共生生物 AI
   - 实现基因融合
   - 添加生态意识网络
   - 实现共生体进化

4. **完善战斗系统**
   - 实现战斗机制
   - 添加敌人 AI
   - 实现技能系统
   - 添加战斗特效

### 第四阶段：内容制作（第 10-18 个月）

1. **关卡制作**
   - 创建 12 颗星球
   - 设计箱庭关卡
   - 添加解谜元素
   - 实现隐藏内容

2. **美术资源**
   - 角色设计
   - 场景设计
   - UI 设计
   - 特效设计

3. **剧情与任务**
   - 主线剧情
   - 支线任务
   - 对话系统
   - 多结局

### 第五阶段：测试与优化（第 19-22 个月）

1. **功能测试**
   - 单元测试
   - 集成测试
   - 游戏测试
   - Bug 修复

2. **性能优化**
   - 渲染优化
   - 内存优化
   - 加载优化
   - 多线程优化

3. **平衡性调整**
   - 数值平衡
   - 难度调整
   - 玩法平衡
   - 经济平衡

### 第六阶段：发布准备（第 23-24 个月）

1. **打包发布**
   - Windows 版本
   - macOS 版本（可选）
   - Linux 版本（可选）
   - 安装包制作

2. **宣发准备**
   - 预告片制作
   - 商店页面
   - 媒体宣传
   - 社区运营

---

## 总结

本项目是一个完整的 2.5D 箱庭探索策略 RPG 游戏框架，基于 Qt 6.10.1 和 C++ 17 开发。

**已完成的工作**：
- ✅ 项目目录结构
- ✅ CMake 配置（支持 MinGW-w64）
- ✅ 核心模块（GameEngine、GameConfig、SceneManager、ResourceManager、InputManager、AudioManager、PluginManager）
- ✅ 游戏模块（Player、TimeSystem、ResonanceSystem、EcosystemSystem、GameManager）
- ✅ 渲染模块（RenderManager）
- ✅ UI 模块（UIManager）
- ✅ 示例场景（ExampleScene）
- ✅ 项目说明文档
- ✅ MinGW 编译指南
- ✅ 完整开发指南（本文档）

**下一步工作**：
- 完善各系统的细节实现
- 添加更多游戏内容
- 优化性能和体验
- 添加测试代码

希望这份开发指南能帮助你快速理解项目结构和开发流程。如果有任何问题，欢迎随时提问！

---

**文档版本**：v1.0
**最后更新**：2026-06-19
