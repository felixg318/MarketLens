#include <alphavantagehandler.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QUrlQuery>

AlphaVantageHandler::AlphaVantageHandler() {}

QUrl AlphaVantageHandler::buildRequest(const QString &symbol, const QString &apiKey) {
    QUrl url(QString::fromStdString(m_queryLink));
    QUrlQuery query;

    query.addQueryItem("function", "TIME_SERIES_DAILY");
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("apikey", apiKey);
    url.setQuery(query);

    return url;
}

Response AlphaVantageHandler::parseResponse(const QByteArray &data) {
    Response result;
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull()) {
        QString err = "QJsonDocument::fromJson(data) - Failed to parse API response into JSON";
        result.error = err;
        return result;
    }

    QJsonObject root = doc.object();
    QString symbol = root["Meta Data"].toObject()["2. Symbol"].toString();
    QJsonObject timeSeries = root["Time Series (Daily)"].toObject();

    result.symbol = symbol;
    result.interval = "daily";

    for (auto it = timeSeries.begin(); it != timeSeries.end(); ++it) {
        QJsonObject day = it.value().toObject();
        OHLCV bar;
        bar.timestamp = it.key();
        bar.open   = day["1. open"].toString().toDouble();
        bar.high   = day["2. high"].toString().toDouble();
        bar.low    = day["3. low"].toString().toDouble();
        bar.close  = day["4. close"].toString().toDouble();
        bar.volume = day["5. volume"].toString().toInt();

        result.bars.append(bar);
    }
    //qDebug() << doc;
    return result;
}