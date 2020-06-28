#ifndef WEATHER_H
#define WEATHER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QDebug>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QGeoCoordinate>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPixmap>


class Weather : public QObject
{
    Q_OBJECT
public:
    explicit Weather(QObject *parent = nullptr);

    void getLocationData();
    int convertToCels(double kelvin);

    QString getCityName() const;
    QString getCountryName() const;
    int getTemperature() const;
    QString getDescription() const;
    QPixmap getIcon() const;
    QString getIconName() const;

signals:
    void readyToGetWeatherInfo();
    void iconIdReady();
    //void iconDataDownloaded();
public slots:  
    void positionUpdated(const QGeoPositionInfo &info);
    void getWeatherData();
    void weatherDataFinish(QNetworkReply *reply);
    //void getWeatherIcon();
    //void iconDownload(QNetworkReply* pReply);

private:
   QNetworkAccessManager m_manager;
   QString m_cityName;
   QString m_countryName;
   int m_temperature;
   QString m_description;
   QString m_iconName;
   QByteArray m_iconData;
   QPixmap m_icon;

   double m_lat;
   double m_lon;

};

#endif // WEATHER_H
