#include "weather.h"

Weather::Weather(QObject *parent) : QObject(parent)
{
    connect(this,&Weather::readyToGetWeatherInfo,this,&Weather::getWeatherData);
}


void Weather::getLocationData()
{
    // Get the location information
    QGeoPositionInfoSource *source = QGeoPositionInfoSource::createDefaultSource(this);
   if (source) {
       connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)),
               this, SLOT(positionUpdated(QGeoPositionInfo)));

       source->setUpdateInterval(1000000);
       source->startUpdates();

   }
}

void Weather::positionUpdated(const QGeoPositionInfo &info)
{
    QGeoCoordinate geoCoord = info.coordinate();
    m_lat = geoCoord.latitude();
    m_lon = geoCoord.longitude();
    emit readyToGetWeatherInfo();
}

int Weather::convertToCels(double kelvin)
{
    return qRound(kelvin-273.15);
}


void Weather::getWeatherData()
{
    connect(&m_manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(weatherDataFinish(QNetworkReply*)));
    // The API key is generated from a dummy email
    QString urlStr = QString("https://api.openweathermap.org/data/2.5/weather?lat="+QString::number(m_lat)+"&lon="+QString::number(m_lon)+"&appid=9787a25230da4c6dd8c81cece93f3895");
    QUrl url(urlStr);
    QNetworkRequest request(url);
    m_manager.get(request);
}

void Weather::weatherDataFinish(QNetworkReply *reply)
{
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (!redirectionTarget.isNull()) {
        QUrl newUrl = QUrl(redirectionTarget.toUrl());
        QNetworkRequest request(newUrl);
        m_manager.get(request);
    } else {
        QString strReply = (QString)reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
        if (!jsonResponse.isNull()) {
            QJsonObject jsonObj = jsonResponse.object();

            QJsonArray tempArr = jsonObj["weather"].toArray();
            m_description = tempArr[0].toObject()["description"].toString();
            m_iconName = tempArr[0].toObject()["icon"].toString();

            QJsonObject tempObj = jsonObj["main"].toObject();
            m_temperature = convertToCels(tempObj["temp"].toDouble());

            tempObj = jsonObj["sys"].toObject();
            m_countryName = tempObj["country"].toString();

            m_cityName = jsonObj["name"].toString();

            emit iconIdReady();
        }
        else {
           // Parse error
        }

    }
    reply->deleteLater();
}

QString Weather::getIconName() const
{
    return m_iconName;
}

QPixmap Weather::getIcon() const
{

    return m_icon;
}

QString Weather::getDescription() const
{
    return m_description;
}

int Weather::getTemperature() const
{
    return m_temperature;
}

QString Weather::getCountryName() const
{
    return m_countryName;
}

QString Weather::getCityName() const
{
    return m_cityName;
}














