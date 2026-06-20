#ifndef GAMECONSTANTS_H
#define GAMECONSTANTS_H

#include <QString>

// ============================================================
// 游戏基础常量
// ============================================================
namespace GameConstants {

// 游戏信息
constexpr const char* GAME_NAME = "星溯旅人";
constexpr const char* GAME_NAME_EN = "Star Retrospect Traveler";
constexpr const char* GAME_VERSION = "0.1.0";

// 窗口设置
constexpr int DEFAULT_WINDOW_WIDTH = 1280;
constexpr int DEFAULT_WINDOW_HEIGHT = 720;
constexpr int MIN_WINDOW_WIDTH = 800;
constexpr int MIN_WINDOW_HEIGHT = 600;

// 游戏设置
constexpr int MAX_LEVEL = 50;
constexpr int MAX_PARTY_SIZE = 3;
constexpr int MAX_SYMBIOSIS_SLOTS = 3;
constexpr int DEFAULT_FPS = 60;
constexpr int MIN_FPS = 30;

// 时间设置
constexpr int AUTO_SAVE_INTERVAL = 300;  // 秒
constexpr float GAME_SPEED_DEFAULT = 1.0f;
constexpr float GAME_SPEED_MIN = 0.5f;
constexpr float GAME_SPEED_MAX = 2.0f;

// 玩家属性
constexpr int DEFAULT_MAX_HEALTH = 100;
constexpr int DEFAULT_MAX_MANA = 50;
constexpr float DEFAULT_MOVE_SPEED = 200.0f;

// 音频设置
constexpr float DEFAULT_MASTER_VOLUME = 0.8f;
constexpr float DEFAULT_MUSIC_VOLUME = 0.6f;
constexpr float DEFAULT_SFX_VOLUME = 0.7f;
constexpr float VOLUME_MIN = 0.0f;
constexpr float VOLUME_MAX = 1.0f;

// 资源路径
constexpr const char* RESOURCE_PREFIX = ":/";
constexpr const char* IMAGE_PATH = ":/assets/images/";
constexpr const char* AUDIO_PATH = ":/assets/audio/";
constexpr const char* FONT_PATH = ":/assets/fonts/";
constexpr const char* CONFIG_PATH = ":/assets/config/";

// 配置键名
constexpr const char* CONFIG_WINDOW_WIDTH = "window/width";
constexpr const char* CONFIG_WINDOW_HEIGHT = "window/height";
constexpr const char* CONFIG_FULLSCREEN = "window/fullscreen";
constexpr const char* CONFIG_MASTER_VOLUME = "audio/masterVolume";
constexpr const char* CONFIG_MUSIC_VOLUME = "audio/musicVolume";
constexpr const char* CONFIG_SFX_VOLUME = "audio/sfxVolume";
constexpr const char* CONFIG_DIFFICULTY = "gameplay/difficulty";
constexpr const char* CONFIG_LANGUAGE = "gameplay/language";

} // namespace GameConstants

// ============================================================
// 枚举类型
// ============================================================
enum class GameState {
    Menu,
    Playing,
    Paused,
    GameOver,
    Victory
};

enum class Difficulty {
    Easy,
    Normal,
    Hard,
    Extreme
};

enum class SceneType {
    MainMenu,
    GameScene,
    Settings,
    Credits
};

#endif // GAMECONSTANTS_H
