# 星溯旅人 - 第一阶段开发指南（项目初始化）

> **阶段名称**：项目初始化  
> **开发周期**：第1个月  
> **技术栈**：Qt 6.10.1 + CMake + C++17  
> **编译器支持**：MinGW-w64 / MSVC / GCC  

---

## 📋 阶段目标

完成项目的基础搭建，建立可运行的游戏引擎框架，为后续开发奠定基础。

---

## 🗂️ 目录

1. [步骤1：环境搭建](#步骤1环境搭建)
2. [步骤2：项目结构搭建](#步骤2项目结构搭建)
3. [步骤3：基础框架搭建](#步骤3基础框架搭建)
4. [步骤4：配置文件与资源](#步骤4配置文件与资源)
5. [编译运行](#编译运行)

---

## 步骤1：环境搭建

### 1.1 安装 Qt 6.10.1

**下载地址**：https://www.qt.io/download

**安装组件**（MinGW-w64 版本）：
- ✅ Qt 6.10.1 → MinGW 13.1.0 64-bit
- ✅ Qt 6.10.1 → Qt Multimedia
- ✅ Qt 6.10.1 → Qt Quick 3D（可选，用于UI特效）
- ✅ Qt 6.10.1 → Qt Shader Tools
- ✅ Qt 6.10.1 → Qt Svg
- ✅ Developer and Designer Tools → Qt Creator
- ✅ Developer and Designer Tools → CMake
- ✅ Developer and Designer Tools → Ninja

### 1.2 配置 Qt Kits

打开 Qt Creator → 工具 → 选项 → Kits：

1. **编译器**：确认 MinGW 64-bit 编译器已检测到
2. **调试器**：确认 GDB 调试器已配置
3. **Qt 版本**：确认 Qt 6.10.1 (mingw_64) 已添加
4. **构建套件**：确认 "Desktop Qt 6.10.1 MinGW 64-bit" 存在

### 1.3 安装 Git

下载并安装 Git for Windows：https://git-scm.com/download/win

配置 Git：
```bash
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

---

## 步骤2：项目结构搭建

### 2.1 创建目录结构

```
star_retrospect_traveler/
├── CMakeLists.txt              # 主 CMake 配置
├── README.md                   # 项目说明
├── assets/                     # 资源目录
│   ├── config/                 # 配置文件
│   │   └── game.json           # 游戏主配置
│   ├── images/                 # 图片资源
│   ├── audio/                  # 音频资源
│   └── fonts/                  # 字体资源
├── include/                    # 头文件目录
│   └── core/                   # 核心模块头文件
│       ├── GameEngine.h
│       ├── GameConfig.h
│       ├── SceneManager.h
│       ├── ResourceManager.h
│       ├── InputManager.h
│       ├── AudioManager.h
│       └── PluginManager.h
├── src/                        # 源代码目录
│   ├── CMakeLists.txt          # 主程序 CMake 配置
│   ├── main.cpp                # 主程序入口
│   └── core/                   # 核心模块源码
│       ├── CMakeLists.txt
│       ├── GameEngine.cpp
│       ├── GameConfig.cpp
│       ├── SceneManager.cpp
│       ├── ResourceManager.cpp
│       ├── InputManager.cpp
│       ├── AudioManager.cpp
│       └── PluginManager.cpp
├── docs/                       # 文档目录
└── tests/                      # 测试目录
```

### 2.2 主 CMakeLists.txt

**文件路径**：`CMakeLists.txt`

```cmake
# ============================================================
# 星溯旅人 - 主CMake配置文件
# 项目名称: Star Retrospect Traveler
# 版本: 0.1.0
# 支持编译器: MSVC, MinGW-w64, GCC, Clang
# ============================================================

cmake_minimum_required(VERSION 3.20)

project(StarRetrospectTraveler
    VERSION 0.1.0
    DESCRIPTION "星溯旅人 - 2.5D箱庭探索策略RPG"
    LANGUAGES CXX
)

# ============================================================
# C++标准配置
# ============================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
# 编译选项配置
# ============================================================
if(MSVC)
    # MSVC编译器选项
    add_compile_options(/W4 /utf-8)
elseif(MINGW)
    # ========================================================
    # MinGW-w64 专用配置
    # ========================================================
    
    # 警告选项
    add_compile_options(-Wall -Wextra -Wpedantic)
    
    # 支持大目标文件（Debug模式下避免文件过大报错）
    add_compile_options(-Wa,-mbig-obj)
    
    # 静态链接运行库，避免依赖 libgcc_s_seh-1.dll, libstdc++-6.dll 等
    add_link_options(-static-libgcc -static-libstdc++)
    
    # Release模式额外优化
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        # -O3: 最高级别优化
        # -s: 剥离符号信息，减小可执行文件体积
        add_compile_options(-O3)
        add_link_options(-s)
    endif()
    
    message(STATUS "检测到 MinGW-w64 编译器，已应用专用优化")
else()
    # GCC/Clang 通用选项
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ============================================================
# AddressSanitizer (ASAN) 支持
# ============================================================
option(ENABLE_ASAN "启用AddressSanitizer内存检测" OFF)

if(ENABLE_ASAN AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MINGW)
        message(WARNING "MinGW下AddressSanitizer支持不完善，已自动禁用")
    else()
        add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address)
        message(STATUS "已启用 AddressSanitizer 内存检测")
    endif()
endif()

# ============================================================
# Qt配置
# ============================================================
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.10 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Multimedia
    Network
    Quick
    Qml
    Svg
)

# ============================================================
# 输出目录配置
# ============================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ============================================================
# 包含目录
# ============================================================
include_directories(${CMAKE_SOURCE_DIR}/include)

# ============================================================
# 子目录 - 核心模块层
# ============================================================
add_subdirectory(src/core)

# ============================================================
# 主程序
# ============================================================
add_subdirectory(src)

# ============================================================
# 配置摘要输出
# ============================================================
message(STATUS "")
message(STATUS "========================================")
message(STATUS "  星溯旅人 - 项目配置摘要")
message(STATUS "========================================")
message(STATUS "  项目名称:    ${PROJECT_NAME}")
message(STATUS "  项目版本:    ${PROJECT_VERSION}")
message(STATUS "  C++标准:     C++${CMAKE_CXX_STANDARD}")
message(STATUS "  构建类型:    ${CMAKE_BUILD_TYPE}")
message(STATUS "  编译器:      ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  编译器版本:  ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Qt版本:      ${Qt6_VERSION}")
message(STATUS "========================================")
message(STATUS "")
```

### 2.3 主程序 CMakeLists.txt

**文件路径**：`src/CMakeLists.txt`

```cmake
# ============================================================
# 主程序 CMake 配置
# ============================================================

# ------------------------------------------------------------
# 创建可执行文件
# 
# WIN32选项说明:
# - 在Windows平台，添加WIN32可以创建GUI应用（无控制台窗口）
# - Debug模式下保留控制台，便于查看调试输出
# - Release模式下隐藏控制台，更像正式游戏
# ------------------------------------------------------------
if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    # Release模式: GUI应用，无控制台窗口
    add_executable(${PROJECT_NAME} WIN32
        main.cpp
    )
else()
    # Debug模式或非Windows: 保留控制台窗口
    add_executable(${PROJECT_NAME}
        main.cpp
    )
endif()

# ------------------------------------------------------------
# 链接库
# ------------------------------------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE
    # Qt模块
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    
    # 核心模块
    srt_core
)

# ------------------------------------------------------------
# 复制资源文件到输出目录
# 
# POST_BUILD: 构建完成后执行
# 将assets目录复制到可执行文件所在目录
# 这样游戏运行时能找到资源文件
# ------------------------------------------------------------
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMENT "复制资源文件到输出目录..."
)

# ------------------------------------------------------------
# 设置输出名称
# ------------------------------------------------------------
set_target_properties(${PROJECT_NAME} PROPERTIES
    # Debug模式下添加d后缀，便于区分
    DEBUG_POSTFIX "d"
)
```

### 2.4 核心模块 CMakeLists.txt

**文件路径**：`src/core/CMakeLists.txt`

```cmake
# ============================================================
# 核心模块 - CMake 配置
# 模块名称: srt_core (Star Retrospect Core)
# 模块类型: 静态库
# ============================================================

# ------------------------------------------------------------
# 创建静态库
# ------------------------------------------------------------
add_library(srt_core STATIC
    # 游戏引擎核心
    GameEngine.cpp
    
    # 配置管理
    GameConfig.cpp
    
    # 场景管理
    SceneManager.cpp
    
    # 资源管理
    ResourceManager.cpp
    
    # 输入管理
    InputManager.cpp
    
    # 音频管理
    AudioManager.cpp
    
    # 插件管理
    PluginManager.cpp
)

# ------------------------------------------------------------
# 包含目录
# ------------------------------------------------------------
target_include_directories(srt_core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# ------------------------------------------------------------
# 链接库
# ------------------------------------------------------------
target_link_libraries(srt_core PUBLIC
    # Qt 基础模块
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    
    # Qt 多媒体模块
    Qt6::Multimedia
    
    # Qt SVG 模块
    Qt6::Svg
    
    # Qt 网络模块
    Qt6::Network
)

# ------------------------------------------------------------
# 编译选项
# ------------------------------------------------------------
if(MSVC)
    target_compile_options(srt_core PRIVATE /W4)
elseif(MINGW)
    target_compile_options(srt_core PRIVATE -Wall -Wextra)
else()
    target_compile_options(srt_core PRIVATE -Wall -Wextra -Wpedantic)
endif()
```

---

## 步骤3：基础框架搭建

### 3.1 游戏配置类 (GameConfig)

负责管理游戏的各种配置选项，支持从 JSON 文件加载和保存。

#### 头文件

**文件路径**：`include/core/GameConfig.h`

```cpp
/**
 * @file GameConfig.h
 * @brief 游戏配置类
 * 
 * 负责管理游戏的各种配置选项，包括窗口、图形、音频、控制等设置。
 * 支持从 JSON 文件加载配置和保存配置到文件。
 */

#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <QString>
#include <QSize>

/**
 * @brief 游戏配置结构体
 * 
 * 包含游戏的各种配置选项
 */
struct GameConfig
{
    // ========================================================
    // 基本配置
    // ========================================================
    
    /// 游戏标题
    QString gameTitle = "星溯旅人";
    
    /// 游戏版本
    QString version = "0.1.0";
    
    // ========================================================
    // 窗口配置
    // ========================================================
    
    /// 窗口大小（默认1280x720）
    QSize windowSize = QSize(1280, 720);
    
    /// 是否全屏模式
    bool fullscreen = false;
    
    /// 是否启用垂直同步
    bool vsync = true;
    
    // ========================================================
    // 图形配置
    // ========================================================
    
    /// 目标帧率（默认60FPS）
    int targetFPS = 60;
    
    /// 是否启用粒子效果
    bool enableParticles = true;
    
    /// 纹理质量（0:低, 1:中, 2:高）
    int textureQuality = 2;
    
    // ========================================================
    // 音频配置
    // ========================================================
    
    /// 主音量（0.0 - 1.0）
    float masterVolume = 1.0f;
    
    /// 音乐音量（0.0 - 1.0）
    float musicVolume = 0.8f;
    
    /// 音效音量（0.0 - 1.0）
    float sfxVolume = 1.0f;
    
    /// 是否静音
    bool muteAudio = false;
    
    // ========================================================
    // 游戏配置
    // ========================================================
    
    /// 是否调试模式
    bool debugMode = false;
    
    /// 是否显示FPS
    bool showFPS = false;
    
    /// 是否显示碰撞体
    bool showCollision = false;
    
    /// 语言设置
    QString language = "zh_CN";
    
    // ========================================================
    // 控制配置
    // ========================================================
    
    /// 鼠标灵敏度
    float mouseSensitivity = 1.0f;
    
    /// 是否反转Y轴
    bool invertY = false;
    
    // ========================================================
    // 成员函数
    // ========================================================
    
    /**
     * @brief 从文件加载配置
     * @param filePath 配置文件路径
     * @return 是否加载成功
     */
    bool loadFromFile(const QString& filePath);
    
    /**
     * @brief 保存配置到文件
     * @param filePath 配置文件路径
     * @return 是否保存成功
     */
    bool saveToFile(const QString& filePath) const;
    
    /**
     * @brief 重置为默认配置
     */
    void resetToDefaults();
};

#endif // GAME_CONFIG_H
```

#### 实现文件

**文件路径**：`src/core/GameConfig.cpp`

```cpp
/**
 * @file GameConfig.cpp
 * @brief 游戏配置类实现
 */

#include "core/GameConfig.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

bool GameConfig::loadFromFile(const QString& filePath)
{
    // 打开配置文件
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "[GameConfig] 无法打开配置文件:" << filePath;
        return false;
    }
    
    // 读取并解析JSON
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "[GameConfig] 配置文件格式错误";
        return false;
    }
    
    QJsonObject obj = doc.object();
    
    // ========================================================
    // 窗口配置
    // ========================================================
    if (obj.contains("windowWidth")) {
        windowSize.setWidth(obj["windowWidth"].toInt());
    }
    if (obj.contains("windowHeight")) {
        windowSize.setHeight(obj["windowHeight"].toInt());
    }
    fullscreen = obj["fullscreen"].toBool(fullscreen);
    vsync = obj["vsync"].toBool(vsync);
    
    // ========================================================
    // 图形配置
    // ========================================================
    targetFPS = obj["targetFPS"].toInt(targetFPS);
    enableParticles = obj["enableParticles"].toBool(enableParticles);
    textureQuality = obj["textureQuality"].toInt(textureQuality);
    
    // ========================================================
    // 音频配置
    // ========================================================
    masterVolume = static_cast<float>(obj["masterVolume"].toDouble(masterVolume));
    musicVolume = static_cast<float>(obj["musicVolume"].toDouble(musicVolume));
    sfxVolume = static_cast<float>(obj["sfxVolume"].toDouble(sfxVolume));
    muteAudio = obj["muteAudio"].toBool(muteAudio);
    
    // ========================================================
    // 游戏配置
    // ========================================================
    debugMode = obj["debugMode"].toBool(debugMode);
    showFPS = obj["showFPS"].toBool(showFPS);
    showCollision = obj["showCollision"].toBool(showCollision);
    language = obj["language"].toString(language);
    
    // ========================================================
    // 控制配置
    // ========================================================
    mouseSensitivity = static_cast<float>(obj["mouseSensitivity"].toDouble(mouseSensitivity));
    invertY = obj["invertY"].toBool(invertY);
    
    qDebug() << "[GameConfig] 配置加载成功:" << filePath;
    return true;
}

bool GameConfig::saveToFile(const QString& filePath) const
{
    QJsonObject obj;
    
    // ========================================================
    // 窗口配置
    // ========================================================
    obj["windowWidth"] = windowSize.width();
    obj["windowHeight"] = windowSize.height();
    obj["fullscreen"] = fullscreen;
    obj["vsync"] = vsync;
    
    // ========================================================
    // 图形配置
    // ========================================================
    obj["targetFPS"] = targetFPS;
    obj["enableParticles"] = enableParticles;
    obj["textureQuality"] = textureQuality;
    
    // ========================================================
    // 音频配置
    // ========================================================
    obj["masterVolume"] = masterVolume;
    obj["musicVolume"] = musicVolume;
    obj["sfxVolume"] = sfxVolume;
    obj["muteAudio"] = muteAudio;
    
    // ========================================================
    // 游戏配置
    // ========================================================
    obj["debugMode"] = debugMode;
    obj["showFPS"] = showFPS;
    obj["showCollision"] = showCollision;
    obj["language"] = language;
    
    // ========================================================
    // 控制配置
    // ========================================================
    obj["mouseSensitivity"] = mouseSensitivity;
    obj["invertY"] = invertY;
    
    // 写入文件
    QJsonDocument doc(obj);
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "[GameConfig] 无法写入配置文件:" << filePath;
        return false;
    }
    
    file.write(doc.toJson(QJsonDocument::Indented));
    qDebug() << "[GameConfig] 配置保存成功:" << filePath;
    return true;
}

void GameConfig::resetToDefaults()
{
    *this = GameConfig();
    qDebug() << "[GameConfig] 已重置为默认配置";
}
```

### 3.2 游戏引擎类 (GameEngine)

游戏引擎的核心类，负责主循环、子系统管理、初始化和关闭。

#### 头文件

**文件路径**：`include/core/GameEngine.h`

```cpp
/**
 * @file GameEngine.h
 * @brief 游戏引擎核心类
 * 
 * 游戏引擎的核心类，负责：
 * - 游戏的初始化和关闭
 * - 游戏主循环（更新+渲染）
 * - 各个子系统的管理
 * - FPS统计和游戏时间管理
 */

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <QMainWindow>

#include "core/GameConfig.h"

// 前向声明（避免循环依赖）
class SceneManager;
class ResourceManager;
class InputManager;
class AudioManager;
class PluginManager;

/**
 * @brief 游戏引擎核心类
 * 
 * 负责游戏的初始化、主循环、子系统管理等核心功能
 */
class GameEngine : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param config 游戏配置
     * @param parent 父对象
     */
    explicit GameEngine(const GameConfig& config, QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~GameEngine() override;

    // ========================================================
    // 生命周期
    // ========================================================
    
    /**
     * @brief 初始化游戏引擎
     * 
     * 依次初始化主窗口、各个子系统、游戏定时器
     * 
     * @return 是否初始化成功
     */
    bool initialize();
    
    /**
     * @brief 运行游戏主循环
     * 
     * 启动游戏定时器，进入Qt事件循环
     * 
     * @return 退出码
     */
    int run();
    
    /**
     * @brief 关闭游戏引擎
     * 
     * 按创建逆序关闭各个子系统，释放资源
     */
    void shutdown();

    // ========================================================
    // 子系统访问
    // ========================================================
    
    /// 获取游戏配置
    const GameConfig& getConfig() const { return m_config; }
    
    /// 获取场景管理器
    SceneManager* getSceneManager() const { return m_sceneManager; }
    
    /// 获取资源管理器
    ResourceManager* getResourceManager() const { return m_resourceManager; }
    
    /// 获取输入管理器
    InputManager* getInputManager() const { return m_inputManager; }
    
    /// 获取音频管理器
    AudioManager* getAudioManager() const { return m_audioManager; }
    
    /// 获取插件管理器
    PluginManager* getPluginManager() const { return m_pluginManager; }
    
    /// 获取主窗口
    QMainWindow* getMainWindow() const { return m_mainWindow; }

    // ========================================================
    // 时间与性能
    // ========================================================
    
    /// 获取当前FPS
    float getCurrentFPS() const { return m_currentFPS; }
    
    /// 获取游戏运行时间（秒）
    float getGameTime() const { return m_gameTime; }

signals:
    /// 游戏初始化完成信号
    void initialized();
    
    /// 游戏即将关闭信号
    void shuttingDown();

private slots:
    /**
     * @brief 游戏主循环更新
     * @param deltaTime 时间增量（秒）
     */
    void update(float deltaTime);
    
    /**
     * @brief 渲染帧
     */
    void render();

private:
    // ========================================================
    // 初始化辅助函数
    // ========================================================
    
    /// 初始化所有子系统
    bool initializeSubsystems();
    
    /// 初始化主窗口
    bool initializeMainWindow();
    
    /// 计算FPS
    void calculateFPS();

    // ========================================================
    // 成员变量
    // ========================================================
    
    /// 游戏配置
    GameConfig m_config;
    
    /// 主窗口
    QMainWindow* m_mainWindow = nullptr;
    
    // 子系统管理器
    SceneManager* m_sceneManager = nullptr;     ///< 场景管理器
    ResourceManager* m_resourceManager = nullptr; ///< 资源管理器
    InputManager* m_inputManager = nullptr;     ///< 输入管理器
    AudioManager* m_audioManager = nullptr;     ///< 音频管理器
    PluginManager* m_pluginManager = nullptr;   ///< 插件管理器
    
    /// 游戏主循环定时器
    QTimer* m_gameTimer = nullptr;
    
    /// 帧计时器（用于计算deltaTime）
    QElapsedTimer m_frameTimer;
    
    /// 游戏总运行时间（秒）
    float m_gameTime = 0.0f;
    
    /// 当前帧的时间增量（秒）
    float m_deltaTime = 0.0f;
    
    // FPS统计
    float m_currentFPS = 0.0f;  ///< 当前FPS
    int m_frameCount = 0;       ///< 帧计数器
    float m_fpsTimer = 0.0f;    ///< FPS统计计时器
    
    // 游戏状态
    bool m_initialized = false; ///< 是否已初始化
    bool m_running = false;     ///< 是否运行中
    bool m_paused = false;      ///< 是否暂停
};

#endif // GAME_ENGINE_H
```

#### 实现文件

**文件路径**：`src/core/GameEngine.cpp`

```cpp
/**
 * @file GameEngine.cpp
 * @brief 游戏引擎核心类实现
 */

#include "core/GameEngine.h"

#include <QDebug>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>

// 子系统头文件
#include "core/SceneManager.h"
#include "core/ResourceManager.h"
#include "core/InputManager.h"
#include "core/AudioManager.h"
#include "core/PluginManager.h"

GameEngine::GameEngine(const GameConfig& config, QObject* parent)
    : QObject(parent)
    , m_config(config)
{
    qDebug() << "[GameEngine] 创建游戏引擎";
}

GameEngine::~GameEngine()
{
    qDebug() << "[GameEngine] 销毁游戏引擎";
    shutdown();
}

bool GameEngine::initialize()
{
    if (m_initialized) {
        qWarning() << "[GameEngine] 游戏引擎已经初始化";
        return true;
    }

    qDebug() << "[GameEngine] 开始初始化游戏引擎...";

    // --------------------------------------------------------
    // 1. 初始化主窗口
    // --------------------------------------------------------
    if (!initializeMainWindow()) {
        qCritical() << "[GameEngine] 主窗口初始化失败";
        return false;
    }

    // --------------------------------------------------------
    // 2. 初始化所有子系统
    // --------------------------------------------------------
    if (!initializeSubsystems()) {
        qCritical() << "[GameEngine] 子系统初始化失败";
        return false;
    }

    // --------------------------------------------------------
    // 3. 创建游戏主循环定时器
    // --------------------------------------------------------
    m_gameTimer = new QTimer(this);
    
    // 连接定时器超时信号到主循环
    connect(m_gameTimer, &QTimer::timeout, this, [this]() {
        // 计算这一帧的时间增量
        qint64 elapsed = m_frameTimer.restart();
        m_deltaTime = elapsed / 1000.0f;
        
        // 更新游戏逻辑
        update(m_deltaTime);
        
        // 渲染（Qt的视图系统会自动渲染，这里做额外处理）
        render();
        
        // 计算FPS
        calculateFPS();
    });

    // 设置定时器间隔（目标FPS）
    int interval = 1000 / m_config.targetFPS;
    m_gameTimer->setInterval(interval);
    
    qDebug() << "[GameEngine] 目标FPS:" << m_config.targetFPS 
             << "间隔:" << interval << "ms";

    m_initialized = true;
    
    qDebug() << "[GameEngine] 游戏引擎初始化完成";
    emit initialized();
    
    return true;
}

int GameEngine::run()
{
    if (!m_initialized) {
        qCritical() << "[GameEngine] 游戏引擎未初始化，无法运行";
        return -1;
    }

    qDebug() << "[GameEngine] 启动游戏主循环";

    m_running = true;
    m_frameTimer.start();
    m_gameTimer->start();

    // 显示主窗口
    if (m_mainWindow) {
        if (m_config.fullscreen) {
            m_mainWindow->showFullScreen();
        } else {
            m_mainWindow->show();
        }
    }

    // 进入Qt事件循环（阻塞调用，直到程序退出）
    int result = QCoreApplication::exec();

    m_running = false;
    
    qDebug() << "[GameEngine] 游戏主循环结束，返回码:" << result;
    
    return result;
}

void GameEngine::shutdown()
{
    if (!m_initialized) {
        return;
    }

    qDebug() << "[GameEngine] 关闭游戏引擎...";

    emit shuttingDown();

    // 停止游戏定时器
    if (m_gameTimer) {
        m_gameTimer->stop();
    }

    // --------------------------------------------------------
    // 按创建逆序关闭子系统
    // --------------------------------------------------------
    
    if (m_pluginManager) {
        delete m_pluginManager;
        m_pluginManager = nullptr;
    }
    
    if (m_audioManager) {
        delete m_audioManager;
        m_audioManager = nullptr;
    }
    
    if (m_inputManager) {
        delete m_inputManager;
        m_inputManager = nullptr;
    }
    
    if (m_resourceManager) {
        delete m_resourceManager;
        m_resourceManager = nullptr;
    }
    
    if (m_sceneManager) {
        delete m_sceneManager;
        m_sceneManager = nullptr;
    }

    // 关闭主窗口
    if (m_mainWindow) {
        m_mainWindow->close();
        delete m_mainWindow;
        m_mainWindow = nullptr;
    }

    m_initialized = false;
    
    qDebug() << "[GameEngine] 游戏引擎已关闭";
}

void GameEngine::update(float deltaTime)
{
    if (m_paused) {
        return;
    }

    // 更新游戏时间
    m_gameTime += deltaTime;

    // --------------------------------------------------------
    // 更新各个子系统
    // --------------------------------------------------------
    
    // 更新输入状态
    if (m_inputManager) {
        m_inputManager->update(deltaTime);
    }

    // 更新场景
    if (m_sceneManager) {
        m_sceneManager->update(deltaTime);
    }
}

void GameEngine::render()
{
    // 注意：Qt的QGraphicsView会自动渲染场景
    // 这里可以添加自定义的渲染逻辑，比如：
    // - 渲染调试信息
    // - 渲染FPS显示
    // - 后期处理等
    
    m_frameCount++;
}

bool GameEngine::initializeSubsystems()
{
    qDebug() << "[GameEngine] 初始化子系统...";

    // --------------------------------------------------------
    // 1. 资源管理器（最先初始化，其他系统可能需要资源）
    // --------------------------------------------------------
    m_resourceManager = new ResourceManager();
    if (!m_resourceManager->initialize()) {
        qCritical() << "[GameEngine] 资源管理器初始化失败";
        return false;
    }
    qDebug() << "[GameEngine] 资源管理器初始化完成";

    // --------------------------------------------------------
    // 2. 输入管理器
    // --------------------------------------------------------
    m_inputManager = new InputManager();
    if (!m_inputManager->initialize()) {
        qCritical() << "[GameEngine] 输入管理器初始化失败";
        return false;
    }
    qDebug() << "[GameEngine] 输入管理器初始化完成";

    // --------------------------------------------------------
    // 3. 音频管理器
    // --------------------------------------------------------
    m_audioManager = new AudioManager();
    if (!m_audioManager->initialize()) {
        qCritical() << "[GameEngine] 音频管理器初始化失败";
        return false;
    }
    qDebug() << "[GameEngine] 音频管理器初始化完成";

    // --------------------------------------------------------
    // 4. 场景管理器
    // --------------------------------------------------------
    m_sceneManager = new SceneManager();
    if (!m_sceneManager->initialize()) {
        qCritical() << "[GameEngine] 场景管理器初始化失败";
        return false;
    }
    qDebug() << "[GameEngine] 场景管理器初始化完成";

    // --------------------------------------------------------
    // 5. 插件管理器（最后初始化，可以加载插件扩展功能）
    // --------------------------------------------------------
    m_pluginManager = new PluginManager();
    if (!m_pluginManager->initialize()) {
        qCritical() << "[GameEngine] 插件管理器初始化失败";
        return false;
    }
    qDebug() << "[GameEngine] 插件管理器初始化完成";

    qDebug() << "[GameEngine] 所有子系统初始化完成";
    return true;
}

bool GameEngine::initializeMainWindow()
{
    qDebug() << "[GameEngine] 初始化主窗口...";

    // 创建主窗口
    m_mainWindow = new QMainWindow();
    m_mainWindow->setWindowTitle(m_config.gameTitle);
    m_mainWindow->resize(m_config.windowSize);
    m_mainWindow->setMinimumSize(QSize(800, 600));

    // --------------------------------------------------------
    // 创建中央视图（QGraphicsView）
    // --------------------------------------------------------
    QGraphicsView* view = new QGraphicsView(m_mainWindow);
    
    // 设置渲染提示
    view->setRenderHint(QPainter::Antialiasing, true);
    view->setRenderHint(QPainter::SmoothPixmapTransform, true);
    
    // 设置视口更新模式（智能更新，提高性能）
    view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    
    // 设置拖拽模式
    view->setDragMode(QGraphicsView::NoDrag);
    
    // 隐藏滚动条（箱庭游戏通常不需要）
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    m_mainWindow->setCentralWidget(view);

    qDebug() << "[GameEngine] 主窗口初始化完成";
    return true;
}

void GameEngine::calculateFPS()
{
    m_fpsTimer += m_deltaTime;
    m_frameCount++;

    // 每秒更新一次FPS
    if (m_fpsTimer >= 1.0f) {
        m_currentFPS = m_frameCount / m_fpsTimer;
        m_frameCount = 0;
        m_fpsTimer = 0.0f;

        // 调试模式下输出FPS
        if (m_config.debugMode && m_config.showFPS) {
            qDebug() << "[GameEngine] FPS:" << QString::number(m_currentFPS, 'f', 1);
        }
    }
}
```

### 3.3 场景管理器 (SceneManager)

负责游戏场景的创建、切换和管理。

#### 头文件

**文件路径**：`include/core/SceneManager.h`

```cpp
/**
 * @file SceneManager.h
 * @brief 场景管理器类
 * 
 * 负责游戏场景的管理、切换和渲染。
 * 基于 Qt 的 QGraphicsScene 实现。
 */

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <QObject>
#include <QGraphicsScene>
#include <QMap>
#include <QString>

/**
 * @brief 场景管理器类
 * 
 * 负责游戏场景的管理、切换和渲染
 */
class SceneManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    explicit SceneManager(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~SceneManager() override;

    // ========================================================
    // 生命周期
    // ========================================================
    
    /// 初始化场景管理器
    bool initialize();
    
    /// 关闭场景管理器
    void shutdown();

    // ========================================================
    // 场景管理
    // ========================================================
    
    /**
     * @brief 创建新场景
     * @param sceneName 场景名称（唯一标识）
     * @return 场景指针
     */
    QGraphicsScene* createScene(const QString& sceneName);
    
    /**
     * @brief 获取场景
     * @param sceneName 场景名称
     * @return 场景指针，不存在返回nullptr
     */
    QGraphicsScene* getScene(const QString& sceneName) const;
    
    /**
     * @brief 切换到指定场景
     * @param sceneName 场景名称
     * @return 是否切换成功
     */
    bool switchToScene(const QString& sceneName);
    
    /// 获取当前场景
    QGraphicsScene* getCurrentScene() const { return m_currentScene; }
    
    /// 获取当前场景名称
    QString getCurrentSceneName() const { return m_currentSceneName; }

    // ========================================================
    // 更新
    // ========================================================
    
    /**
     * @brief 更新场景
     * @param deltaTime 时间增量（秒）
     */
    void update(float deltaTime);

signals:
    /**
     * @brief 场景切换信号
     * @param sceneName 新场景名称
     */
    void sceneChanged(const QString& sceneName);

private:
    /// 场景映射表（名称 -> 场景指针）
    QMap<QString, QGraphicsScene*> m_scenes;
    
    /// 当前场景
    QGraphicsScene* m_currentScene = nullptr;
    
    /// 当前场景名称
    QString m_currentSceneName;
    
    /// 是否已初始化
    bool m_initialized = false;
};

#endif // SCENE_MANAGER_H
```

#### 实现文件

**文件路径**：`src/core/SceneManager.cpp`

```cpp
/**
 * @file SceneManager.cpp
 * @brief 场景管理器类实现
 */

#include "core/SceneManager.h"

#include <QDebug>

SceneManager::SceneManager(QObject* parent)
    : QObject(parent)
{
}

SceneManager::~SceneManager()
{
    shutdown();
}

bool SceneManager::initialize()
{
    if (m_initialized) {
        qWarning() << "[SceneManager] 已经初始化";
        return true;
    }

    qDebug() << "[SceneManager] 初始化场景管理器...";

    // 创建默认场景
    createScene("default");
    switchToScene("default");

    m_initialized = true;
    qDebug() << "[SceneManager] 初始化完成";
    return true;
}

void SceneManager::shutdown()
{
    if (!m_initialized) {
        return;
    }

    qDebug() << "[SceneManager] 关闭场景管理器...";

    // 清理所有场景
    qDeleteAll(m_scenes);
    m_scenes.clear();
    m_currentScene = nullptr;
    m_currentSceneName.clear();

    m_initialized = false;
    qDebug() << "[SceneManager] 已关闭";
}

QGraphicsScene* SceneManager::createScene(const QString& sceneName)
{
    // 检查场景是否已存在
    if (m_scenes.contains(sceneName)) {
        qWarning() << "[SceneManager] 场景已存在:" << sceneName;
        return m_scenes[sceneName];
    }

    // 创建新场景
    QGraphicsScene* scene = new QGraphicsScene(this);
    
    // 设置默认场景大小（可以根据需要调整）
    scene->setSceneRect(0, 0, 1920, 1080);
    
    // 设置背景色（深灰色，科幻风格）
    scene->setBackgroundBrush(QBrush(QColor(20, 20, 30)));

    m_scenes[sceneName] = scene;

    qDebug() << "[SceneManager] 创建场景:" << sceneName;
    return scene;
}

QGraphicsScene* SceneManager::getScene(const QString& sceneName) const
{
    return m_scenes.value(sceneName, nullptr);
}

bool SceneManager::switchToScene(const QString& sceneName)
{
    // 检查场景是否存在
    if (!m_scenes.contains(sceneName)) {
        qWarning() << "[SceneManager] 场景不存在:" << sceneName;
        return false;
    }

    m_currentScene = m_scenes[sceneName];
    m_currentSceneName = sceneName;

    emit sceneChanged(sceneName);
    
    qDebug() << "[SceneManager] 切换到场景:" << sceneName;
    return true;
}

void SceneManager::update(float deltaTime)
{
    Q_UNUSED(deltaTime);
    
    // 场景更新逻辑
    // 可以在这里更新场景中的所有可更新对象
    // 例如：遍历所有QGraphicsItem，调用update方法
    
    // 注意：QGraphicsScene本身不提供update(deltaTime)机制
    // 如果需要帧更新，可以自定义一个基类
}
```

### 3.4 资源管理器 (ResourceManager)

负责游戏资源的加载、缓存和管理，支持图片、音频、字体等。

#### 头文件

**文件路径**：`include/core/ResourceManager.h`

```cpp
/**
 * @file ResourceManager.h
 * @brief 资源管理器类
 * 
 * 负责游戏资源的加载、缓存和管理。
 * 支持图片、音效、字体等资源的加载和缓存复用。
 */

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <QObject>
#include <QPixmap>
#include <QSoundEffect>
#include <QFont>
#include <QMap>
#include <QString>

/**
 * @brief 资源管理器类
 * 
 * 负责游戏资源的加载、缓存和管理
 */
class ResourceManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    explicit ResourceManager(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~ResourceManager() override;

    // ========================================================
    // 生命周期
    // ========================================================
    
    /// 初始化资源管理器
    bool initialize();
    
    /// 关闭资源管理器
    void shutdown();

    // ========================================================
    // 资源加载
    // ========================================================
    
    /**
     * @brief 加载图片资源
     * 
     * 如果资源已缓存，直接返回缓存的图片；
     * 否则从文件加载并缓存。
     * 
     * @param path 资源相对路径
     * @return 图片
     */
    QPixmap loadPixmap(const QString& path);
    
    /**
     * @brief 加载音效资源
     * @param path 资源相对路径
     * @return 音效指针，加载失败返回nullptr
     */
    QSoundEffect* loadSound(const QString& path);
    
    /**
     * @brief 加载字体资源
     * @param path 资源相对路径
     * @param pointSize 字号（磅）
     * @return 字体
     */
    QFont loadFont(const QString& path, int pointSize = 12);

    // ========================================================
    // 资源管理
    // ========================================================
    
    /// 卸载未使用的资源（垃圾回收）
    void collectGarbage();
    
    /// 清空所有缓存
    void clearCache();

    // ========================================================
    // 路径配置
    // ========================================================
    
    /// 获取资源基础路径
    QString getResourcePath() const { return m_resourcePath; }
    
    /// 设置资源基础路径
    void setResourcePath(const QString& path) { m_resourcePath = path; }

private:
    /**
     * @brief 获取完整资源路径
     * @param relativePath 相对路径
     * @return 完整路径
     */
    QString getFullPath(const QString& relativePath) const;

    // ========================================================
    // 资源缓存
    // ========================================================
    
    /// 图片缓存（完整路径 -> 图片）
    QMap<QString, QPixmap> m_pixmapCache;
    
    /// 音效缓存（完整路径 -> 音效）
    QMap<QString, QSoundEffect*> m_soundCache;
    
    /// 字体缓存（缓存键 -> 字体）
    QMap<QString, QFont> m_fontCache;

    // ========================================================
    // 配置
    // ========================================================
    
    /// 资源基础路径
    QString m_resourcePath = "assets";
    
    /// 是否已初始化
    bool m_initialized = false;
};

#endif // RESOURCE_MANAGER_H
```

#### 实现文件

**文件路径**：`src/core/ResourceManager.cpp`

```cpp
/**
 * @file ResourceManager.cpp
 * @brief 资源管理器类实现
 */

#include "core/ResourceManager.h"

#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QSoundEffect>

ResourceManager::ResourceManager(QObject* parent)
    : QObject(parent)
{
}

ResourceManager::~ResourceManager()
{
    shutdown();
}

bool ResourceManager::initialize()
{
    if (m_initialized) {
        qWarning() << "[ResourceManager] 已经初始化";
        return true;
    }

    qDebug() << "[ResourceManager] 初始化资源管理器...";
    qDebug() << "[ResourceManager] 资源路径:" << m_resourcePath;

    // 检查资源目录是否存在
    QDir dir(m_resourcePath);
    if (!dir.exists()) {
        qWarning() << "[ResourceManager] 资源目录不存在:" << m_resourcePath;
        qWarning() << "[ResourceManager] 正在创建资源目录...";
        dir.mkpath(".");
    }

    m_initialized = true;
    qDebug() << "[ResourceManager] 初始化完成";
    return true;
}

void ResourceManager::shutdown()
{
    if (!m_initialized) {
        return;
    }

    qDebug() << "[ResourceManager] 关闭资源管理器...";

    // 清理音效缓存（需要手动delete）
    qDeleteAll(m_soundCache);
    m_soundCache.clear();

    // 清理图片缓存（QPixmap会自动释放）
    m_pixmapCache.clear();

    // 清理字体缓存（QFont会自动释放）
    m_fontCache.clear();

    m_initialized = false;
    qDebug() << "[ResourceManager] 已关闭";
}

QPixmap ResourceManager::loadPixmap(const QString& path)
{
    QString fullPath = getFullPath(path);

    // 检查缓存
    if (m_pixmapCache.contains(fullPath)) {
        return m_pixmapCache[fullPath];
    }

    // 加载图片
    QPixmap pixmap;
    if (pixmap.load(fullPath)) {
        m_pixmapCache[fullPath] = pixmap;
        qDebug() << "[ResourceManager] 加载图片:" << path;
    } else {
        qWarning() << "[ResourceManager] 加载图片失败:" << fullPath;
        
        // 创建一个占位图（灰色方块）
        pixmap = QPixmap(100, 100);
        pixmap.fill(Qt::gray);
    }

    return pixmap;
}

QSoundEffect* ResourceManager::loadSound(const QString& path)
{
    QString fullPath = getFullPath(path);

    // 检查缓存
    if (m_soundCache.contains(fullPath)) {
        return m_soundCache[fullPath];
    }

    // 加载音效
    QSoundEffect* sound = new QSoundEffect(this);
    sound->setSource(QUrl::fromLocalFile(fullPath));

    // 检查是否加载成功
    if (sound->status() == QSoundEffect::Error) {
        qWarning() << "[ResourceManager] 加载音效失败:" << fullPath;
        delete sound;
        return nullptr;
    }

    m_soundCache[fullPath] = sound;
    qDebug() << "[ResourceManager] 加载音效:" << path;
    return sound;
}

QFont ResourceManager::loadFont(const QString& path, int pointSize)
{
    // 字体缓存键 = 路径_字号
    QString cacheKey = QString("%1_%2").arg(path).arg(pointSize);

    // 检查缓存
    if (m_fontCache.contains(cacheKey)) {
        return m_fontCache[cacheKey];
    }

    QString fullPath = getFullPath(path);

    // 加载字体
    QFont font;
    int fontId = QFontDatabase::addApplicationFont(fullPath);
    
    if (fontId != -1) {
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
        font = QFont(fontFamily, pointSize);
        qDebug() << "[ResourceManager] 加载字体:" << path 
                 << "字体族:" << fontFamily;
    } else {
        qWarning() << "[ResourceManager] 加载字体失败:" << fullPath;
        // 回退到默认字体
        font = QFont("Arial", pointSize);
    }

    m_fontCache[cacheKey] = font;
    return font;
}

void ResourceManager::collectGarbage()
{
    // 这里可以实现更复杂的垃圾回收逻辑
    // 比如：
    // - 卸载长时间未使用的资源
    // - 根据内存使用情况动态调整缓存大小
    // - 按优先级保留常用资源
    
    qDebug() << "[ResourceManager] 执行资源垃圾回收...";
    
    // 简单实现：目前不做任何清理
    // 后续可以根据需要添加引用计数、LRU缓存等机制
}

void ResourceManager::clearCache()
{
    qDebug() << "[ResourceManager] 清空所有资源缓存...";

    m_pixmapCache.clear();
    
    qDeleteAll(m_soundCache);
    m_soundCache.clear();
    
    m_fontCache.clear();
}

QString ResourceManager::getFullPath(const QString& relativePath) const
{
    return QString("%1/%2").arg(m_resourcePath, relativePath);
}
```

### 3.5 输入管理器 (InputManager)

负责键盘、鼠标等输入的管理，支持按键状态查询和事件通知。

#### 头文件

**文件路径**：`include/core/InputManager.h`

```cpp
/**
 * @file InputManager.h
 * @brief 输入管理器类
 * 
 * 负责键盘、鼠标、手柄等输入的管理。
 * 支持按键状态查询（按下/刚按下/刚释放）和事件通知。
 */

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <QObject>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMap>
#include <QPoint>
#include <Qt>

/**
 * @brief 输入管理器类
 * 
 * 负责键盘、鼠标、手柄等输入的管理
 */
class InputManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 按键状态枚举
     */
    enum KeyState {
        KeyReleased = 0,    ///< 释放状态
        KeyPressed = 1,     ///< 按下状态
        KeyJustPressed = 2, ///< 刚按下（只持续一帧）
        KeyJustReleased = 3 ///< 刚释放（只持续一帧）
    };

    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    explicit InputManager(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     */
    ~InputManager() override;

    // ========================================================
    // 生命周期
    // ========================================================
    
    /// 初始化输入管理器
    bool initialize();
    
    /// 关闭输入管理器
    void shutdown();

    // ========================================================
    // 帧更新
    // ========================================================
    
    /**
     * @brief 更新输入状态
     * 
     * 每帧调用一次，用于更新按键状态（JustPressed -> Pressed 等）
     * 
     * @param deltaTime 时间增量
     */
    void update(float deltaTime);

    // ========================================================
    // 键盘输入查询
    // ========================================================
    
    /**
     * @brief 检查按键是否按下
     * @param key 按键码（Qt::Key）
     * @return 是否按下
     */
    bool isKeyPressed(int key) const;
    
    /**
     * @brief 检查按键是否刚按下（只在按下的第一帧返回true）
     * @param key 按键码（Qt::Key）
     * @return 是否刚按下
     */
    bool isKeyJustPressed(int key) const;
    
    /**
     * @brief 检查按键是否刚释放（只在释放的第一帧返回true）
     * @param key 按键码（Qt::Key）
     * @return 是否刚释放
     */
    bool isKeyJustReleased(int key) const;

    // ========================================================
    // 鼠标输入查询
    // ========================================================
    
    /// 获取鼠标位置（相对于窗口）
    QPoint getMousePosition() const { return m_mousePosition; }
    
    /// 获取鼠标移动增量（这一帧移动了多少）
    QPoint getMouseDelta() const { return m_mouseDelta; }
    
    /**
     * @brief 检查鼠标按钮是否按下
     * @param button 鼠标按钮
     * @return 是否按下
     */
    bool isMouseButtonPressed(Qt::MouseButton button) const;

    // ========================================================
    // 事件处理（由外部调用）
    // ========================================================
    
    /// 处理按键按下事件
    void handleKeyPress(QKeyEvent* event);
    
    /// 处理按键释放事件
    void handleKeyRelease(QKeyEvent* event);
    
    /// 处理鼠标移动事件
    void handleMouseMove(QMouseEvent* event);
    
    /// 处理鼠标按下事件
    void handleMousePress(QMouseEvent* event);
    
    /// 处理鼠标释放事件
    void handleMouseRelease(QMouseEvent* event);

signals:
    /// 按键按下信号
    void keyPressed(int key);
    
    /// 按键释放信号
    void keyReleased(int key);
    
    /// 鼠标点击信号
    void mouseClicked(Qt::MouseButton button, const QPoint& position);

private:
    // ========================================================
    // 键盘状态
    // ========================================================
    
    /// 按键状态映射
    QMap<int, KeyState> m_keyStates;

    // ========================================================
    // 鼠标状态
    // ========================================================
    
    /// 当前鼠标位置
    QPoint m_mousePosition;
    
    /// 鼠标移动增量
    QPoint m_mouseDelta;
    
    /// 上一帧鼠标位置
    QPoint m_lastMousePosition;
    
    /// 鼠标按钮状态
    QMap<Qt::MouseButton, bool> m_mouseButtons;

    // ========================================================
    // 状态
    // ========================================================
    
    /// 是否已初始化
    bool m_initialized = false;
};

#endif // INPUT_MANAGER_H
```

#### 实现文件

**文件路径**：`src/core/InputManager.cpp`

```cpp
/**
 * @file InputManager.cpp
 * @brief 输入管理器类实现
 */

#include "core/InputManager.h"

#include <QDebug>

InputManager::InputManager(QObject* parent)
    : QObject(parent)
{
}

InputManager::~InputManager()
{
    shutdown();
}

bool InputManager::initialize()
{
    if (m_initialized) {
        qWarning() << "[InputManager] 已经初始化";
        return true;
    }

    qDebug() << "[InputManager] 初始化输入管理器...";

    // 初始化鼠标状态
    m_mousePosition = QPoint(0, 0);
    m_mouseDelta = QPoint(0, 0);
    m_lastMousePosition = QPoint(0, 0);

    // 初始化常用鼠标按钮
    m_mouseButtons[Qt::LeftButton] = false;
    m_mouseButtons[Qt::RightButton] = false;
    m_mouseButtons[Qt::MiddleButton] = false;

    m_initialized = true;
    qDebug() << "[InputManager] 初始化完成";
    return true;
}

void InputManager::shutdown()
{
    if (!m_initialized) {
        return;
    }

    qDebug() << "[InputManager] 关闭输入管理器...";

    m_keyStates.clear();
    m_mouseButtons.clear();

    m_initialized = false;
    qDebug() << "[InputManager] 已关闭";
}

void InputManager::update(float deltaTime)
{
    Q_UNUSED(deltaTime);

    // --------------------------------------------------------
    // 更新鼠标移动增量
    // --------------------------------------------------------
    m_mouseDelta = m_mousePosition - m_lastMousePosition;
    m_lastMousePosition = m_mousePosition;

    // --------------------------------------------------------
    // 更新按键状态
    // 
    // 将 JustPressed 转为 Pressed
    // 将 JustReleased 转为 Released
    // 
    // 这样 isKeyJustPressed 只在按下的第一帧返回true
    // isKeyJustReleased 只在释放的第一帧返回true
    // --------------------------------------------------------
    QMutableMapIterator<int, KeyState> it(m_keyStates);
    while (it.hasNext()) {
        it.next();
        
        if (it.value() == KeyJustPressed) {
            it.setValue(KeyPressed);
        } else if (it.value() == KeyJustReleased) {
            it.setValue(KeyReleased);
        }
    }
}

bool InputManager::isKeyPressed(int key) const
{
    KeyState state = m_keyStates.value(key, KeyReleased);
    return (state == KeyPressed || state == KeyJustPressed);
}

bool InputManager::isKeyJustPressed(int key) const
{
    return m_keyStates.value(key, KeyReleased) == KeyJustPressed;
}

bool InputManager::isKeyJustReleased(int key) const
{
    return m_keyStates.value(key, KeyReleased) == KeyJustReleased;
}

bool InputManager::isMouseButtonPressed(Qt::MouseButton button) const
{
    return m_mouseButtons.value(button, false);
}

void InputManager::handleKeyPress(QKeyEvent* event)
{
    int key = event->key();

    // 如果之前是释放状态，设置为刚按下
    if (m_keyStates.value(key, KeyReleased) == KeyReleased) {
        m_keyStates[key] = KeyJustPressed;
        emit keyPressed(key);
    }
    
    // 否则保持按下状态（避免重复触发 JustPressed）
    // 注意：如果按住不放，Qt会重复发送keyPress事件
    // 我们只在第一次按下时设置为 JustPressed
}

void InputManager::handleKeyRelease(QKeyEvent* event)
{
    int key = event->key();
    m_keyStates[key] = KeyJustReleased;
    emit keyReleased(key);
}

void InputManager::handleMouseMove(QMouseEvent* event)
{
    m_mousePosition = event->pos();
}

void InputManager::handleMousePress(QMouseEvent* event)
{
    Qt::MouseButton button = event->button();
    m_mouseButtons[button] = true;
    emit mouseClicked(button, event->pos());
}

void InputManager::handleMouseRelease(QMouseEvent* event)
{
    Qt::MouseButton button = event->button();
    m_mouseButtons[button] = false;
}
```

### 3.6 主程序入口 (main.cpp)

程序的入口点，负责初始化 QApplication 和游戏引擎。

**文件路径**：`src/main.cpp`

```cpp
/**
 * @file main.cpp
 * @brief 星溯旅人 - 主程序入口
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * 
 * 程序入口点，负责：
 * - 创建 QApplication
 * - 解析命令行参数
 * - 创建并初始化游戏引擎
 * - 运行游戏主循环
 */

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>

#include "core/GameEngine.h"
#include "core/GameConfig.h"

int main(int argc, char *argv[])
{
    // ========================================================
    // 1. 创建 QApplication
    // ========================================================
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    QApplication::setApplicationName("StarRetrospectTraveler");
    QApplication::setApplicationVersion("0.1.0");
    QApplication::setOrganizationName("StarRetrospectStudio");

    // ========================================================
    // 2. 解析命令行参数
    // ========================================================
    QCommandLineParser parser;
    parser.setApplicationDescription("星溯旅人 - 2.5D箱庭探索策略RPG");
    parser.addHelpOption();
    parser.addVersionOption();

    // 调试模式选项
    QCommandLineOption debugOption(QStringList() << "d" << "debug",
        QCoreApplication::translate("main", "启用调试模式"));
    parser.addOption(debugOption);

    // 全屏模式选项
    QCommandLineOption fullscreenOption(QStringList() << "f" << "fullscreen",
        QCoreApplication::translate("main", "全屏模式"));
    parser.addOption(fullscreenOption);

    // 解析命令行
    parser.process(app);

    // ========================================================
    // 3. 游戏配置
    // ========================================================
    GameConfig config;
    config.debugMode = parser.isSet(debugOption);
    config.fullscreen = parser.isSet(fullscreenOption);

    // 输出启动信息
    qDebug() << "";
    qDebug() << "========================================";
    qDebug() << "  星溯旅人 (Star Retrospect Traveler)";
    qDebug() << "  版本: 0.1.0";
    qDebug() << "========================================";
    qDebug() << "";

    if (config.debugMode) {
        qDebug() << "[启动] 调试模式已启用";
    }

    // ========================================================
    // 4. 创建并初始化游戏引擎
    // ========================================================
    GameEngine engine(config);

    if (!engine.initialize()) {
        qCritical() << "[错误] 游戏初始化失败！";
        return -1;
    }

    qDebug() << "[启动] 游戏初始化完成";
    qDebug() << "";

    // ========================================================
    // 5. 运行游戏主循环
    // ========================================================
    int result = engine.run();

    qDebug() << "";
    qDebug() << "[退出] 游戏退出，返回码:" << result;
    qDebug() << "";

    return result;
}
```

---

## 步骤4：配置文件与资源

### 4.1 游戏主配置文件

**文件路径**：`assets/config/game.json`

```json
{
    "windowWidth": 1280,
    "windowHeight": 720,
    "fullscreen": false,
    "vsync": true,
    "targetFPS": 60,
    "enableParticles": true,
    "textureQuality": 2,
    "masterVolume": 1.0,
    "musicVolume": 0.8,
    "sfxVolume": 1.0,
    "muteAudio": false,
    "debugMode": false,
    "showFPS": false,
    "showCollision": false,
    "language": "zh_CN",
    "mouseSensitivity": 1.0,
    "invertY": false
}
```

---

## 编译运行

### 方式一：Qt Creator（推荐）

1. 打开 Qt Creator
2. 文件 → 打开文件或项目 → 选择 `CMakeLists.txt`
3. 选择构建套件：**Desktop Qt 6.10.1 MinGW 64-bit**
4. 选择构建类型：Debug 或 Release
5. 点击「配置项目」
6. 等待 CMake 配置完成
7. 点击左下角的运行按钮（绿色三角形）

### 方式二：命令行

```bash
# 1. 创建构建目录
mkdir build
cd build

# 2. 配置项目（MinGW版本）
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/path/to/Qt/6.10.1/mingw_64

# 3. 编译
cmake --build . -j$(nproc)

# 4. 运行
./bin/StarRetrospectTraveler.exe
```

### 命令行参数

```bash
# 调试模式运行
./StarRetrospectTraveler --debug

# 全屏模式运行
./StarRetrospectTraveler --fullscreen

# 调试+全屏
./StarRetrospectTraveler -d -f
```

---

## ✅ 第一阶段完成检查清单

- [x] 环境搭建（Qt 6.10.1 + MinGW-w64）
- [x] 项目目录结构创建
- [x] CMake 配置（主配置 + 核心模块）
- [x] 游戏配置类 (GameConfig)
- [x] 游戏引擎类 (GameEngine) - 主循环
- [x] 场景管理器 (SceneManager)
- [x] 资源管理器 (ResourceManager)
- [x] 输入管理器 (InputManager)
- [x] 音频管理器 (AudioManager)
- [x] 插件管理器 (PluginManager)
- [x] 主程序入口 (main.cpp)
- [x] 配置文件 (game.json)
- [x] 项目可编译运行

---

## 📁 文件清单

第一阶段共 **17 个文件**：

| 类型 | 文件路径 | 说明 |
|------|---------|------|
| 构建 | `CMakeLists.txt` | 主 CMake 配置 |
| 构建 | `src/CMakeLists.txt` | 主程序 CMake 配置 |
| 构建 | `src/core/CMakeLists.txt` | 核心模块 CMake 配置 |
| 头文件 | `include/core/GameConfig.h` | 游戏配置类 |
| 头文件 | `include/core/GameEngine.h` | 游戏引擎类 |
| 头文件 | `include/core/SceneManager.h` | 场景管理器 |
| 头文件 | `include/core/ResourceManager.h` | 资源管理器 |
| 头文件 | `include/core/InputManager.h` | 输入管理器 |
| 头文件 | `include/core/AudioManager.h` | 音频管理器 |
| 头文件 | `include/core/PluginManager.h` | 插件管理器 |
| 源码 | `src/main.cpp` | 主程序入口 |
| 源码 | `src/core/GameConfig.cpp` | 游戏配置实现 |
| 源码 | `src/core/GameEngine.cpp` | 游戏引擎实现 |
| 源码 | `src/core/SceneManager.cpp` | 场景管理实现 |
| 源码 | `src/core/ResourceManager.cpp` | 资源管理实现 |
| 源码 | `src/core/InputManager.cpp` | 输入管理实现 |
| 配置 | `assets/config/game.json` | 游戏主配置 |

---

## 🎮 运行效果

运行后会看到一个空白的游戏窗口，标题为「星溯旅人」。
这是正常的，因为第一阶段只是搭建了基础框架，还没有添加游戏内容。

后续阶段会逐步添加：
- 玩家角色
- 地图和场景
- 时间褶皱系统
- 星核共鸣系统
- 生态共生系统
- 等等...

---

**下一个阶段**：第二阶段 - 核心引擎开发（第2-4个月）
