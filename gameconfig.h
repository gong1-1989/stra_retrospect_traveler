#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <QString>
#include <QSize>
#include "GameConstants.h"

/**
 * @brief 游戏配置类
 * 
 * 使用 Qt QSettings 自动管理配置，无需手动处理文件路径
 */
class GameConfig
{
public:
    // 窗口配置
    QSize windowSize = QSize(GameConstants::DEFAULT_WINDOW_WIDTH, 
                              GameConstants::DEFAULT_WINDOW_HEIGHT);
    bool fullscreen = false;
    bool vsync = true;
    
    // 图形配置
    int targetFPS = GameConstants::DEFAULT_FPS;
    bool enableParticles = true;
    int textureQuality = 2; // 0:低, 1:中, 2:高
    
    // 音频配置
    float masterVolume = GameConstants::DEFAULT_MASTER_VOLUME;
    float musicVolume = GameConstants::DEFAULT_MUSIC_VOLUME;
    float sfxVolume = GameConstants::DEFAULT_SFX_VOLUME;
    bool muteAudio = false;
    
    // 游戏配置
    bool debugMode = false;
    bool showFPS = false;
    bool showCollision = false;
    QString language = "zh_CN";
    Difficulty difficulty = Difficulty::Normal;
    
    // 控制配置
    float mouseSensitivity = 1.0f;
    bool invertY = false;
    
    /**
     * @brief 加载配置（使用 QSettings，自动从系统配置目录读取）
     */
    void load();
    
    /**
     * @brief 保存配置（使用 QSettings，自动保存到系统配置目录）
     */
    void save() const;
    
    /**
     * @brief 重置为默认配置
     */
    void resetToDefaults();
    
    /**
     * @brief 获取单例实例
     */
    static GameConfig& instance();
    
private:
    GameConfig() = default;
    ~GameConfig() = default;
    GameConfig(const GameConfig&) = delete;
    GameConfig& operator=(const GameConfig&) = delete;
};

#endif // GAMECONFIG_H
