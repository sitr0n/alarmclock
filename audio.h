#ifndef AUDIO_H
#define AUDIO_H
#include "bluetoothcon.h"

#include <QProcess>
#include <QString>
#include <QTime>
#include <QThread>

#include <QObject>

class Audio
{
    //Q_OBJECT
public:
    Audio();
    ~Audio();
    void play(QString file_name);

private:
    Bluetoothcon bluetooth;
};

#endif // AUDIO_H
