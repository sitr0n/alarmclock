#ifndef HUELIGHTS_H
#define HUELIGHTS_H

#include <QNetworkReply>
#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QByteArray>

struct State {
    bool on = false;
    int bri = 254;
    int hue = 47110;
    int sat = 253;
    QString effect = "none";
    double xy[2] = {0.4578f, 0.412f};
    int ct = 0;
    QString alert = "none";
    QString colormode = "xy";
    QString mode = "homeautomation";
    bool reachable = true;

    State() {}
    State(QJsonObject jsonState) {
        this->on = jsonState["on"].toBool();
        this->bri = jsonState["bri"].toInt();
        this->hue = jsonState["hue"].toInt();
        this->sat = jsonState["sat"].toInt();
        this->effect = jsonState["effect"].toString();
        this->xy[0] = jsonState["xy"].toArray()[0].toDouble();
        this->xy[1] = jsonState["xy"].toArray()[1].toDouble();
        this->ct = jsonState["ct"].toInt();
        this->alert = jsonState["alert"].toString();
        this->colormode = jsonState["colormode"].toString();
        this->mode = jsonState["mode"].toString();
        this->reachable = jsonState["reachable"].toBool();
    }
    QByteArray toBytes() {
        QJsonObject json;
        json["on"]          =   this->on;
        json["bri"]         =   this->bri;
        json["hue"]         =   this->hue;
        json["sat"]         =   this->sat;
        json["effect"]      =   this->effect;
        json["xy"]          =   QJsonArray({this->xy[0], this->xy[1]});
        json["ct"]          =   this->ct;
        json["alert"]       =   this->alert;
        json["colormode"]   =   this->colormode;
        json["mode"]        =   this->mode;
        json["reachable"]   =   this->reachable;
        QJsonDocument jsonDoc(json);
        return jsonDoc.toJson();
    }

};

enum class ColorMode { SUNLIGHT, BLUE, RED };

class HueLights : public QObject
{
Q_OBJECT
public slots:
    void replyFinished(QNetworkReply*);

public:
    HueLights();
    ~HueLights();
    void setBrightness(double percent);
    void setColormode(ColorMode mode);

private:
    void getStateFromServer();
    void pair();
    void updateServer();
    State parseServerData(QJsonObject blob);
    QJsonObject selectFromKey(QJsonObject json, QString key);

    QNetworkAccessManager *m_manager;
    QString m_ip;
    QString m_username;
    State m_lightState;
};

#endif // HUELIGHTS_H
