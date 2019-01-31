#include "jblgo2driver.h"
#include <QDebug>
#include <QTime>

#define SPEAKER_TIMEOUT 600000
#define ATTEMPT_CONNECTION_TIMEOUT 6000

JBLGO2driver::JBLGO2driver() :
    connected(false)
{
    m_player.setWorkingDirectory("/home/pi/Music");
//    connect(m_player, &QProcess::readyReadStandardError,
//        [=](){
//        auto time_stamp = QTime::currentTime().toString("HH:mm");
//        QByteArray stream_output = m_player->readAllStandardError();
//        auto lines = QString(stream_output).split("\n");
//        for (auto& line: lines){
//            if (!line.trimmed().isEmpty()) {
//                qDebug() << time_stamp << "Stream feedback:\t" << line;
//            }
//        }
//    });
    connect(&m_player, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int exit_code, QProcess::ExitStatus exit_status){
        if (exit_code == 0) {
            connected = true;
            m_speaker_inactivity.start(SPEAKER_TIMEOUT);
        } else {
            connected = false;
            m_speaker_inactivity.start(ATTEMPT_CONNECTION_TIMEOUT);
        }
        if (exit_status != QProcess::ExitStatus(QProcess::NormalExit)) {
            auto time_stamp = QTime::currentTime().toString("HH:mm");
            qDebug() << time_stamp << "Stream exit error:\t" << exit_status;
        }
    });
    connect(&m_speaker_inactivity, &QTimer::timeout,
        [=](){
        auto time_stamp = QTime::currentTime().toString("HH:mm");
        if (JBLGO2driver::is_ready()) {
            stream_to_speaker("blank.wav");
            qDebug() << time_stamp << "Ping speaker";
        } else {
            qDebug() << time_stamp << "Exceptional case: Inactivity timer triggered while audio was playing";
        }
    });
    m_speaker_inactivity.start(ATTEMPT_CONNECTION_TIMEOUT);
}

JBLGO2driver::~JBLGO2driver()
{
    m_speaker_inactivity.stop();
    m_player.kill();
    m_player.close();
}

void JBLGO2driver::stream_to_speaker(QString file_name)
{
    m_speaker_inactivity.stop();
    m_player.waitForFinished();

    QString command = "aplay " + file_name;
    m_player.start(command);
}

void JBLGO2driver::stop_stream()
{
    if (JBLGO2driver::is_ready() == false) {
        m_player.terminate();
    }
}

bool JBLGO2driver::is_ready() const
{
    return (m_player.state() == QProcess::ProcessState(QProcess::NotRunning));
}

bool JBLGO2driver::is_connected() const
{
    return connected;
}
