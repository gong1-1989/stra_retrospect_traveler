/**
 * @file AudioManager.h
 * @brief 音频管理器类
 * 
 * 音频管理器负责游戏中所有音频的播放、控制和管理，包括：
 * - 背景音乐（BGM）的播放、暂停、停止、循环
 * - 音效（SFX）的播放和管理
 * - 音量控制（主音量、音乐音量、音效音量）
 * - 静音控制
 * 
 * 主要特性：
 * 1. 背景音乐：支持循环播放、暂停、恢复、停止
 * 2. 音效系统：支持多音效同时播放，自动缓存
 * 3. 音量控制：三级音量控制（主音量、音乐、音效）
 * 4. 静音开关：一键静音/取消静音
 * 
 * 使用示例：
 * @code
 * AudioManager audio;
 * audio.initialize();
 * 
 * // 播放背景音乐
 * audio.playMusic(":/audio/bgm/main_theme.mp3", true);
 * 
 * // 播放音效
 * audio.playSound(":/audio/sfx/click.wav");
 * 
 * // 设置音量
 * audio.setMusicVolume(0.5f);
 * @endcode
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QMap>
#include <QString>

/**
 * @brief 音频管理器类
 * 
 * 负责游戏音频的播放、控制和管理。
 * 提供背景音乐、音效、音量控制等功能。
 * 
 * 音频架构：
 * - 背景音乐：使用 QMediaPlayer，支持流媒体播放，适合长音频
 * - 音效：使用 QSoundEffect，支持低延迟播放，适合短音效
 * - 音量控制：主音量 × 分类音量 = 实际音量
 * 
 * 注意：QSoundEffect 只支持 WAV 格式，不支持 MP3 等压缩格式。
 * 如果需要播放 MP3 音效，可以使用 QMediaPlayer，但延迟会更高。
 */
class AudioManager : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * 
     * 创建音频管理器实例，但不进行初始化。
     * 必须调用 initialize() 完成初始化后才能使用。
     * 
     * @param parent 父对象，用于 Qt 对象树内存管理
     */
    explicit AudioManager(QObject* parent = nullptr);
    
    /**
     * @brief 析构函数
     * 
     * 销毁音频管理器，自动调用 shutdown() 清理资源。
     */
    ~AudioManager() override;

    // ============================================================
    // 生命周期管理
    // ============================================================
    
    /**
     * @brief 初始化音频管理器
     * 
     * 创建音乐播放器和音频输出设备，设置初始音量。
     * 
     * @return true 初始化成功
     * @return false 初始化失败
     */
    bool initialize();
    
    /**
     * @brief 关闭音频管理器
     * 
     * 停止所有音频播放，清理音效缓存，释放资源。
     */
    void shutdown();

    // ============================================================
    // 背景音乐控制
    // ============================================================
    
    /**
     * @brief 播放背景音乐
     * 
     * 加载并播放指定的背景音乐文件。
     * 如果已经有音乐在播放，会先停止再播放新的。
     * 
     * @param path 音乐文件路径（支持资源路径 :/ 和本地路径）
     * @param loop 是否循环播放，默认为 true
     */
    void playMusic(const QString& path, bool loop = true);
    
    /**
     * @brief 暂停背景音乐
     * 
     * 暂停当前播放的音乐，保持播放位置。
     * 调用 resumeMusic() 可以从暂停位置继续播放。
     */
    void pauseMusic();
    
    /**
     * @brief 恢复背景音乐
     * 
     * 从暂停位置继续播放背景音乐。
     * 如果音乐已停止，则从头开始播放。
     */
    void resumeMusic();
    
    /**
     * @brief 停止背景音乐
     * 
     * 停止播放背景音乐，播放位置重置为开头。
     */
    void stopMusic();

    // ============================================================
    // 音效控制
    // ============================================================
    
    /**
     * @brief 播放音效
     * 
     * 播放指定的音效文件。
     * 音效会自动缓存，第一次播放时加载，后续播放直接使用缓存。
     * 
     * 注意：QSoundEffect 只支持 WAV 格式。
     * 
     * @param path 音效文件路径（支持资源路径 :/ 和本地路径）
     * @param volume 音效音量倍率（0.0 - 1.0），默认为 1.0
     */
    void playSound(const QString& path, float volume = 1.0f);

    // ============================================================
    // 音量控制
    // ============================================================
    
    /**
     * @brief 设置主音量
     * 
     * 主音量会影响所有音频（音乐 + 音效）。
     * 实际音量 = 主音量 × 分类音量。
     * 
     * @param volume 音量值（0.0 - 1.0）
     */
    void setMasterVolume(float volume);
    
    /**
     * @brief 设置音乐音量
     * 
     * 设置背景音乐的音量倍率。
     * 实际音乐音量 = 主音量 × 音乐音量。
     * 
     * @param volume 音量值（0.0 - 1.0）
     */
    void setMusicVolume(float volume);
    
    /**
     * @brief 设置音效音量
     * 
     * 设置音效的音量倍率。
     * 实际音效音量 = 主音量 × 音效音量。
     * 
     * @param volume 音量值（0.0 - 1.0）
     */
    void setSoundVolume(float volume);
    
    /**
     * @brief 获取主音量
     * @return 主音量值（0.0 - 1.0）
     */
    float getMasterVolume() const { return m_masterVolume; }
    
    /**
     * @brief 获取音乐音量
     * @return 音乐音量值（0.0 - 1.0）
     */
    float getMusicVolume() const { return m_musicVolume; }
    
    /**
     * @brief 获取音效音量
     * @return 音效音量值（0.0 - 1.0）
     */
    float getSoundVolume() const { return m_soundVolume; }

    // ============================================================
    // 静音控制
    // ============================================================
    
    /**
     * @brief 设置静音
     * 
     * 一键静音/取消静音所有音频。
     * 静音时音量设为 0，但保留原来的音量设置。
     * 
     * @param muted true 静音，false 取消静音
     */
    void setMuted(bool muted);
    
    /**
     * @brief 是否静音
     * @return true 静音状态
     * @return false 非静音状态
     */
    bool isMuted() const { return m_muted; }

private:
    /**
     * @brief 更新所有音量
     * 
     * 根据当前的音量设置和静音状态，更新所有音频输出的实际音量。
     * 在音量设置改变或静音状态改变时调用。
     */
    void updateVolumes();

    // ============================================================
    // 成员变量
    // ============================================================
    
    // 背景音乐播放器
    QMediaPlayer* m_musicPlayer = nullptr;       ///< 音乐播放器
    QAudioOutput* m_musicAudioOutput = nullptr;  ///< 音乐音频输出设备
    
    // 音量设置
    float m_masterVolume = 1.0f;  ///< 主音量（0.0 - 1.0）
    float m_musicVolume = 0.8f;   ///< 音乐音量倍率（0.0 - 1.0）
    float m_soundVolume = 1.0f;   ///< 音效音量倍率（0.0 - 1.0）
    bool m_muted = false;         ///< 是否静音
    
    // 音效缓存
    QMap<QString, QSoundEffect*> m_soundEffects;  ///< 音效缓存（路径 -> 音效对象）
    
    // 状态
    bool m_initialized = false;  ///< 是否已初始化
};

#endif // AUDIO_MANAGER_H
