#include "SoundManager.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSaveFile>
#include <QUrl>

// Tries to make sure the sound exists as a local file.
// First it copies from the Qt resource file, and if that fails,
// it looks for the file in resources/sounds.
static QString ensureLocalWavFromResource(const QString& resourcePath, const QString& fileName)
{
    const QString tempDir = QDir::tempPath();
    const QString outPath = QDir(tempDir).filePath(QString("HaramBallFC-%1").arg(fileName));

    QFile in(resourcePath);
    if (!in.open(QIODevice::ReadOnly)) {
        QDir dir(QCoreApplication::applicationDirPath());
        for (int i = 0; i < 10; ++i) {
            const QString candidate = dir.absoluteFilePath(QString("resources/sounds/%1").arg(fileName));
            if (QFileInfo::exists(candidate))
                return candidate;
            if (!dir.cdUp())
                break;
        }
        return {};
    }

    // Copy the resource into a temp file so it can be loaded as a normal local file.
    QSaveFile out(outPath);
    if (!out.open(QIODevice::WriteOnly))
        return {};

    out.write(in.readAll());
    if (!out.commit())
        return {};

    return outPath;
}

// Sets up a sound effect from either a local file or the Qt resource path.
static void initEffect(QSoundEffect& effect, const QString& resourcePath, const QString& fileName)
{
    const QString localPath = ensureLocalWavFromResource(resourcePath, fileName);
    if (!localPath.isEmpty())
        effect.setSource(QUrl::fromLocalFile(localPath));
    else
        effect.setSource(QUrl(QString("qrc%1").arg(resourcePath)));

    effect.setVolume(0.8f);
}

SoundManager& SoundManager::instance()
{
    static SoundManager s_instance;
    return s_instance;
}

SoundManager::SoundManager()
    : m_bgmPlayer(new QMediaPlayer)
    , m_bgmAudio(new QAudioOutput)
{
    initEffect(m_click,   ":/sounds/click.wav",   "click.wav");
    initEffect(m_correct, ":/sounds/correct.wav", "correct.wav");
    initEffect(m_wrong,   ":/sounds/wrong.wav",   "wrong.wav");
    initEffect(m_pass,    ":/sounds/pass.wav",    "pass.wav");
    initEffect(m_fail,    ":/sounds/fail.wav",    "fail.wav");
    initEffect(m_juggle,  ":/sounds/juggle.wav",  "juggle.wav");
    initEffect(m_thud,    ":/sounds/thud.wav",    "thud.wav");
    initEffect(m_ceiling, ":/sounds/ceiling.wav", "ceiling.wav");

    const QString bgmPath = ensureLocalWavFromResource(":/sounds/background.mp3", "background.mp3");

    m_bgmPlayer->setAudioOutput(m_bgmAudio);
    m_bgmAudio->setVolume(0.35f);
    m_bgmPlayer->setLoops(QMediaPlayer::Infinite);

    if (!bgmPath.isEmpty())
        m_bgmPlayer->setSource(QUrl::fromLocalFile(bgmPath));
    else
        m_bgmPlayer->setSource(QUrl("qrc:/sounds/background.mp3"));
}

// Simple wrappers for each sound effect.
void SoundManager::playClick()   { 
    m_click.play(); 
}
void SoundManager::playCorrect() { 
    m_correct.play(); 
}
void SoundManager::playWrong()   { 
    m_wrong.play(); 
}
void SoundManager::playPass()    { 
    m_pass.play(); 
}
void SoundManager::playFail()    { 
    m_fail.play(); 
}
void SoundManager::playJuggle()  { 
    m_juggle.play(); 
}
void SoundManager::playThud()    { 
    m_thud.play(); 
}
void SoundManager::playCeiling() { 
    m_ceiling.play(); 
}

void SoundManager::startBgm() { 
    m_bgmPlayer->play(); 
}
void SoundManager::stopBgm()  {
    m_bgmPlayer->stop(); 
}

void SoundManager::setBgmVolume(float v)
{
    m_bgmAudio->setVolume(v);
}
