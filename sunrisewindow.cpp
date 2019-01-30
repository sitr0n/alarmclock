#include "sunrisewindow.h"
#include "ui_sunrisewindow.h"
#include <QSettings>
#include <QDir>
#include <QGraphicsPixmapItem>


SunriseWindow::SunriseWindow(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::SunriseWindow),
    alarm(new AlarmClock())
{
    ui->setupUi(this);
    ui->alarm_display->display("--:--");

    QString file_name = QDir::currentPath();
    file_name += "/c++/AlarmClockApp/resources";
    file_name += "/color_wheel_rect.png";
    qDebug() << file_name;
    QImage image(file_name);
    if(image.isNull()) {
        qDebug() << "\nBad Image Path\n";
    }

    QGraphicsScene* scene = new QGraphicsScene();
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(item);
    ui->lights_color_palette->setScene(scene);
    ui->lights_color_palette->show();
}

SunriseWindow::~SunriseWindow()
{
    delete ui;
    delete alarm;
}

void SunriseWindow::on_alarm_start_button_clicked()
{
    QSettings settings;
    auto alarm_hour = settings.value("Alarm/hour", 0).toInt();
    auto alarm_minute = settings.value("Alarm/minute", 0).toInt();

    alarm->setAlarm(alarm_hour, alarm_minute);

    QString alarm_str = QString::number(alarm_hour) + ":";
    if(alarm_minute < 10) {
        alarm_str += "0";
    }
    alarm_str += QString::number(alarm_minute);
    ui->alarm_display->display(alarm_str);
    ui->alarm_stop_button->setText("Stop");
}

void SunriseWindow::on_alarm_stop_button_clicked()
{
    auto action = ui->alarm_stop_button->text();
    if(action == "Stop") {
        alarm->stopAlarm();
        alarm->stop_audio();
        ui->alarm_display->display("--:--");
        ui->alarm_stop_button->setText("Set");
    } else {
        SettingsWindow window(this);
        window.setModal(true);
        window.exec();

        QSettings settings;
        auto alarm_hour = settings.value("Alarm/hour", 0).toInt();
        auto alarm_minute = settings.value("Alarm/minute", 0).toInt();

        QString alarm_str = QString::number(alarm_hour) + ":";
        if(alarm_minute < 10) {
            alarm_str += "0";
        }
        alarm_str += QString::number(alarm_minute);
        ui->alarm_display->display(alarm_str);
    }
}

void SunriseWindow::on_lights_on_button_clicked()
{
    alarm->setLights(true);
}

void SunriseWindow::on_lights_off_button_clicked()
{
    alarm->setLights(false);
}

void SunriseWindow::on_lights_brightness_slider_sliderReleased()
{
    auto percent = ui->lights_brightness_slider->value();
    alarm->set_brightness(percent);
}
