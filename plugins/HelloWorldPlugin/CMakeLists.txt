# ============================================================
# HelloWorld 示例插件 - CMake 配置
# ============================================================
# 这是一个最简单的插件示例
# 展示如何构建一个星溯旅人插件
# ============================================================

# 插件名称
set(PLUGIN_NAME HelloWorldPlugin)

# 源文件
set(PLUGIN_SOURCES
    HelloWorldPlugin.cpp
)

# 头文件
set(PLUGIN_HEADERS
    HelloWorldPlugin.h
)

# 元数据文件
set(PLUGIN_METADATA
    helloworld.json
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

# 设置输出名称（去掉 lib 前缀）
set_target_properties(${PLUGIN_NAME} PROPERTIES
    PREFIX ""
    OUTPUT_NAME "HelloWorldPlugin"
)

# Windows 下的特殊设置
if(WIN32)
    # Windows 下 DLL 不需要 lib 前缀
    set_target_properties(${PLUGIN_NAME} PROPERTIES
        PREFIX ""
    )
endif()

message(STATUS "  - 插件: ${PLUGIN_NAME} v1.0.0")
