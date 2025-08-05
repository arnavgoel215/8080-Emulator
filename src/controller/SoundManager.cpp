
#include "SoundManager.hpp"
#include <QUrl>


SoundManager::SoundManager(QObject *parent) : QObject(parent)
{
    loadSounds();
}

SoundManager::~SoundManager() = default;

void SoundManager::loadSounds()
{
    playerShootPlayer = std::make_unique<QMediaPlayer>();
    playerShootOutput = std::make_unique<QAudioOutput>();
    playerShootPlayer->setAudioOutput(playerShootOutput.get());
    playerShootPlayer->setSource(QUrl("qrc:/sounds/shoot.wav"));
    playerShootOutput->setVolume(0.5f);

    invaderKilledPlayer = std::make_unique<QMediaPlayer>();
    invaderKilledOutput = std::make_unique<QAudioOutput>();
    invaderKilledPlayer->setAudioOutput(invaderKilledOutput.get());
    invaderKilledPlayer->setSource(QUrl("qrc:/sounds/invaderkilled.wav"));
    invaderKilledOutput->setVolume(0.5f);

    playerKilledPlayer = std::make_unique<QMediaPlayer>();
    playerKilledOutput = std::make_unique<QAudioOutput>();
    playerKilledPlayer->setAudioOutput(playerKilledOutput.get());
    playerKilledPlayer->setSource(QUrl("qrc:/sounds/explosion.wav"));
    playerKilledOutput->setVolume(0.5f);

    ufoPlayer = std::make_unique<QMediaPlayer>();
    ufoOutput = std::make_unique<QAudioOutput>();
    ufoPlayer->setAudioOutput(ufoOutput.get());
    ufoPlayer->setSource(QUrl("qrc:/sounds/ufo_highpitch.wav"));
    ufoOutput->setVolume(0.25f);
    // Looping for UFO sound
    ufoPlayer->setLoops(QMediaPlayer::Infinite);

    invaderMove1Player = std::make_unique<QMediaPlayer>();
    invaderMove1Output = std::make_unique<QAudioOutput>();
    invaderMove1Player->setAudioOutput(invaderMove1Output.get());
    invaderMove1Player->setSource(QUrl("qrc:/sounds/fastinvader1.wav"));
    invaderMove1Output->setVolume(0.5f);

    invaderMove2Player = std::make_unique<QMediaPlayer>();
    invaderMove2Output = std::make_unique<QAudioOutput>();
    invaderMove2Player->setAudioOutput(invaderMove2Output.get());
    invaderMove2Player->setSource(QUrl("qrc:/sounds/fastinvader2.wav"));
    invaderMove2Output->setVolume(0.5f);

    invaderMove3Player = std::make_unique<QMediaPlayer>();
    invaderMove3Output = std::make_unique<QAudioOutput>();
    invaderMove3Player->setAudioOutput(invaderMove3Output.get());
    invaderMove3Player->setSource(QUrl("qrc:/sounds/fastinvader3.wav"));
    invaderMove3Output->setVolume(0.5f);

    invaderMove4Player = std::make_unique<QMediaPlayer>();
    invaderMove4Output = std::make_unique<QAudioOutput>();
    invaderMove4Player->setAudioOutput(invaderMove4Output.get());
    invaderMove4Player->setSource(QUrl("qrc:/sounds/fastinvader4.wav"));
    invaderMove4Output->setVolume(0.5f);
}

void SoundManager::playPlayerShoot()
{
    playerShootPlayer->stop();
    playerShootPlayer->play();
}

void SoundManager::playInvaderKilled()
{
    invaderKilledPlayer->stop();
    invaderKilledPlayer->play();
}

void SoundManager::playPlayerKilled()
{
    playerKilledPlayer->stop();
    playerKilledPlayer->play();
}

void SoundManager::playUfo()
{
    if (ufoPlayer->playbackState() != QMediaPlayer::PlayingState) {
        ufoPlayer->play();
    }
}

void SoundManager::playInvaderMove1()
{
    invaderMove1Player->stop();
    invaderMove1Player->play();
}

void SoundManager::playInvaderMove2()
{
    invaderMove2Player->stop();
    invaderMove2Player->play();
}

void SoundManager::playInvaderMove3()
{
    invaderMove3Player->stop();
    invaderMove3Player->play();
}

void SoundManager::playInvaderMove4()
{
    invaderMove4Player->stop();
    invaderMove4Player->play();
}

void SoundManager::stopUfo()
{
    ufoPlayer->stop();
}
