#include "audio.h"
#define TICK_RATE 1000

Audio::Audio() :
    m_connected(false)
{
    connect(&m_ticker, &QTimer::timeout,
        [=](){
        if (m_queue.empty() == false && m_driver.is_ready()) {
            m_driver.stream_to_speaker(m_queue.front());
            m_queue.erase(m_queue.begin());
        }
        if (m_driver.is_connected() && !m_connected) {
            m_connected = true;
            emit audio_connected(m_connected);
        } else if (!m_driver.is_connected() && m_connected) {
            m_queue.clear();
            m_driver.stop_stream();
            m_connected = false;
            emit audio_connected(m_connected);
        }
    });
    m_ticker.setInterval(TICK_RATE);
    m_ticker.start();
}

Audio::~Audio()
{
    m_ticker.stop();
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
