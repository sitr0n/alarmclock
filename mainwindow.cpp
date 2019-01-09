#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_alarm(new AlarmClock),
    m_alarmTimer(new QTimer(this))
{
    ui->setupUi(this);
    ui->lcdNumber->display("--:--");
    connect(m_alarmTimer, SIGNAL(timeout()), m_alarm, SLOT(update()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_alarm;
    delete m_alarmTimer;
}

void MainWindow::on_btnSetAlarm_clicked()
{
    auto alarmTime = ui->timeEdit->time();
    auto hour = alarmTime.hour();
    auto minute = alarmTime.minute();
    m_alarm->setAlarm(hour, minute);
    m_alarmTimer->start();
    auto hourString = QString::number(hour);
    if (hour < 10) {
        hourString = "0" + hourString;
    }
    auto minuteString = QString::number(minute);
    if (minute < 10) {
        minuteString = "0" + minuteString;
    }
    QString alarmString = hourString + ":" + minuteString;
    ui->lcdNumber->display(alarmString);
}

void MainWindow::on_btnOff_clicked()
{
    m_alarmTimer->stop();
    ui->lcdNumber->display("--:--");
}

void MainWindow::on_pushButton_clicked()
{
    m_alarm->setLights(true);
}

void MainWindow::on_pushButton_2_clicked()
{
    m_alarm->setLights(false);
}
