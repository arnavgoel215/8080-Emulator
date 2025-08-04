#include "SoundManager.hpp"
#include <QUrl>

SoundManager::SoundManager(QObject *parent) : QObject(parent)
{
    loadSounds();

    // Connect signals to slots. This ensures the play() calls happen on the correct thread.
    connect(this, &SoundManager::requestPlayPlayerShoot, this, &SoundManager::onPlayPlayerShoot);
    connect(this, &SoundManager::requestPlayInvaderKilled, this, &SoundManager::onPlayInvaderKilled);
    connect(this, &SoundManager::requestPlayPlayerKilled, this, &SoundManager::onPlayPlayerKilled);
    connect(this, &SoundManager::requestPlayUfo, this, &SoundManager::onPlayUfo);
    connect(this, &SoundManager::requestPlayInvaderMove1, this, &SoundManager::onPlayInvaderMove1);
    connect(this, &SoundManager::requestPlayInvaderMove2, this, &SoundManager::onPlayInvaderMove2);
    connect(this, &SoundManager::requestPlayInvaderMove3, this, &SoundManager::onPlayInvaderMove3);
    connect(this, &SoundManager::requestPlayInvaderMove4, this, &SoundManager::onPlayInvaderMove4);
    connect(this, &SoundManager::requestStopUfo, this, &SoundManager::onStopUfo);
}

SoundManager::~SoundManager() = default;

void SoundManager::loadSounds()
{
    playerShootSound = std::make_unique<QSoundEffect>();
    playerShootSound->setSource(QUrl("qrc:/sounds/shoot.wav"));
    playerShootSound->setVolume(0.5f);

    invaderKilledSound = std::make_unique<QSoundEffect>();
    invaderKilledSound->setSource(QUrl("qrc:/sounds/invaderkilled.wav"));
    invaderKilledSound->setVolume(0.5f);

    playerKilledSound = std::make_unique<QSoundEffect>();
    playerKilledSound->setSource(QUrl("qrc:/sounds/explosion.wav"));
    playerKilledSound->setVolume(0.5f);

    ufoSound = std::make_unique<QSoundEffect>();
    ufoSound->setSource(QUrl("qrc:/sounds/ufo_highpitch.wav"));
    ufoSound->setVolume(0.25f);
    ufoSound->setLoopCount(QSoundEffect::Infinite);

    invaderMove1Sound = std::make_unique<QSoundEffect>();
    invaderMove1Sound->setSource(QUrl("qrc:/sounds/fastinvader1.wav"));
    invaderMove1Sound->setVolume(0.5f);

    invaderMove2Sound = std::make_unique<QSoundEffect>();
    invaderMove2Sound->setSource(QUrl("qrc:/sounds/fastinvader2.wav"));
    invaderMove2Sound->setVolume(0.5f);

    invaderMove3Sound = std::make_unique<QSoundEffect>();
    invaderMove3Sound->setSource(QUrl("qrc:/sounds/fastinvader3.wav"));
    invaderMove3Sound->setVolume(0.5f);

    invaderMove4Sound = std::make_unique<QSoundEffect>();
    invaderMove4Sound->setSource(QUrl("qrc:/sounds/fastinvader4.wav"));
    invaderMove4Sound->setVolume(0.5f);
}

void SoundManager::onPlayPlayerShoot()
{
    if (playerShootSound->isLoaded()) {
        playerShootSound->play();
    }
}

void SoundManager::onPlayInvaderKilled()
{
    if (invaderKilledSound->isLoaded()) {
        invaderKilledSound->play();
    }
}

void SoundManager::onPlayPlayerKilled()
{
    if (playerKilledSound->isLoaded()) {
        playerKilledSound->play();
    }
}

void SoundManager::onPlayUfo()
{
    if (ufoSound->isLoaded() && !ufoSound->isPlaying()) {
        ufoSound->play();
    }
}

void SoundManager::onPlayInvaderMove1()
{
    if (invaderMove1Sound->isLoaded()) {
        invaderMove1Sound->play();
    }
}

void SoundManager::onPlayInvaderMove2()
{
    if (invaderMove2Sound->isLoaded()) {
        invaderMove2Sound->play();
    }
}

void SoundManager::onPlayInvaderMove3()
{
    if (invaderMove3Sound->isLoaded()) {
        invaderMove3Sound->play();
    }
}

void SoundManager::onPlayInvaderMove4()
{
    if (invaderMove4Sound->isLoaded()) {
        invaderMove4Sound->play();
    }
}

void SoundManager::onStopUfo()
{
    ufoSound->stop();
}
