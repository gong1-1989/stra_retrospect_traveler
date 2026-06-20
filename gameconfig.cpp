#include "GameConfig.h"
#include <QSettings>

// ============================================================
// 单例实现
// ============================================================
GameConfig& GameConfig::instance()
{
    static GameConfig config;
    return config;
}

// ============================================================
// 加载配置（使用 QSettings，自动从系统配置目录读取）
// ============================================================
void GameConfig::load()
{
    QSettings settings;
    
    // 窗口配置
    windowSize.setWidth(settings.value(GameConstants::CONFIG_WINDOW_WIDTH, 
                                        GameConstants::DEFAULT_WINDOW_WIDTH).toInt());
    windowSize.setHeight(settings.value(GameConstants::CONFIG_WINDOW_HEIGHT, 
                                         GameConstants::DEFAULT_WINDOW_HEIGHT).toInt());
    fullscreen = settings.value(GameConstants::CONFIG_FULLSCREEN, false).toBool();
    vsync = settings.value("window/vsync", true).toBool();
    
    // 图形配置
    targetFPS = settings.value("graphics/targetFPS", GameConstants::DEFAULT_FPS).toInt();
    enableParticles = settings.value("graphics/enableParticles", true).toBool();
    textureQuality = settings.value("graphics/textureQuality", 2).toInt();
    
    // 音频配置
    masterVolume = settings.value(GameConstants::CONFIG_MASTER_VOLUME, 
                                   GameConstants::DEFAULT_MASTER_VOLUME).toFloat();
    musicVolume = settings.value(GameConstants::CONFIG_MUSIC_VOLUME, 
                                  GameConstants::DEFAULT_MUSIC_VOLUME).toFloat();
    sfxVolume = settings.value(GameConstants::CONFIG_SFX_VOLUME, 
                                GameConstants::DEFAULT_SFX_VOLUME).toFloat();
    muteAudio = settings.value("audio/mute", false).toBool();
    
    // 游戏配置
    debugMode = settings.value("gameplay/debugMode", false).toBool();
    showFPS = settings.value("gameplay/showFPS", false).toBool();
    showCollision = settings.value("gameplay/showCollision", false).toBool();
    language = settings.value(GameConstants::CONFIG_LANGUAGE, "zh_CN").toString();
    int diff = settings.value(GameConstants::CONFIG_DIFFICULTY, 
                               static_cast<int>(Difficulty::Normal)).toInt();
    difficulty = static_cast<Difficulty>(diff);
    
    // 控制配置
    mouseSensitivity = settings.value("controls/mouseSensitivity", 1.0f).toFloat();
    invertY = settings.value("controls/invertY", false).toBool();
}

// ============================================================
// 保存配置（使用 QSettings，自动保存到系统配置目录）
// ============================================================
void GameConfig::save() const
{
    QSettings settings;
    
    // 窗口配置
    settings.setValue(GameConstants::CONFIG_WINDOW_WIDTH, windowSize.width());
    settings.setValue(GameConstants::CONFIG_WINDOW_HEIGHT, windowSize.height());
    settings.setValue(GameConstants::CONFIG_FULLSCREEN, fullscreen);
    settings.setValue("window/vsync", vsync);
    
    // 图形配置
    settings.setValue("graphics/targetFPS", targetFPS);
    settings.setValue("graphics/enableParticles", enableParticles);
    settings.setValue("graphics/textureQuality", textureQuality);
    
    // 音频配置
    settings.setValue(GameConstants::CONFIG_MASTER_VOLUME, masterVolume);
    settings.setValue(GameConstants::CONFIG_MUSIC_VOLUME, musicVolume);
    settings.setValue(GameConstants::CONFIG_SFX_VOLUME, sfxVolume);
    settings.setValue("audio/mute", muteAudio);
    
    // 游戏配置
    settings.setValue("gameplay/debugMode", debugMode);
    settings.setValue("gameplay/showFPS", showFPS);
    settings.setValue("gameplay/showCollision", showCollision);
    settings.setValue(GameConstants::CONFIG_LANGUAGE, language);
    settings.setValue(GameConstants::CONFIG_DIFFICULTY, static_cast<int>(difficulty));
    
    // 控制配置
    settings.setValue("controls/mouseSensitivity", mouseSensitivity);
    settings.setValue("controls/invertY", invertY);
    
    settings.sync();
}

// ============================================================
// 重置为默认配置
// ============================================================
void GameConfig::resetToDefaults()
{
    QSettings settings;
    settings.clear();
    settings.sync();
    
    // 重新加载默认值
    windowSize = QSize(GameConstants::DEFAULT_WINDOW_WIDTH, 
                       GameConstants::DEFAULT_WINDOW_HEIGHT);
    fullscreen = false;
    vsync = true;
    targetFPS = GameConstants::DEFAULT_FPS;
    enableParticles = true;
    textureQuality = 2;
    masterVolume = GameConstants::DEFAULT_MASTER_VOLUME;
    musicVolume = GameConstants::DEFAULT_MUSIC_VOLUME;
    sfxVolume = GameConstants::DEFAULT_SFX_VOLUME;
    muteAudio = false;
    debugMode = false;
    showFPS = false;
    showCollision = false;
    language = "zh_CN";
    difficulty = Difficulty::Normal;
    mouseSensitivity = 1.0f;
    invertY = false;
}
