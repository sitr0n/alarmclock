#ifndef ALARMCLOCK_H
#define ALARMCLOCK_H
#include "huelights.h"
#include "audio.h"
#include <QObject>
#include <QProcess>

class AlarmClock : public QObject
{
Q_OBJECT
public slots:
    void update();

public:
    AlarmClock();
    ~AlarmClock() = default;
    AlarmClock(int hour, int minute);
    void setAlarm(int hour, int minute);
    void tick();
    void setLights(bool);

private:
    void alarmEvent();

    int m_hour, m_minute;
    bool m_alarmHandled;
    HueLights m_lights;
    Audio m_audio;
};

#endif // ALARMCLOCK_H
