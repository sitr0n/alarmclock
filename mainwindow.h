#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "alarmclock.h"
#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT
public slots:

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnSetAlarm_clicked();
    void on_btnOff_clicked();

private:
    Ui::MainWindow *ui;
    AlarmClock *m_alarm;
    QTimer *m_alarmTimer;
};

#endif // MAINWINDOW_H
