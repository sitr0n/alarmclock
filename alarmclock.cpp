#include "alarmclock.h"
#include <QTime>
#include <QDebug>
#include <QThread>

#define TRIGGER_TIMEOUT 300
#define PRE_ALARM_DIM 1800

AlarmClock::AlarmClock() :
    m_hour(0),
    m_minute(0),
    m_alarmHandled(false)
{
    m_lights.setColormode(ColorMode::SUNLIGHT);
}

AlarmClock::AlarmClock(int hour, int minute) :
    m_hour(hour),
    m_minute(minute),
    m_alarmHandled(false)
{
    m_lights.setColormode(ColorMode::SUNLIGHT);
}

void AlarmClock::setLights(bool on)
{
    if (on == true) {
        m_lights.setBrightness(1);
    } else {
        m_lights.setBrightness(0);
    }
}

void AlarmClock::setAlarm(int hour, int minute)
{
    m_hour = hour;
    m_minute = minute;
    m_alarmHandled = false;
}

void AlarmClock::update()
{
    tick();
}

void AlarmClock::tick()
{
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
        double bri = (double) secondsElapsed / PRE_ALARM_DIM;
        m_lights.setBrightness(bri);
    }
}

void AlarmClock::alarmEvent()
{
    m_audio.play("ShikiNoUta.wav");
}
