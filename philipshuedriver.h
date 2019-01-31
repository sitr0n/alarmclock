#ifndef PHILIPSHUEDRIVER_H
#define PHILIPSHUEDRIVER_H
#include <QObject>

#include <QNetworkAccessManager>

enum class ColorMode { SUNLIGHT, BLUE, RED };

class PhilipsHUEdriver : public QObject
{
Q_OBJECT
public:
    PhilipsHUEdriver();
    ~PhilipsHUEdriver() = default;
    void set_brightness(int percent);
    void set_colormode(ColorMode mode);
    void pair();

private:
    void request_settings();
    void change_settings(const QJsonObject state);

    QNetworkAccessManager m_manager;
    QString m_ip;
    QString m_username;
    int m_prev_bri;
};

#endif // PHILIPSHUEDRIVER_H
