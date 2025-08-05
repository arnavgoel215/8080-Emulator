#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>

#include <memory>
#include <QMediaPlayer>
#include <QAudioOutput>

class SoundManager : public QObject
{
    Q_OBJECT
public:
    explicit SoundManager(QObject *parent = nullptr);
    ~SoundManager();

    void playPlayerShoot();
    void playInvaderKilled();
    void playPlayerKilled() ;
    void playUfo();
    void playInvaderMove1();
    void playInvaderMove2();
    void playInvaderMove3();
    void playInvaderMove4();
    void stopUfo();

private:
    void loadSounds();

    std::unique_ptr<QMediaPlayer> playerShootPlayer;
    std::unique_ptr<QAudioOutput> playerShootOutput;
    std::unique_ptr<QMediaPlayer> invaderKilledPlayer;
    std::unique_ptr<QAudioOutput> invaderKilledOutput;
    std::unique_ptr<QMediaPlayer> playerKilledPlayer;
    std::unique_ptr<QAudioOutput> playerKilledOutput;
    std::unique_ptr<QMediaPlayer> ufoPlayer;
    std::unique_ptr<QAudioOutput> ufoOutput;
    std::unique_ptr<QMediaPlayer> invaderMove1Player;
    std::unique_ptr<QAudioOutput> invaderMove1Output;
    std::unique_ptr<QMediaPlayer> invaderMove2Player;
    std::unique_ptr<QAudioOutput> invaderMove2Output;
    std::unique_ptr<QMediaPlayer> invaderMove3Player;
    std::unique_ptr<QAudioOutput> invaderMove3Output;
    std::unique_ptr<QMediaPlayer> invaderMove4Player;
    std::unique_ptr<QAudioOutput> invaderMove4Output;
};

#endif // SOUNDMANAGER_H
