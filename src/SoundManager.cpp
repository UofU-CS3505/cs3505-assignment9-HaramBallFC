#include "SoundManager.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QCoreApplication>
#include <QSaveFile>
#include <QUrl>

// Ensures a sound file exists as a local file path by copying from qrc when possible,
// with a fallback search in resources/sounds relative to the app directory.
static QString ensureLocalWavFromResource(const QString& resourcePath, const QString& fileName)
{
    const QString tempDir = QDir::tempPath();
    const QString outPath = QDir(tempDir).filePath(QString("HaramBallFC-%1").arg(fileName));

    QFile in(resourcePath);
    if (!in.open(QIODevice::ReadOnly)) {
        // If qrc open fails, try locating a real file in resources/sounds up the directory tree.
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

    // Copy qrc contents to a temp file so APIs requiring local files can load it reliably.
    QSaveFile out(outPath);
    if (!out.open(QIODevice::WriteOnly))
        return {};

    out.write(in.readAll());
    if (!out.commit())
        return {};

    return outPath;
}

// Initializes a QSoundEffect source from local temp/fallback path, else directly from qrc URL.
static void initEffect(QSoundEffect& effect, const QString& resourcePath, const QString& fileName)
{
    const QString localPath = ensureLocalWavFromResource(resourcePath, fileName);
    if (!localPath.isEmpty())
        effect.setSource(QUrl::fromLocalFile(localPath));
    else
        effect.setSource(QUrl(QString("qrc%1").arg(resourcePath)));
    effect.setVolume(0.8f);
}

// Returns the singleton SoundManager instance.
SoundManager& SoundManager::instance()
{
    static SoundManager s_instance;
    return s_instance;
}

// Constructs SoundManager and initializes all SFX + looping background music.
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

// Plays UI click sound effect.
void SoundManager::playClick()          { m_click.play(); }

// Plays correct-answer sound effect.
void SoundManager::playCorrect()        { m_correct.play(); }

// Plays wrong-answer sound effect.
void SoundManager::playWrong()          { m_wrong.play(); }

// Plays pass/advance sound effect.
void SoundManager::playPass()           { m_pass.play(); }

// Plays fail/lose sound effect.
void SoundManager::playFail()           { m_fail.play(); }

// Plays juggling hit sound effect.
void SoundManager::playJuggle()         { m_juggle.play(); }

// Plays ground/impact thud sound effect.
void SoundManager::playThud()           { m_thud.play(); }

// Plays ceiling collision sound effect.
void SoundManager::playCeiling()        { m_ceiling.play(); }

// Starts looping background music playback.
void SoundManager::startBgm()           { m_bgmPlayer->play(); }

// Stops background music playback.
void SoundManager::stopBgm()            { m_bgmPlayer->stop(); }

// Sets background music volume (typically expected range is 0.0 to 1.0).
void SoundManager::setBgmVolume(float v){ m_bgmAudio->setVolume(v); }
