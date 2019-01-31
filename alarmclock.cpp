#include "alarmclock.h"
#include <QTime>
#include <QDebug>
#include <QThread>
#include <QSettings>

#define TICK_RATE 1000
#define TRIGGER_TIMEOUT 300
#define PRE_ALARM_DIM 1800

AlarmClock::AlarmClock() :
    m_hour(0),
    m_minute(0),
    m_alarmHandled(false),
    m_ticker(new QTimer(this)),
    m_audio(new Audio()),
    m_connected(true)
{
    m_lights.set_colormode(ColorMode::SUNLIGHT);
    connect(m_ticker, &QTimer::timeout,
    [=](){
        auto time = QTime::currentTime();
        QTime alarm(m_hour, m_minute);
        if(time.hour() == alarm.hour() && time.minute() == alarm.minute() && m_alarmHandled == false) {
            alarmEvent();
            m_alarmHandled = true;
        } else if(m_alarmHandled == true && time >= alarm.addSecs(TRIGGER_TIMEOUT)) {
            m_alarmHandled = false;
        }
        if(time >= alarm.addSecs(-PRE_ALARM_DIM) && time <= alarm) {
            int minDifference = time.minute() - alarm.addSecs(-PRE_ALARM_DIM).minute();
            if (minDifference < 0) minDifference += 60;
            int secondsElapsed = minDifference * 60;
            int bri = (int) secondsElapsed*100 / PRE_ALARM_DIM;
            m_lights.set_brightness(bri);
        }
    });
    m_ticker->setInterval(TICK_RATE);

    connect(m_audio, &Audio::audio_connected, this,
        [=](bool connection){
        if (connection && !m_connected) {
            m_connected = true;
            emit audio_connected(m_connected);
        } else if (!connection && m_connected) {
            m_connected = false;
            emit audio_connected(m_connected);
        }

        auto time_stamp = QTime::currentTime().toString("HH:mm");

        qDebug() << time_stamp << "In alarm clock:: " << connection;
    });
}

AlarmClock::~AlarmClock()
{
    delete m_ticker;
    delete m_audio;
}

void AlarmClock::setLights(bool on)
{
    if (on == true) {
        m_lights.set_brightness(100);
    } else {
        m_lights.set_brightness(0);
    }
}

void AlarmClock::set_brightness(int percent)
{
    m_lights.set_brightness(percent);
}

void AlarmClock::stop_audio()
{
    m_audio->stop();
}

void AlarmClock::setAlarm(int hour, int minute)
{
    m_hour = hour;
    m_minute = minute;
    m_alarmHandled = false;
    m_ticker->start();
}

void AlarmClock::stopAlarm()
{
    m_ticker->stop();
}

void AlarmClock::alarmEvent()
{
    QSettings settings;
    qDebug() << settings.value("Audio/alarmSong").toString();

    m_audio->queue("ShikiNoUta.wav");
    m_audio->queue("medicine.wav");
    m_audio->queue("weback.wav");

}
