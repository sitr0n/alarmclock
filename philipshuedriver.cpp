#include "philipshuedriver.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QByteArray>

#define MAX_BRI 254

PhilipsHUEdriver::PhilipsHUEdriver() :
    m_manager   (new QNetworkAccessManager(this)),
    m_ip        ("192.168.1.159"),
    m_username  ("YK7nzmX6qDOvOh2TigfYgaFxn1H-bdVTPWY2Yszq")
{
    connect(m_manager, &QNetworkAccessManager::finished, this,
        [=](QNetworkReply *reply) {
        reply->deleteLater();

        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (statusCode != 200) {
            qDebug() << "Http status code from Hue:" << statusCode;
        }

        QByteArray bytes = reply->readAll();
        QJsonObject json = QJsonDocument::fromJson(bytes).array()[0].toObject();

        auto keyList = json.keys();
        for (auto key: keyList) {
            if (key == "error" || key == "success") {
                auto value = json[key];
                QJsonDocument doc(value.toObject());
                QString str(doc.toJson(QJsonDocument::Compact));
                qDebug() << str;
            }
        }
    });
}

PhilipsHUEdriver::~PhilipsHUEdriver()
{
    delete m_manager;
}

void PhilipsHUEdriver::change_settings(const QJsonObject state)
{
    QNetworkRequest request;
    request.setUrl(QUrl("http://" + m_ip + "/api/" + m_username + "/lights/4/state"));
    QJsonDocument capsule(state);
    m_manager->put(request, capsule.toJson());
}

void PhilipsHUEdriver::set_brightness(int percent)
{
    if (percent > 100)
        percent = 100;
    if (percent < 0)
        percent = 0;
    if (percent != m_prev_bri) {
        QJsonObject state;
        state["bri"] = (int) (MAX_BRI*percent/100);
        state["on"] = (percent > 0) ? true : false;
        change_settings(state);
        m_prev_bri = percent;
    }
}

void PhilipsHUEdriver::set_colormode(ColorMode mode)
{
    QJsonObject state;
    switch (mode) {
    case ColorMode::SUNLIGHT :
        state["xy"] = QJsonArray({0.4578f, 0.412f});
    break;
    case ColorMode::BLUE :
        state["xy"] = QJsonArray({0.1393f, 0.0813f});
    break;
    case ColorMode::RED :
        state["xy"] = QJsonArray({0.6284f, 0.3386f});
    break;
    }
    change_settings(state);
}

void PhilipsHUEdriver::request_settings()
{
    QNetworkRequest request;
    request.setUrl(QUrl("http://" + m_ip + "/api/" + m_username));
    m_manager->get(request);
}

void PhilipsHUEdriver::pair()
{
    QNetworkRequest request;
    request.setUrl(QUrl("http://" + m_ip + "/api/"));

    QJsonObject json;
    json.insert("devicetype", "my_hue_app");

    QJsonDocument doc(json);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    QByteArray body = strJson.toUtf8();
    m_manager->post(request, body);
}
