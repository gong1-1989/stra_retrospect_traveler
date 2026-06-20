# ============================================================
# 星溯旅人 - 插件构建配置
# ============================================================
# 
# 这个文件用于构建所有示例插件。
# 插件会被编译为动态库，在运行时由插件管理器加载。
# 
# 使用方法：
#   mkdir build
#   cd build
#   cmake ..
#   cmake --build .
# 
# ============================================================

cmake_minimum_required(VERSION 3.16)
project(StarRetrospectPlugins
    VERSION 0.1.0
    DESCRIPTION "星溯旅人 - 示例插件集合"
    LANGUAGES CXX
)

# C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt配置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.10 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
)

# 游戏源码目录（用于包含头文件）
set(GAME_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

# 输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)

# ============================================================
# 示例玩法插件
# ============================================================

add_library(ExampleGameplayPlugin SHARED
    ExampleGameplayPlugin.h
    ExampleGameplayPlugin.cpp
    example_gameplay_plugin.json
)

target_include_directories(ExampleGameplayPlugin PRIVATE
    ${GAME_SOURCE_DIR}
)

target_link_libraries(ExampleGameplayPlugin PRIVATE
    Qt6::Core
    Qt6::Widgets
)

set_target_properties(ExampleGameplayPlugin PROPERTIES
    PREFIX ""
    OUTPUT_NAME "ExampleGameplayPlugin"
)

# ============================================================
# 示例UI插件
# ============================================================

add_library(ExampleUIPlugin SHARED
    ExampleUIPlugin.h
    ExampleUIPlugin.cpp
    example_ui_plugin.json
)

target_include_directories(ExampleUIPlugin PRIVATE
    ${GAME_SOURCE_DIR}
)

target_link_libraries(ExampleUIPlugin PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

set_target_properties(ExampleUIPlugin PROPERTIES
    PREFIX ""
    OUTPUT_NAME "ExampleUIPlugin"
)

# ============================================================
# 示例内容插件
# ============================================================

add_library(ExampleContentPlugin SHARED
    ExampleContentPlugin.h
    ExampleContentPlugin.cpp
    example_content_plugin.json
)

target_include_directories(ExampleContentPlugin PRIVATE
    ${GAME_SOURCE_DIR}
)

target_link_libraries(ExampleContentPlugin PRIVATE
    Qt6::Core
    Qt6::Widgets
)

set_target_properties(ExampleContentPlugin PROPERTIES
    PREFIX ""
    OUTPUT_NAME "ExampleContentPlugin"
)

# ============================================================
# MinGW专用优化
# ============================================================

if(MINGW)
    foreach(plugin ExampleGameplayPlugin ExampleUIPlugin ExampleContentPlugin)
        target_compile_options(${plugin} PRIVATE -Wall -Wextra)
        target_compile_options(${plugin} PRIVATE -Wa,-mbig-obj)
        target_link_options(${plugin} PRIVATE -static-libgcc -static-libstdc++)
        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            target_compile_options(${plugin} PRIVATE -O3)
            target_link_options(${plugin} PRIVATE -s)
        endif()
    endforeach()
endif()

# ============================================================
# 安装规则
# ============================================================

install(TARGETS
    ExampleGameplayPlugin
    ExampleUIPlugin
    ExampleContentPlugin
    LIBRARY DESTINATION plugins
    RUNTIME DESTINATION plugins
)

# 安装JSON配置文件
install(FILES
    example_gameplay_plugin.json
    example_ui_plugin.json
    example_content_plugin.json
    DESTINATION plugins
)

message(STATUS "========================================")
message(STATUS "星溯旅人插件配置完成")
message(STATUS "插件列表:")
message(STATUS "  - ExampleGameplayPlugin (玩法插件)")
message(STATUS "  - ExampleUIPlugin (UI插件)")
message(STATUS "  - ExampleContentPlugin (内容插件)")
message(STATUS "输出目录: ${CMAKE_BINARY_DIR}/plugins")
message(STATUS "========================================")
