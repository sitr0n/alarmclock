#include "settingswindow.h"
#include "ui_settingswindow.h"
#include <QSettings>
#include <QDebug>
#include <QTime>

#define EMPTY_COMBOBOX -1

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    connect(&file_finder, &QProcess::readyReadStandardError,
        [=](){
        auto time_stamp = QTime::currentTime().toString("HH:mm");
        QByteArray stream_output = file_finder.readAllStandardError();
        auto lines = QString(stream_output).split("\n");
        for (auto& line: lines){
            if (line != "")
                qDebug() << time_stamp << "Error while searching music folder:" << line;
        }
    });
    connect(&file_finder, &QProcess::readyReadStandardOutput,
        [=](){
        auto time_stamp = QTime::currentTime().toString("HH:mm");
        auto stream_output = file_finder.readAllStandardOutput();
        auto audio_files = QString(stream_output).simplified().split(" ");

        ui->song_chooser->clear();
        bool contains_audio_files = false;
        for (auto& file_name: audio_files){
            if (file_name.contains(".wav", Qt::CaseInsensitive) && file_name != "blank.wav") {
                ui->song_chooser->addItem(file_name);
                contains_audio_files = true;
            }
        }
        if (contains_audio_files) {
            ui->song_chooser->addItem("Surprise me!");
            QSettings settings;
            settings.setValue("Audio/path", ui->audio_path->text());
        }
    });
    QSettings settings;
    auto alarm_hour = settings.value("Alarm/hour", 0).toInt();
    auto alarm_minute = settings.value("Alarm/minute", 0).toInt();
    QTime old_alarm(alarm_hour, alarm_minute);
    ui->alarm_changer->setTime(old_alarm);

    auto audio_path = settings.value("Audio/path", "/path/to/music/folder").toString();
    ui->audio_path->setText(audio_path);
    file_finder.setWorkingDirectory(audio_path);
    file_finder.start("dir");
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_buttonBox_accepted()
{
    if (ui->song_chooser->currentIndex() != EMPTY_COMBOBOX) {
        QSettings settings;
        settings.setValue("Audio/alarmSong", ui->song_chooser->itemText(ui->song_chooser->currentIndex()));

        auto alarm_time = ui->alarm_changer->time();
        settings.setValue("Alarm/hour", alarm_time.hour());
        settings.setValue("Alarm/minute", alarm_time.minute());
    }
}

void SettingsWindow::on_search_path_button_clicked()
{
    file_finder.setWorkingDirectory(ui->audio_path->text());
    file_finder.start("dir");
}
