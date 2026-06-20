# ============================================================
# 内容插件示例 - CMake 配置
# ============================================================
# 时空裂隙内容扩展包
# 展示如何通过插件添加游戏内容
# ============================================================

# 插件名称
set(PLUGIN_NAME ContentPlugin)

# 源文件
set(PLUGIN_SOURCES
    ContentPlugin.cpp
)

# 头文件
set(PLUGIN_HEADERS
    ContentPlugin.h
)

# 元数据文件
set(PLUGIN_METADATA
    content.json
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
)

# 设置输出名称
set_target_properties(${PLUGIN_NAME} PROPERTIES
    PREFIX ""
    OUTPUT_NAME "ContentPlugin"
)

# Windows 下的特殊设置
if(WIN32)
    set_target_properties(${PLUGIN_NAME} PROPERTIES
        PREFIX ""
    )
endif()

message(STATUS "  - 插件: ${PLUGIN_NAME} v1.2.0 (时空裂隙内容包)")
