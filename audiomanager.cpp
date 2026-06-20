/**
 * @file AudioManager.cpp
 * @brief 音频管理器类实现
 * 
 * 本文件实现了音频管理器的所有功能，包括：
 * - 背景音乐的播放、暂停、恢复、停止
 * - 音效的播放和缓存管理
 * - 音量控制（主音量、音乐音量、音效音量）
 * - 静音控制
 * 
 * 音频架构说明：
 * - 背景音乐使用 QMediaPlayer，支持流媒体播放，适合长音频（如 BGM）
 * - 音效使用 QSoundEffect，支持低延迟播放，适合短音效（如点击、攻击）
 * - 音量采用三级控制：主音量 × 分类音量 = 实际音量
 * 
 * @author 星溯旅人开发团队
 * @version 0.1.0
 * @date 2026
 */

#include "AudioManager.h"

#include <QDebug>
#include <QUrl>
#include <QtMath>

// ============================================================
// 构造函数与析构函数
// ============================================================

/**
 * @brief 构造函数
 * 
 * 创建音频管理器实例，但不进行任何初始化操作。
 * 所有的初始化工作都在 initialize() 中完成。
 * 
 * @param parent 父对象，用于 Qt 对象树内存管理
 */
AudioManager::AudioManager(QObject* parent)
    : QObject(parent)
{
    qDebug() << "[AudioManager] 创建音频管理器";
}

/**
 * @brief 析构函数
 * 
 * 销毁音频管理器，自动调用 shutdown() 清理所有资源。
 */
AudioManager::~AudioManager()
{
    qDebug() << "[AudioManager] 销毁音频管理器";
    shutdown();
}

// ============================================================
// 生命周期管理
// ============================================================

/**
 * @brief 初始化音频管理器
 * 
 * 执行以下初始化操作：
 * 1. 检查是否已经初始化（防止重复初始化）
 * 2. 创建音乐播放器（QMediaPlayer）
 * 3. 创建音频输出设备（QAudioOutput）
 * 4. 将音频输出设备绑定到播放器
 * 5. 设置初始音量
 * 
 * @return true 初始化成功
 * @return false 初始化失败
 */
bool AudioManager::initialize()
{
    // 防止重复初始化
    if (m_initialized) {
        qWarning() << "[AudioManager] 音频管理器已经初始化，跳过重复初始化";
        return true;
    }

    qDebug() << "[AudioManager] 初始化音频管理器...";

    // 1. 创建音乐播放器
    qDebug() << "[AudioManager] 创建音乐播放器";
    m_musicPlayer = new QMediaPlayer(this);
    
    // 2. 创建音频输出设备
    qDebug() << "[AudioManager] 创建音频输出设备";
    m_musicAudioOutput = new QAudioOutput(this);
    
    // 3. 将音频输出设备绑定到播放器
    m_musicPlayer->setAudioOutput(m_musicAudioOutput);
    
    // 4. 设置初始音量
    qDebug() << "[AudioManager] 设置初始音量 - 主音量:" << m_masterVolume 
             << "音乐音量:" << m_musicVolume 
             << "音效音量:" << m_soundVolume;
    updateVolumes();

    // 初始化完成
    m_initialized = true;
    
    qDebug() << "[AudioManager] 音频管理器初始化完成";
    return true;
}

/**
 * @brief 关闭音频管理器
 * 
 * 执行以下清理操作：
 * 1. 停止背景音乐播放
 * 2. 清理所有音效缓存
 * 3. 重置状态
 * 
 * 注意：QMediaPlayer 和 QAudioOutput 会通过 Qt 对象树自动释放，
 * 不需要手动 delete。
 * 
 * 此函数是幂等的，可以安全地多次调用。
 */
void AudioManager::shutdown()
{
    // 如果没有初始化，直接返回
    if (!m_initialized) {
        return;
    }

    qDebug() << "[AudioManager] 关闭音频管理器...";

    // 1. 停止背景音乐
    if (m_musicPlayer) {
        qDebug() << "[AudioManager] 停止背景音乐播放";
        m_musicPlayer->stop();
    }

    // 2. 清理音效缓存
    qDebug() << "[AudioManager] 清理音效缓存，数量:" << m_soundEffects.size();
    qDeleteAll(m_soundEffects);
    m_soundEffects.clear();

    // 重置状态
    m_initialized = false;
    
    qDebug() << "[AudioManager] 音频管理器已关闭";
}

// ============================================================
// 背景音乐控制
// ============================================================

/**
 * @brief 播放背景音乐
 * 
 * 加载并播放指定的背景音乐文件。
 * 如果已经有音乐在播放，会先停止再播放新的。
 * 
 * 支持的路径格式：
 * - 资源路径：":/audio/bgm/main_theme.mp3"
 * - 本地路径："/path/to/music.mp3"
 * - URL："file:///path/to/music.mp3"
 * 
 * @param path 音乐文件路径
 * @param loop 是否循环播放，默认为 true
 */
void AudioManager::playMusic(const QString& path, bool loop)
{
    // 检查播放器是否存在
    if (!m_musicPlayer) {
        qWarning() << "[AudioManager] 音乐播放器未初始化，无法播放音乐";
        return;
    }

    qDebug() << "[AudioManager] 播放背景音乐:" << path 
             << "循环:" << (loop ? "是" : "否");

    // 1. 设置音乐源
    m_musicPlayer->setSource(QUrl::fromLocalFile(path));
    
    // 2. 设置循环模式
    if (loop) {
        // 无限循环
        m_musicPlayer->setLoops(QMediaPlayer::Infinite);
    } else {
        // 只播放一次
        m_musicPlayer->setLoops(QMediaPlayer::Once);
    }
    
    // 3. 开始播放
    m_musicPlayer->play();
}

/**
 * @brief 暂停背景音乐
 * 
 * 暂停当前播放的音乐，保持播放位置。
 * 调用 resumeMusic() 可以从暂停位置继续播放。
 * 
 * 如果音乐未播放或已暂停，则不做任何操作。
 */
void AudioManager::pauseMusic()
{
    if (m_musicPlayer && m_musicPlayer->playbackState() == QMediaPlayer::PlayingState) {
        qDebug() << "[AudioManager] 暂停背景音乐";
        m_musicPlayer->pause();
    }
}

/**
 * @brief 恢复背景音乐
 * 
 * 从暂停位置继续播放背景音乐。
 * 如果音乐已停止，则从头开始播放。
 * 
 * 如果音乐已经在播放，则不做任何操作。
 */
void AudioManager::resumeMusic()
{
    if (m_musicPlayer && m_musicPlayer->playbackState() != QMediaPlayer::PlayingState) {
        qDebug() << "[AudioManager] 恢复背景音乐播放";
        m_musicPlayer->play();
    }
}

/**
 * @brief 停止背景音乐
 * 
 * 停止播放背景音乐，播放位置重置为开头。
 * 
 * 如果音乐已经停止，则不做任何操作。
 */
void AudioManager::stopMusic()
{
    if (m_musicPlayer && m_musicPlayer->playbackState() != QMediaPlayer::StoppedState) {
        qDebug() << "[AudioManager] 停止背景音乐播放";
        m_musicPlayer->stop();
    }
}

// ============================================================
// 音效控制
// ============================================================

/**
 * @brief 播放音效
 * 
 * 播放指定的音效文件。
 * 音效会自动缓存，第一次播放时加载，后续播放直接使用缓存。
 * 
 * 音效的实际音量计算：
 * actualVolume = volume × 音效音量 × 主音量
 * 如果静音，则 actualVolume = 0
 * 
 * 注意：
 * - QSoundEffect 只支持 WAV 格式，不支持 MP3 等压缩格式
 * - 音效可以多个同时播放，互不干扰
 * - 音效播放完成后会自动停止
 * 
 * @param path 音效文件路径（支持资源路径 :/ 和本地路径）
 * @param volume 音效音量倍率（0.0 - 1.0），默认为 1.0
 */
void AudioManager::playSound(const QString& path, float volume)
{
    // 1. 检查缓存中是否已有该音效
    if (!m_soundEffects.contains(path)) {
        qDebug() << "[AudioManager] 加载新音效:" << path;
        
        // 创建新的音效对象
        QSoundEffect* sound = new QSoundEffect(this);
        sound->setSource(QUrl::fromLocalFile(path));
        
        // 添加到缓存
        m_soundEffects[path] = sound;
    }
    
    // 2. 获取音效对象
    QSoundEffect* sound = m_soundEffects[path];
    if (sound) {
        // 3. 计算实际音量
        float actualVolume = volume * m_soundVolume * m_masterVolume;
        
        // 4. 如果静音，音量设为 0
        if (m_muted) {
            actualVolume = 0.0f;
        }
        
        // 5. 确保音量在有效范围内
        actualVolume = qBound(0.0f, actualVolume, 1.0f);
        
        // 6. 设置音量并播放
        sound->setVolume(actualVolume);
        sound->play();
    }
}

// ============================================================
// 音量控制
// ============================================================

/**
 * @brief 设置主音量
 * 
 * 主音量会影响所有音频（音乐 + 音效）。
 * 实际音量 = 主音量 × 分类音量。
 * 
 * 设置后会自动调用 updateVolumes() 更新所有音频的实际音量。
 * 
 * @param volume 音量值（0.0 - 1.0），超出范围会被自动限制
 */
void AudioManager::setMasterVolume(float volume)
{
    // 限制音量在有效范围内
    m_masterVolume = qBound(0.0f, volume, 1.0f);
    
    qDebug() << "[AudioManager] 设置主音量:" << m_masterVolume;
    
    // 更新所有音量
    updateVolumes();
}

/**
 * @brief 设置音乐音量
 * 
 * 设置背景音乐的音量倍率。
 * 实际音乐音量 = 主音量 × 音乐音量。
 * 
 * 设置后会自动调用 updateVolumes() 更新音乐的实际音量。
 * 
 * @param volume 音量值（0.0 - 1.0），超出范围会被自动限制
 */
void AudioManager::setMusicVolume(float volume)
{
    // 限制音量在有效范围内
    m_musicVolume = qBound(0.0f, volume, 1.0f);
    
    qDebug() << "[AudioManager] 设置音乐音量:" << m_musicVolume;
    
    // 更新所有音量
    updateVolumes();
}

/**
 * @brief 设置音效音量
 * 
 * 设置音效的音量倍率。
 * 实际音效音量 = 主音量 × 音效音量。
 * 
 * 注意：已播放的音效不会受到影响，只有新播放的音效会使用新的音量。
 * 如果需要实时调整所有音效音量，需要遍历缓存中的所有音效。
 * 
 * @param volume 音量值（0.0 - 1.0），超出范围会被自动限制
 */
void AudioManager::setSoundVolume(float volume)
{
    // 限制音量在有效范围内
    m_soundVolume = qBound(0.0f, volume, 1.0f);
    
    qDebug() << "[AudioManager] 设置音效音量:" << m_soundVolume;
    
    // 注意：已播放的音效不会受到影响
    // 如果需要实时调整，需要遍历 m_soundEffects 并设置每个音效的音量
}

// ============================================================
// 静音控制
// ============================================================

/**
 * @brief 设置静音
 * 
 * 一键静音/取消静音所有音频。
 * 静音时音量设为 0，但保留原来的音量设置。
 * 取消静音后会恢复原来的音量。
 * 
 * 设置后会自动调用 updateVolumes() 更新所有音频的实际音量。
 * 
 * @param muted true 静音，false 取消静音
 */
void AudioManager::setMuted(bool muted)
{
    m_muted = muted;
    
    qDebug() << "[AudioManager] 设置静音:" << (m_muted ? "是" : "否");
    
    // 更新所有音量
    updateVolumes();
}

// ============================================================
// 内部方法
// ============================================================

/**
 * @brief 更新所有音量
 * 
 * 根据当前的音量设置和静音状态，更新所有音频输出的实际音量。
 * 在音量设置改变或静音状态改变时调用。
 * 
 * 当前实现只更新音乐音量，因为音效是在播放时计算音量的。
 * 如果需要实时调整所有音效音量，需要遍历 m_soundEffects。
 * 
 * 音乐实际音量计算：
 * musicActualVolume = 静音 ? 0 : (音乐音量 × 主音量)
 */
void AudioManager::updateVolumes()
{
    // 更新音乐音量
    if (m_musicAudioOutput) {
        // 计算实际音量
        float volume = m_muted ? 0.0f : (m_musicVolume * m_masterVolume);
        
        // 确保音量在有效范围内
        volume = qBound(0.0f, volume, 1.0f);
        
        // 设置音量
        m_musicAudioOutput->setVolume(volume);
    }
    
    // 注意：音效的音量在播放时计算，这里不需要更新
    // 如果需要实时调整所有音效音量，可以取消下面的注释：
    /*
    for (QSoundEffect* sound : m_soundEffects) {
        float volume = m_muted ? 0.0f : (m_soundVolume * m_masterVolume);
        sound->setVolume(volume);
    }
    */
}
