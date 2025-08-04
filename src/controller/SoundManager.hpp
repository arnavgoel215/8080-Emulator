#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QSoundEffect>
#include <memory>

class SoundManager : public QObject
{
    Q_OBJECT
public:
    explicit SoundManager(QObject *parent = nullptr);
    ~SoundManager();

signals:
    void requestPlayPlayerShoot();
    void requestPlayInvaderKilled();
    void requestPlayPlayerKilled();
    void requestPlayUfo();
    void requestPlayInvaderMove1();
    void requestPlayInvaderMove2();
    void requestPlayInvaderMove3();
    void requestPlayInvaderMove4();
    void requestStopUfo();

public slots:
    void playPlayerShoot() { emit requestPlayPlayerShoot(); }
    void playInvaderKilled() { emit requestPlayInvaderKilled(); }
    void playPlayerKilled() { emit requestPlayPlayerKilled(); }
    void playUfo() { emit requestPlayUfo(); }
    void playInvaderMove1() { emit requestPlayInvaderMove1(); }
    void playInvaderMove2() { emit requestPlayInvaderMove2(); }
    void playInvaderMove3() { emit requestPlayInvaderMove3(); }
    void playInvaderMove4() { emit requestPlayInvaderMove4(); }
    void stopUfo() { emit requestStopUfo(); }

private slots:
    void onPlayPlayerShoot();
    void onPlayInvaderKilled();
    void onPlayPlayerKilled();
    void onPlayUfo();
    void onPlayInvaderMove1();
    void onPlayInvaderMove2();
    void onPlayInvaderMove3();
    void onPlayInvaderMove4();
    void onStopUfo();

private:
    void loadSounds();

    std::unique_ptr<QSoundEffect> playerShootSound;
    std::unique_ptr<QSoundEffect> invaderKilledSound;
    std::unique_ptr<QSoundEffect> playerKilledSound;
    std::unique_ptr<QSoundEffect> ufoSound;
    std::unique_ptr<QSoundEffect> invaderMove1Sound;
    std::unique_ptr<QSoundEffect> invaderMove2Sound;
    std::unique_ptr<QSoundEffect> invaderMove3Sound;
    std::unique_ptr<QSoundEffect> invaderMove4Sound;
};

#endif // SOUNDMANAGER_H
