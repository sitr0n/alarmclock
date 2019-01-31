#ifndef ALARMCLOCK_H
#define ALARMCLOCK_H
#include "philipshuedriver.h"
#include "audio.h"
#include <QObject>
#include <QProcess>
#include <QTimer>

class AlarmClock : public QObject
{
Q_OBJECT

public:
    AlarmClock();
    ~AlarmClock();
    void setAlarm(int hour, int minute);
    void stopAlarm();
    void stop_audio();

    void setLights(bool);
    void set_brightness(int percent);

signals:
    void audio_connected(bool connected);

private:
    void alarmEvent();

    int m_hour, m_minute;
    bool m_alarmHandled;
    PhilipsHUEdriver m_lights;
    Audio *m_audio;
    QTimer *m_ticker;
    bool m_connected;
};

#endif // ALARMCLOCK_H
