#ifndef BLUETOOTHCON_H
#define BLUETOOTHCON_H

#include <QObject>
#include <QThread>
#include <QMutex>

class Bluetoothcon : public QThread
{
    Q_OBJECT

public:
    bool is_ready();
    void play(QString file_name);

protected:
   virtual void run();

private:
    bool is_running;
    bool m_is_ready;
};

#endif // BLUETOOTHCON_H
