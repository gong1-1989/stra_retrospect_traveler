
cmake_minimum_required(VERSION 3.16)

project(star_retrospect_traveler VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
        src/core/gameengine.h src/core/gameengine.cpp
        docs/No1.pdf docs/No2.pdf docs/No3.pdf docs/No4.pdf

        src/core/scenemanager.h src/core/scenemanager.cpp
        src/core/resourcemanager.h src/core/resourcemanager.cpp
        src/core/inputmanager.h src/core/inputmanager.cpp
        src/core/audiomanager.h src/core/audiomanager.cpp






        src/game/player.h src/game/player.cpp
        src/game/enemy.h src/game/enemy.cpp
        src/game/timesystem.h src/game/timesystem.cpp
        src/game/resonancesystem.h src/game/resonancesystem.cpp
        src/game/ecosystemsystem.h src/game/ecosystemsystem.cpp
        src/game/combatsystem.h src/game/combatsystem.cpp
        src/game/skillsystem.h src/game/skillsystem.cpp
        src/game/aisystem.h src/game/aisystem.cpp
        src/game/inventorysystem.h src/game/inventorysystem.cpp
        src/game/questsystem.h src/game/questsystem.cpp
        src/core/gameconfig.h src/core/gameconfig.cpp
        src/core/savesystem.h src/core/savesystem.cpp
        src/core/pluginmanager.h src/core/pluginmanager.cpp
        src/core/eventmanager.h src/core/eventmanager.cpp
        src/render/rendermanager.h src/render/rendermanager.cpp
        src/render/camerasystem.h src/render/camerasystem.cpp
        src/render/particlesystem.h src/render/particlesystem.cpp
        src/render/spriteanimation.h src/render/spriteanimation.cpp
        src/render/tilemap.h src/render/tilemap.cpp
        src/ui/uimanager.h src/ui/uimanager.cpp
        src/ui/mainmenu.h src/ui/mainmenu.cpp src/ui/mainmenu.ui
        src/ui/settingsdialog.h src/ui/settingsdialog.cpp src/ui/settingsdialog.ui
        src/ui/inventorypanel.h src/ui/inventorypanel.cpp src/ui/inventorypanel.ui
        src/ui/dialogbox.h src/ui/dialogbox.cpp src/ui/dialogbox.ui
        src/ui/talentpanel.h src/ui/talentpanel.cpp
        src/ui/starmappanel.h src/ui/starmappanel.cpp
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
add_subdirectory(modules/CoreModule)