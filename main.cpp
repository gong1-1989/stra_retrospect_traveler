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
#include <QCommandLineParser>
#include <QDebug>
#include <QLocale>
#include <QFontDatabase>
#include <QTranslator>
#include "GameConfig.h"
#include "GameEngine.h"
#include "GameConstants.h"

/**
 * @brief 主函数
 * @param argc 命令行参数数量
 * @param argv 命令行参数数组
 * @return 退出码
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 设置应用程序信息（用于 QSettings 等）
    QApplication::setApplicationName(GameConstants::GAME_NAME_EN);
    QApplication::setApplicationVersion(GameConstants::GAME_VERSION);
    QApplication::setOrganizationName("StarRetrospectStudio");
    QApplication::setOrganizationDomain("starretrospect.com");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "star_retrospect_traveler_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    // ============================================================
    // 2. 解析命令行参数
    // ============================================================

    QCommandLineParser parser;
    parser.setApplicationDescription(QObject::tr("星溯旅人 - 探索策略RPG"));
    parser.addHelpOption();      // 添加 --help 选项
    parser.addVersionOption();   // 添加 --version 选项

    // 调试模式选项
    QCommandLineOption debugOption(
        QStringList() << "d" << "debug",
        QObject::tr("启用调试模式，显示调试信息和性能统计")
        );
    parser.addOption(debugOption);

    // 全屏模式选项
    QCommandLineOption fullscreenOption(
        QStringList() << "f" << "fullscreen",
        QObject::tr("以全屏模式启动游戏")
        );
    parser.addOption(fullscreenOption);

    // 窗口宽度选项
    QCommandLineOption widthOption(
        QStringList() << "width",
        QObject::tr("设置窗口宽度（像素）"),
        QObject::tr("宽度")
        );
    parser.addOption(widthOption);

    // 窗口高度选项
    QCommandLineOption heightOption(
        QStringList() << "height",
        QObject::tr("设置窗口高度（像素）"),
        QObject::tr("高度")
        );
    parser.addOption(heightOption);

    // 解析命令行参数
    parser.process(a);

    // ============================================================
    // 3. 加载游戏配置
    // ============================================================

    // 获取配置单例并从系统配置目录加载
    GameConfig& config = GameConfig::instance();
    config.load();

    // 应用命令行参数覆盖配置
    if (parser.isSet(debugOption)) {
        config.debugMode = true;
        config.showFPS = true;
    }

    if (parser.isSet(fullscreenOption)) {
        config.fullscreen = true;
    }

    if (parser.isSet(widthOption)) {
        int width = parser.value(widthOption).toInt();
        if (width >= GameConstants::MIN_WINDOW_WIDTH) {
            config.windowSize.setWidth(width);
        }
    }

    if (parser.isSet(heightOption)) {
        int height = parser.value(heightOption).toInt();
        if (height >= GameConstants::MIN_WINDOW_HEIGHT) {
            config.windowSize.setHeight(height);
        }
    }

    // ============================================================
    // 4. 输出启动信息
    // ============================================================

    qDebug() << "========================================";
    qDebug() << "  " << GameConstants::GAME_NAME << "(" << GameConstants::GAME_NAME_EN << ")";
    qDebug() << "  版本: " << GameConstants::GAME_VERSION;
    qDebug() << "========================================";
    qDebug() << "窗口大小: " << config.windowSize.width() << "x" << config.windowSize.height();
    qDebug() << "全屏模式: " << (config.fullscreen ? "是" : "否");
    qDebug() << "调试模式: " << (config.debugMode ? "是" : "否");

    if (config.debugMode) {
        qDebug() << "[调试] 调试模式已启用，FPS显示已开启";
    }

    // ============================================================
    // 5. 创建并初始化游戏引擎
    // ============================================================

    qDebug() << "[主程序] 正在创建游戏引擎...";

    GameEngine engine;

    if (!engine.initialize()) {
        qCritical() << "[主程序] 游戏引擎初始化失败！";
        qCritical() << "[主程序] 请检查配置和资源文件是否正确";
        return -1;
    }

    qDebug() << "[主程序] 游戏引擎初始化完成";

    // ============================================================
    // 6. 运行游戏主循环
    // ============================================================

    qDebug() << "[主程序] 启动游戏主循环...";

    int result = engine.run();

    // ============================================================
    // 7. 游戏退出，保存配置
    // ============================================================

    qDebug() << "[主程序] 游戏退出，保存配置...";
    config.save();

    qDebug() << "[主程序] 程序正常退出，返回码:" << result;
    qDebug() << "========================================";

    return result;
}
