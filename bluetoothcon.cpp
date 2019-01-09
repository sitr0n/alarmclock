#include "bluetoothcon.h"
#include <QProcess>
#include <QDebug>

bool Bluetoothcon::is_ready()
{
    return m_is_ready;
}

void Bluetoothcon::play(QString file_name)
{
    while(m_is_ready == false);
    m_is_ready = false;
    qDebug() << "playing song\n";
    QString command = "aplay " + file_name;
    QProcess player;
    player.setWorkingDirectory("/home/pi/Music");
    player.start(command);
    QThread::sleep(7*60 + 12);
    player.close();
    //player.wait();
    m_is_ready = true;
}

void Bluetoothcon::run()
{
    qDebug() << "enter run";
    while(is_running == true)
    {
        if(m_is_ready == true) {
            m_is_ready = false; // replace with mutex
            qDebug() << "ping speaker\n";
            QString blank_file = "blank.wav";
            QString ping_bluetooth = "aplay " + blank_file;
            QProcess player;
            player.setWorkingDirectory("/home/pi/Music");
            player.start(ping_bluetooth);
            QThread::sleep(5);
            player.close();
            m_is_ready = true;
        }
        QThread::sleep(600);
    }
}
