#ifndef SUNRISEWINDOW_H
#define SUNRISEWINDOW_H
#include <QTabWidget>

#include "settingswindow.h"
#include "philipshuedriver.h"
#include "audio.h"
#include <QObject>
#include <QProcess>
#include <QTimer>

namespace Ui {
class SunriseWindow;
}

class SunriseWindow : public QTabWidget
{
    Q_OBJECT

public:
    explicit SunriseWindow(QWidget *parent = 0);
    ~SunriseWindow();

private slots:
    void on_alarm_start_button_clicked();
    void on_alarm_set_button_clicked();

    void on_lights_on_button_clicked();
    void on_lights_off_button_clicked();
    void on_lights_brightness_slider_sliderReleased();

private:
    void setup_color_palette();
    void display_started_state();
    void display_ready_state();
    void display_waiting_state();

    void alarm_event();
    void set_alarm(int hour, int minute);

    Ui::SunriseWindow *ui;
    PhilipsHUEdriver m_lights;
    Audio m_audio;
    QTimer m_alarm_ticker;
    int m_alarm_hour, m_alarm_minute;
    bool m_alarm_handled;
};

#endif // SUNRISEWINDOW_H
