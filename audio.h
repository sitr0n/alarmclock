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

private:
    QVector<QString> m_queue;
    QTimer* m_ticker;
    JBLGO2driver m_driver;
};

#endif // AUDIO_H
