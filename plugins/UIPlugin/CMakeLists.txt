# ============================================================
# UI 插件示例 - CMake 配置
# ============================================================
# 星空主题 HUD
# 展示如何通过插件自定义游戏界面
# ============================================================

# 插件名称
set(PLUGIN_NAME UIPlugin)

# 源文件
set(PLUGIN_SOURCES
    UIPlugin.cpp
)

# 头文件
set(PLUGIN_HEADERS
    UIPlugin.h
)

# 元数据文件
set(PLUGIN_METADATA
    ui.json
)

# 创建动态库
add_library(${PLUGIN_NAME} SHARED
    ${PLUGIN_SOURCES}
    ${PLUGIN_HEADERS}
    ${PLUGIN_METADATA}
)

# 链接Qt库
target_link_libraries(${PLUGIN_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
)

# 设置输出名称
set_target_properties(${PLUGIN_NAME} PROPERTIES
    PREFIX ""
    OUTPUT_NAME "UIPlugin"
)

# Windows 下的特殊设置
if(WIN32)
    set_target_properties(${PLUGIN_NAME} PROPERTIES
        PREFIX ""
    )
endif()

message(STATUS "  - 插件: ${PLUGIN_NAME} v2.0.0 (星空主题 UI)")
