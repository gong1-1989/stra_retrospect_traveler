/**
 * @file main.cpp
 * @brief 星溯旅人 - 主程序入口
 *
* 这是游戏的主入口文件，负责：
 * - 初始化 Qt 应用程序
 * - 解析命令行参数
 * - 加载游戏配置
 * - 创建并启动游戏引擎
 * - 进入事件循环
 *
 */
#include <QApplication>
#include <QDebug>
#include <QLocale>
#include <QTranslator>
#include "GameWindow.h"

/**
 * @brief 主函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 退出码
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "star_retrospect_traveler_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    a.setApplicationName("星溯旅人");
    a.setApplicationVersion("0.1.0");
    a.setOrganizationName("Star Retrospect Traveler");
    qDebug()<<"=====================================\n"
                "星溯旅人 —— Star Retrospect Traveler\n"
                "版本：0.1.0\n"
                "第一阶段：项目初始化\n"
                "=====================================\n"
                "\n"
                "游戏启动中...\n"
                "=====================================";
    GameWindow w;
    w.show();
    qDebug()<<"游戏启动成功\n"
                "\n"
                "已加载系统：\n"
                " -游戏主循环(GameLoop)\n"
                " -场景管理器(SceneManager)\n"
                " -资源管理器(ResourceManager)\n"
                " -输入管理器(InputManager)\n";
    return a.exec();
}
