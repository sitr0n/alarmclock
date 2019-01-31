#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QProcess>
#include <QTime>
#include "philipshuedriver.h"
#include "audio.h"
#include <QObject>
#include <QProcess>
#include <QTimer>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();


private slots:
    void on_buttonBox_accepted();
    void on_search_path_button_clicked();

private:
    Ui::SettingsWindow *ui;
    QProcess file_finder;
};

#endif // SETTINGSWINDOW_H
