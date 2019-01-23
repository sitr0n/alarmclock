#include "jblgo2driver.h"
#include <QDebug>
#include <QTime>

#define SPEAKER_TIMEOUT 600000

JBLGO2driver::JBLGO2driver() :
    m_player(new QProcess()),
    m_speaker_inactivity(new QTimer(this))
{
    m_player->setWorkingDirectory("/home/pi/Music");
    connect(m_player, &QProcess::readyReadStandardError,
        [=](){
        auto time_stamp = QTime::currentTime().toString("HH:mm");
        QByteArray stream_output = m_player->readAllStandardError();
        auto lines = QString(stream_output).split("\n");
        for (auto& line: lines){
            if (line != "")
                qDebug() << time_stamp << "Stream feedback:\t" << line;
        }
    });
    connect(m_player, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        [=](int exit_code, QProcess::ExitStatus exit_status){
        if (exit_status == QProcess::ExitStatus(QProcess::NormalExit)) {
            m_speaker_inactivity->start();
        } else {
            auto time_stamp = QTime::currentTime().toString("HH:mm");
            qDebug() << time_stamp << "Stream exit status:\t" << exit_status << "\nInactivity timer will no longer keep speaker alive.";
        }
        if (exit_code != 0) {
            auto time_stamp = QTime::currentTime().toString("HH:mm");
            qDebug() << time_stamp << "Streaming error code:\t" << exit_code;
        }
    });
    connect(m_speaker_inactivity, &QTimer::timeout,
        [=](){
        auto time_stamp = QTime::currentTime().toString("HH:mm");
        if (JBLGO2driver::is_ready()) {
            stream_to_speaker("blank.wav");
            qDebug() << time_stamp << "Ping speaker";
        } else {
            qDebug() << time_stamp << "Exceptional case: Inactivity timer triggered while audio was playing";
        }
    });
    m_speaker_inactivity->setInterval(SPEAKER_TIMEOUT);
    m_speaker_inactivity->start();
}

JBLGO2driver::~JBLGO2driver()
{
    m_speaker_inactivity->stop();
    delete m_speaker_inactivity;

    m_player->kill();
    m_player->close();
    delete m_player;

    qDebug() << "Driver closed";
}

void JBLGO2driver::stream_to_speaker(QString file_name)
{
    m_speaker_inactivity->stop();
    m_player->waitForFinished();

    QString command = "aplay " + file_name;
    m_player->start(command);
}

void JBLGO2driver::stop_stream()
{
    if (JBLGO2driver::is_ready() == false) {
        m_player->terminate();
    }
}

bool JBLGO2driver::is_ready()
{
    if (m_player->state() == QProcess::ProcessState(QProcess::NotRunning)) {
        return true;
    } else {
        return false;
    }
}
