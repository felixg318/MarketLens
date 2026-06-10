#ifndef ALPHAVANTAGEHANDLER_H
#define ALPHAVANTAGEHANDLER_H

#include <apihandler.h>

class AlphaVantageHandler : public APIHandler {
public:
    AlphaVantageHandler();
    QUrl buildRequest(const QString &symbol, const QString &apiKey) override;
    Response parseResponse(const QByteArray &data) override;
private:
    std::string m_name {"Alpha Vantage"};
    std::string m_queryLink {"https://www.alphavantage.co/query?"};
    bool m_premium {0}; //default to premium
};

#endif // ALPHAVANTAGEHANDLER_H
