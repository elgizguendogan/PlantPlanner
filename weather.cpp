#include "weather.h"

Weather::Weather(QObject *parent) : QObject(parent)
{
    connect(this,&Weather::readyToGetWeatherInfo,this,&Weather::getWeatherData);
    //connect(this,&Weather::iconIdReady,this,&Weather::getWeatherIcon);
}


void Weather::getLocationData()
{
    // Get the location information
    QGeoPositionInfoSource *source = QGeoPositionInfoSource::createDefaultSource(this);
   if (source) {
       connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)),
               this, SLOT(positionUpdated(QGeoPositionInfo)));

      // source->setPreferredPositioningMethods(QGeoPositionInfoSource::NonSatellitePositioningMethods);
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
    //qDebug() << "Position updated:" << info;
}

int Weather::convertToCels(double kelvin)
{
    return qRound(kelvin-273.15);
}


void Weather::getWeatherData()
{
    connect(&m_manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(weatherDataFinish(QNetworkReply*)));
    //
    QString urlStr = QString("https://api.openweathermap.org/data/2.5/weather?lat="+QString::number(m_lat)+"&lon="+QString::number(m_lon)+"&appid=fffe62094d85cb152d8fc5a31b335604");
    //QString urlStr = QString("https://api.openweathermap.org/data/2.5/weather?q="+location+"&appid=fffe62094d85cb152d8fc5a31b335604");
    QUrl url(urlStr);
    QNetworkRequest request(url);
    m_manager.get(request);
}

void Weather::weatherDataFinish(QNetworkReply *reply)
{
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
   // qDebug() << redirectionTarget;
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

/*void Weather::getWeatherIcon()
{
    connect(&m_manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(iconDownload(QNetworkReply*)));
    QString urlStr = QString("http://openweathermap.org/img/wn/"+ m_iconName +"@2x.png");
    QUrl url(urlStr);
    QNetworkRequest request(url);
    m_manager.get(request);
}

void Weather::iconDownload(QNetworkReply *pReply)
{
    QVariant redirectionTarget = pReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
   // qDebug() << redirectionTarget;
    if (!redirectionTarget.isNull()) {
        QUrl newUrl = QUrl(redirectionTarget.toUrl());
        QNetworkRequest request(newUrl);
        m_manager.get(request);
    } else {
        qDebug() << pReply->readAll();
        m_iconData = pReply->readAll();
        m_icon.loadFromData(m_iconData);
        emit iconDataDownloaded();
    }
    pReply->deleteLater();
}*/



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














