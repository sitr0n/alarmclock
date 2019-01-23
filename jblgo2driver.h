#ifndef JBLGO2DRIVER_H
#define JBLGO2DRIVER_H
#include <QObject>

#include <QProcess>
#include <QTimer>

class JBLGO2driver : public QObject
{
    Q_OBJECT
public:
    JBLGO2driver();
    ~JBLGO2driver();
    void stream_to_speaker(QString file_name);
    void stop_stream();
    bool is_ready();

private:
    QProcess* m_player;
    QTimer* m_speaker_inactivity;

};

#endif // JBLGO2DRIVER_H
