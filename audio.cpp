#include "audio.h"
#define TICK_PERIOD 1000

Audio::Audio() :
    m_ticker(new QTimer(this))
{
    connect(m_ticker, &QTimer::timeout,
        [=](){
        if (m_queue.empty() == false && m_driver.is_ready()) {
            m_driver.stream_to_speaker(m_queue.front());
            m_queue.erase(m_queue.begin());
        }
    });
    m_ticker->setInterval(TICK_PERIOD);
    m_ticker->start();
}

Audio::~Audio()
{
    m_ticker->stop();
    delete m_ticker;
}

void Audio::play(QString file_name)
{
    m_driver.stop_stream();
    m_driver.stream_to_speaker(file_name);
}

void Audio::queue(QString file_name)
{
    m_queue.push_back(file_name);
}

void Audio::skip()
{
    m_driver.stop_stream();
}

void Audio::stop()
{
    m_queue.clear();
    m_driver.stop_stream();
}
