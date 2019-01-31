#include "sunrisewindow.h"
#include "ui_sunrisewindow.h"
#include <QSettings>
#include <QDir>
#include <QGraphicsPixmapItem>

#include <QTime>
#include <QDebug>
#include <QThread>

#define TICK_RATE 1000
#define TRIGGER_TIMEOUT 300
#define PRE_ALARM_DIM 1800


SunriseWindow::SunriseWindow(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::SunriseWindow),
    m_alarm_hour(0),
    m_alarm_minute(0),
    m_alarm_handled(false)
{
    ui->setupUi(this);
    setup_color_palette();
    display_waiting_state();
    m_lights.set_colormode(ColorMode::SUNLIGHT);

    connect(&m_alarm_ticker, &QTimer::timeout,
        [=](){
        auto time = QTime::currentTime();
        QTime alarm(m_alarm_hour, m_alarm_minute);
        if (time.hour() == alarm.hour() && time.minute() == alarm.minute() && m_alarm_handled == false) {
            alarm_event();
            m_alarm_handled = true;
        } else if (m_alarm_handled == true && time >= alarm.addSecs(TRIGGER_TIMEOUT)) {
            m_alarm_handled = false;
        }
        if(time >= alarm.addSecs(-PRE_ALARM_DIM) && time <= alarm) {
            int minDifference = time.minute() - alarm.addSecs(-PRE_ALARM_DIM).minute();
            if (minDifference < 0) {
                minDifference += 60;
            }
            int secondsElapsed = minDifference * 60;
            int bri = (int) secondsElapsed*100 / PRE_ALARM_DIM;
            m_lights.set_brightness(bri);
        }
    });
    m_alarm_ticker.setInterval(TICK_RATE);

    connect(&m_audio, &Audio::audio_connected, this,
        [=](bool connection){
        auto time_stamp = QTime::currentTime().toString("HH:mm");
        if (connection) {
            display_ready_state();
            qDebug() << time_stamp << "Audio connected";
        } else {
            display_waiting_state();
            qDebug() << time_stamp << "Audio disconnected";
        }
    });
}

SunriseWindow::~SunriseWindow()
{
    delete ui;
}

void SunriseWindow::on_alarm_start_button_clicked()
{
    auto action = ui->alarm_start_button->text();
    if(action == "Start") {
        m_alarm_handled = false;
        m_alarm_ticker.start();

        ui->alarm_start_button->setText("Stop");
        display_started_state();
    } else {
        m_alarm_ticker.stop();
        m_audio.stop();

        ui->alarm_start_button->setText("Start");
        display_ready_state();
    }
}

void SunriseWindow::on_alarm_set_button_clicked()
{
    SettingsWindow window(this);
    window.setModal(true);
    window.exec();

    QSettings settings;
    m_alarm_hour = settings.value("Alarm/hour", 0).toInt();
    m_alarm_minute = settings.value("Alarm/minute", 0).toInt();
    display_ready_state();
}


void SunriseWindow::on_lights_on_button_clicked()
{
    m_lights.set_brightness(100);
}

void SunriseWindow::on_lights_off_button_clicked()
{
    m_lights.set_brightness(0);
}

void SunriseWindow::on_lights_brightness_slider_sliderReleased()
{
    auto percent = ui->lights_brightness_slider->value();
    m_lights.set_brightness(percent);
}

void SunriseWindow::setup_color_palette()
{
    QString file_name = QDir::currentPath();
    file_name += "/c++/AlarmClockApp/resources";
    file_name += "/color_wheel_rect.png";
    QImage image(file_name);
    if(!image.isNull()) {
        QGraphicsScene* scene = new QGraphicsScene();
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        scene->addItem(item);
        ui->lights_color_palette->setScene(scene);
        ui->lights_color_palette->show();
    } else {
        qDebug() << "Error while loading lights color wheel.";
    }
}

void SunriseWindow::display_started_state()
{
    ui->alarm_start_button->setStyleSheet("color: black");
}

void SunriseWindow::display_ready_state()
{
    QSettings settings;
    auto alarm_hour = settings.value("Alarm/hour", 0).toInt();
    auto alarm_minute = settings.value("Alarm/minute", 0).toInt();

    QString alarm_str;
    if(alarm_hour < 10) {
        alarm_str += "0";
    }
    alarm_str += QString::number(alarm_hour) + ":";
    if(alarm_minute < 10) {
        alarm_str += "0";
    }
    alarm_str += QString::number(alarm_minute);

    ui->alarm_display->display(alarm_str);
    ui->alarm_start_button->setEnabled(true);
    ui->alarm_start_button->setStyleSheet("color: green");
    ui->alarm_set_button->setEnabled(true);
}

void SunriseWindow::display_waiting_state()
{
    ui->alarm_display->display("--:--");
    ui->alarm_start_button->setStyleSheet("color: gray");
    ui->alarm_start_button->setEnabled(false);
    ui->alarm_set_button->setEnabled(false);
}

void SunriseWindow::alarm_event()
{
    QSettings settings;
    qDebug() << "Alarm triggered." << settings.value("Audio/alarmSong").toString();

    m_audio.queue("ShikiNoUta.wav");
    m_audio.queue("medicine.wav");
    m_audio.queue("weback.wav");
}
