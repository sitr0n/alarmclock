#ifndef SUNRISEWINDOW_H
#define SUNRISEWINDOW_H

#include <QTabWidget>
#include "alarmclock.h"
#include "settingswindow.h"

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
    void on_alarm_stop_button_clicked();

    void on_lights_on_button_clicked();
    void on_lights_off_button_clicked();
    void on_lights_brightness_slider_sliderReleased();

private:
    Ui::SunriseWindow *ui;
    AlarmClock* alarm;
};

#endif // SUNRISEWINDOW_H
