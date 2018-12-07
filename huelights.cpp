#include "huelights.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QStringList>
#include <QUrl>

#define MAX_BRI 254

HueLights::HueLights() :
    m_manager   (new QNetworkAccessManager(this)),
    m_ip        ("192.168.1.159"),
    m_username  ("YK7nzmX6qDOvOh2TigfYgaFxn1H-bdVTPWY2Yszq")
{
    connect(m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

HueLights::~HueLights()
{
    delete m_manager;
}

void HueLights::updateServer()
{
    QString url = "http://" + m_ip + "/api/" + m_username + "/lights/4/state";
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    auto stateCapsule = m_lightState.toBytes();
    m_manager->put(request, stateCapsule);
}

void HueLights::setBrightness(double percent)
{
    auto prevState = m_lightState;
    if (percent > 0) {
        if (percent > 1) {
            percent = 1;
        }
        if (percent < 0) {
            percent = 0;
        }
        m_lightState.on = true;
        m_lightState.bri = (int) (MAX_BRI*percent);
    } else {
        m_lightState.on = false;
        m_lightState.bri = 0;
    }
    if (m_lightState.bri != prevState.bri) {
        updateServer();
    }
}

void HueLights::setColormode(ColorMode mode)
{
    switch (mode) {
    case ColorMode::SUNLIGHT :
        m_lightState.xy[0] = 0.4578f;
        m_lightState.xy[1] = 0.412f;
    break;
    case ColorMode::BLUE :
        m_lightState.xy[0] = 0.1393f;
        m_lightState.xy[1] = 0.0813f;
    break;
    case ColorMode::RED :
        m_lightState.xy[0] = 0.6284f;
        m_lightState.xy[1] = 0.3386f;
    break;
    }
    updateServer();
}

void HueLights::getStateFromServer()
{
    QString url = "http://" + m_ip + "/api/" + m_username;
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    m_manager->get(request);
}

QJsonObject HueLights::selectFromKey(QJsonObject json, QString selector)
{
    auto keyList = json.keys();
    for (auto key: keyList) {
        if (key == selector) {
            auto value = json[key];
            return value.toObject();
        }
    }
    QJsonObject empty;
    return empty;
}

State HueLights::parseServerData(QJsonObject blob)
{
    auto lights = selectFromKey(blob, "lights");
    auto unit = selectFromKey(lights, "4");
    auto state = selectFromKey(unit, "state");

    State lightState(state);
    return lightState;
}

void HueLights::replyFinished(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "\n" << QString::number(statusCode);

    QByteArray replyBytes = reply->readAll();
    QJsonDocument replyDoc = QJsonDocument::fromJson(replyBytes);
    QString replyString = QString::fromUtf8(replyBytes.data(), replyBytes.size());
    qDebug() << replyString << "\n";

    QJsonObject jsonRoot = replyDoc.object();
    //m_lightState = parseServerData(jsonRoot);
}

void HueLights::pair()
{
    QString url = "http://" + m_ip + "/api/";

    QNetworkRequest request;
    request.setUrl(QUrl(url));

    QJsonObject json
    {
        {"devicetype", "my_hue_app"}
    };
    QJsonDocument doc(json);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    QByteArray body = strJson.toUtf8();
    m_manager->post(request, body);
}
