#==================================
# 星塑旅人 - 主Cmake配置文件
# 项目名称：星塑旅人 Star Retrospect Traveler
# 版本：0.1.0
# 作者：云中的鱼
#==================================
#-----------------------------------
# Cmake最低版本要求
# 使用3.20版本是因为需要一些现代Cmake特性
# 比如target_link_libraries的 PUBLIC/PRIVATE/INTERFACE
#-----------------------------------
cmake_minimum_required(VERSION 3.20)

#-----------------------------------
# 项目定义
# prihect()命令用于定义项目的基本信息
# 参数说明
#  -项目名称：star_retrospect_traveler
#  -VERSION：醒项目版本号（主版本.次版本.补丁版本）
#  -LANGUAGES：使用的编程语言（CXX=C++）
#-----------------------------------
project(star_retrospect_traveler VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#===================================
# C++标准配置
#===================================
#-----------------------------------
# 设置C++标准
# CMAKE_CXX_STANDARD：指定C++标准版本
# CMAKE_CXX_STANDARD_REQUIRED：强制要求编译器支持该标准
# CMAKE_CXX_EXTENSIONS：禁用编译器扩展，保证代码可移植性
#-----------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /permissive-)
elseif(MINGW)
    add_compile_options(-Wall -Wextra -Wpedantic)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--large-address-aware")
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
    Widgets
    LinguistTools   
)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Widgets
    LinguistTools
    Core
    Gui
    Multimedia
    Network
    Quick
    Qml
    Svg
    Test
)

set(TS_FILES star_retrospect_traveler_zh_CN.ts)

set(PROJECT_SOURCES
        main.cpp
        gamewindow.h
        gamewindow.cpp
        gamewindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(star_retrospect_traveler
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        docs/config_resource_doc.html
        docs/config_resource_doc.xml
        docs/dev_guide_doc.html
        docs/dev_guide_doc.xml
        docs/game_design_doc.html
        docs/game_design_doc.xml
        docs/project_ops_doc.html
        docs/project_ops_doc.xml
        docs/tech_arch_doc.html
        docs/tech_arch_doc.xml
        docs/README.md
        docs/star_retrospect_traveler.pdf
        resources.qrc
        gameloop.h gameloop.cpp
        basescene.h basescene.cpp
        scenemanager.h scenemanager.cpp
        resourcemanager.h resourcemanager.cpp
        inputmanager.h inputmanager.cpp
        src/gameengine.h src/gameengine.cpp
        docs/No1.pdf docs/No2.pdf docs/No3.pdf docs/No4.pdf
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET star_retrospect_traveler APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(star_retrospect_traveler SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(star_retrospect_traveler
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(star_retrospect_traveler PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Svg
)

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/game
    ${CMAKE_SOURCE_DIR}/src/render
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_BINARY_DIR}
)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.star_retrospect_traveler)
endif()
set_target_properties(star_retrospect_traveler PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS star_retrospect_traveler
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(star_retrospect_traveler)
endif()
