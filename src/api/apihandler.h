#ifndef APIHANDLER_H
#define APIHANDLER_H

#include <QUrl>
#include <QString>
#include <QByteArray>
#include <QJsonDocument>

#include <ohlcv.h>
enum class APIProvider {
    AlphaVantage
};

struct Response {
    QString symbol;
    QString interval;
    QVector<OHLCV> bars;
    QString error;
};

class APIHandler {
public:
    virtual ~APIHandler() = default;
    virtual QUrl buildRequest(const QString &symbol, const QString &apiKey) = 0;
    virtual Response parseResponse(const QByteArray &data) = 0;
};

#endif // APIHANDLER_H
