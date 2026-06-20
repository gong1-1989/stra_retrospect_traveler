# MinGW-w64 编译指南

## 环境要求

### 必需软件

| 软件 | 版本要求 | 说明 |
|------|----------|------|
| Qt | 6.10.1 或更高 | MinGW 64-bit 版本 |
| MinGW-w64 | GCC 11+ | 随 Qt 一起安装即可 |
| CMake | 3.20 或更高 | 构建系统 |
| Git | 任意 | 版本控制（可选） |

### 推荐安装方式

使用 Qt 官方安装包，选择 MinGW 64-bit 组件：
- Qt 6.10.1 → MinGW 64-bit
- Developer and Designer Tools → MinGW 11.2.0 64-bit
- Developer and Designer Tools → CMake
- Developer and Designer Tools → Ninja（可选，更快的构建速度）

---

## 编译步骤

### 方法一：使用 Qt Creator（推荐）

1. **打开 Qt Creator**
2. **打开项目**
   - 文件 → 打开文件或项目
   - 选择项目根目录下的 `CMakeLists.txt`
3. **配置项目**
   - 选择构建套件：Desktop Qt 6.10.1 MinGW 64-bit
   - 点击"配置项目"
4. **选择构建类型**
   - 左下角选择 Debug 或 Release
5. **构建项目**
   - 点击构建按钮（锤子图标）或按 Ctrl+B
6. **运行项目**
   - 点击运行按钮或按 Ctrl+R

### 方法二：使用命令行

#### 1. 设置环境变量

确保 MinGW 和 Qt 的 bin 目录在 PATH 中：

```batch
:: 示例路径，请根据实际安装位置调整
set PATH=C:\Qt\6.10.1\mingw_64\bin;%PATH%
set PATH=C:\Qt\Tools\mingw1120_64\bin;%PATH%
set PATH=C:\Qt\Tools\CMake_64\bin;%PATH%
```

或者使用 Qt 提供的命令行环境：
- 开始菜单 → Qt → Qt 6.10.1 (MinGW 64-bit)

#### 2. 创建构建目录

```bash
# 进入项目目录
cd star_retrospect_traveler

# 创建构建目录
mkdir build
cd build
```

#### 3. 配置 CMake

**Debug 模式：**
```bash
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
```

**Release 模式：**
```bash
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
```

**使用 Ninja（更快）：**
```bash
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release
```

#### 4. 编译

```bash
# 使用 MinGW Makefiles
mingw32-make -j%NUMBER_OF_PROCESSORS%

# 或者使用 Ninja
ninja
```

`-j%NUMBER_OF_PROCESSORS%` 表示使用所有 CPU 核心并行编译。

#### 5. 运行

```bash
# Debug 模式
cd bin
StarRetrospectTravelerd.exe --debug

# Release 模式
cd bin
StarRetrospectTraveler.exe
```

---

## MinGW 专用优化说明

项目已针对 MinGW-w64 做了以下优化：

### 1. 静态链接运行库

```cmake
add_link_options(-static-libgcc -static-libstdc++)
```

- **作用**：静态链接 libgcc 和 libstdc++
- **好处**：不需要附带 `libgcc_s_seh-1.dll`、`libstdc++-6.dll` 等运行库
- **影响**：可执行文件体积稍大，但部署更简单

### 2. 大目标文件支持

```cmake
add_compile_options(-Wa,-mbig-obj)
```

- **作用**：支持生成大于 2GB 的目标文件
- **原因**：Debug 模式下开启调试信息后，某些文件可能很大
- **避免错误**：`File too big` 或 `section .text$_ZN... can not be handled`

### 3. Release 模式优化

```cmake
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)  # 最高优化级别
    add_link_options(-s)       # 剥离符号信息
endif()
```

- **-O3**：最高级别的代码优化，提升运行性能
- **-s**：剥离调试符号，减小可执行文件体积

### 4. 控制台窗口控制

- **Debug 模式**：保留控制台窗口，便于查看 qDebug() 输出
- **Release 模式**：隐藏控制台窗口，纯 GUI 应用

### 5. AddressSanitizer 适配

MinGW 下 ASan 支持不完善，已自动禁用，避免编译错误。

---

## 常见问题

### Q1: 编译时提示 "File too big" 错误

**原因**：Debug 模式下某些文件包含大量调试信息，超过了默认限制。

**解决**：项目已添加 `-Wa,-mbig-obj` 选项，应该不会出现此问题。
如果仍有问题，尝试 Release 模式编译。

### Q2: 运行时提示缺少 DLL

**原因**：没有正确部署 Qt 运行库。

**解决方法一：使用 windeployqt**
```bash
cd build/bin
windeployqt StarRetrospectTraveler.exe
```

**解决方法二：静态链接**
项目已静态链接 libgcc 和 libstdc++，但 Qt 库本身仍是动态的。
如需完全静态，需要编译静态版 Qt（较复杂）。

### Q3: 编译速度慢

**优化建议**：
1. 使用 Ninja 替代 MinGW Makefiles（更快的增量构建）
2. 使用 `-j` 参数启用多线程编译
3. 使用 SSD 硬盘
4. 开启预编译头（后续版本会添加）

### Q4: Debug 模式下程序很慢

**原因**：Debug 模式没有优化，且包含大量调试信息。

**正常现象**：
- Debug 模式用于开发调试，性能较低是正常的
- Release 模式会启用 -O3 优化，性能会大幅提升

### Q5: 中文显示乱码

**解决**：
1. 确保源文件保存为 UTF-8 编码
2. 代码中使用 `QStringLiteral("中文")` 或 `u8"中文"`
3. 项目已添加 `/utf-8` 等效选项

---

## 部署与打包

### 快速部署

使用 `windeployqt` 工具自动收集依赖：

```bash
cd build/bin
windeployqt --release --no-compiler-runtime StarRetrospectTraveler.exe
```

常用参数：
- `--release`：Release 模式
- `--debug`：Debug 模式
- `--no-compiler-runtime`：不包含编译器运行库（已静态链接）
- `--no-system-d3d-compiler`：不包含 D3D 编译器
- `--no-opengl-sw`：不包含软件 OpenGL 渲染器

### 打包为 ZIP

将 bin 目录下所有文件打包即可。

---

## 性能对比

| 构建类型 | 优化级别 | 可执行文件大小 | 运行性能 | 适用场景 |
|----------|----------|----------------|----------|----------|
| Debug | -O0 | 大（含调试信息） | 慢 | 开发调试 |
| Release | -O3 | 小（剥离符号） | 快 | 发布运行 |

---

## 相关链接

- [Qt 官方文档 - MinGW](https://doc.qt.io/qt-6/windows-building.html)
- [MinGW-w64 官网](https://www.mingw-w64.org/)
- [CMake 官方文档](https://cmake.org/documentation/)
