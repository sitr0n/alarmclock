#ifndef AUDIO_H
#define AUDIO_H
#include <QObject>

#include <QVector>
#include <QTimer>
#include <jblgo2driver.h>

class Audio : public QObject
{
    Q_OBJECT
public:
    Audio();
    ~Audio();
    void play(QString file_name);
    void queue(QString file_name);
    void stop();
    void skip();

signals:
    void audio_connected(bool connection);

private:
    QVector<QString> m_queue;
    QTimer m_ticker;
    JBLGO2driver m_driver;
    bool m_connected;
};

#endif // AUDIO_H
