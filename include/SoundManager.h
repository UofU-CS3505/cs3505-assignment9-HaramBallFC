#pragma once

// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galvan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// Data types for lessons (content shown in Fan/Player modes).

#include <QSoundEffect>
#include <QMediaPlayer>
#include <QAudioOutput>

class SoundManager final
{
public:
    static SoundManager& instance();

    void playClick();
    void playCorrect();
    void playWrong();
    void playPass();
    void playFail();
    void playJuggle();
    void playThud();
    void playCeiling();

    void startBgm();
    void stopBgm();
    void setBgmVolume(float volume);  // 0.0 – 1.0

private:
    SoundManager();

    QSoundEffect m_click;
    QSoundEffect m_correct;
    QSoundEffect m_wrong;
    QSoundEffect m_pass;
    QSoundEffect m_fail;
    QSoundEffect m_juggle;
    QSoundEffect m_thud;
    QSoundEffect m_ceiling;

    QMediaPlayer  *m_bgmPlayer;
    QAudioOutput  *m_bgmAudio;
};
