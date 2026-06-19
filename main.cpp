/**
 * @file main.cpp
 * @brief 星溯旅人 - 主程序入口
 *
 * 游戏启动入口，负责初始化游戏引擎并进入主循环
 */
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QCommandLineParser>
#include <QDebug>
#include "GameEngine.h"
#include "GameConfig.h"

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
    QApplication::setApplicationName("星溯旅人");
    QApplication::setApplicationVersion("0.1.0");

    return a.exec();
}
