#include "audio.h"
#include <QObject>
#include <QDebug>

Audio::Audio()
{
    bluetooth.start();
}

Audio::~Audio()
{
    //wait for ready
    //bluetooth.stop();
}

void Audio::play(QString file_name)
{
    while(bluetooth.is_ready() == false);
    bluetooth.play(file_name);
}
