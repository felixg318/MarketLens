#include <marketdata.h>
#include <alphavantagehandler.h>

#include <QDebug>
#include <QJsonDocument>
#include <QVariantMap>

static const qsizetype MAX_SEARCH = 20;

MarketData::MarketData(Profile *settings, QObject *parent) : QObject(parent), m_settings(settings) {
    m_ohlcvModel = new OHLCVModel(this);
    connect(&m_net, &NetworkManager::responseReady, this, &MarketData::onAPIResponse);
    connect(&m_net, &NetworkManager::errorOccurred, this, &MarketData::onAPIError);
    connect(m_settings, &Profile::providerChanged, this, &MarketData::onProviderChanged);

    onProviderChanged(); //init m_apiHandler
}

QString MarketData::userSymbol() const { return m_userSymbol; }
QString MarketData::lookupSymbol() const { return m_lookupSymbol; }
QVariantList MarketData::symbolFilterList() const { return m_symbolFilterList; }
OHLCVModel *MarketData::ohlcvModel() const { return m_ohlcvModel; }
QString MarketData::interval() const { return m_interval; }

//getRequest sends signal to onAPIResponse slot
void MarketData::fetchSymbolData() {
    QUrl url = m_apiHandler->buildRequest(m_userSymbol, m_settings->key());
    m_net.getRequest(url);
}

void MarketData::onAPIResponse(const QByteArray &data) {
    if (data.isEmpty()) {
        qWarning("API request is empty");
        return;
    }
    Response result = m_apiHandler->parseResponse(data);
    if (!result.error.isEmpty()) {
        qWarning() << result.error;
        return;
    }
    setInterval(result.interval);
    m_ohlcvModel->setData(result.bars);
}

void MarketData::onAPIError(const QString &error) {
    qWarning("API error: %s", qPrintable(error));
}

//not tested
void MarketData::onProviderChanged() {
    auto provider = static_cast<APIProvider>(m_settings->provider());
    switch(provider) {
        case APIProvider::AlphaVantage:
            m_apiHandler = std::make_unique<AlphaVantageHandler>();
            break;
    }
}

//search does not properly lookup symbols, but works if user is explicit
void MarketData::setFilteredSearchList () {
    QVariantList results;
    //delete previous m_symbolFilterList ??
    results.reserve(MAX_SEARCH);
    const QString q = lookupSymbol().toLower();

    int cnt = 0;
    const auto &stockMap = m_db.qStockCache();

    for (auto it = stockMap.cbegin(); it != stockMap.cend(); ++it) {
        QString name = it.value().name.toLower();

        QVariantMap item;
        item["ticker"] = it.value().ticker;
        item["name"] = it.value().name;

        if ((it.key().contains(q) || name.contains(q)) && cnt < MAX_SEARCH) {
            results.append(item);
            ++cnt;
        }
    }

    m_symbolFilterList = results;
    emit symbolFilterListChanged();
}

void MarketData::setLookupSymbol(const QString &symbol) {
    if (m_lookupSymbol != symbol) {
        m_lookupSymbol = symbol;
        emit lookupSymbolChanged();
        qDebug("Lookup symbol set to: %s", qPrintable(m_lookupSymbol));
        setFilteredSearchList();
    }
}

void MarketData::setUserSymbol(const QString &symbol) {
    if (m_userSymbol != symbol) {
        m_userSymbol = symbol;
        emit userSymbolChanged();
        qDebug("User set symbol to: %s", qPrintable(m_userSymbol));

        fetchSymbolData();
        qDebug("Fetched symbol data for %s", qPrintable(m_userSymbol));
    }
}

void MarketData::setInterval(const QString &interval) {
    if (m_interval != interval) {
        m_interval = interval.toUpper();
        emit intervalChanged();
    }
}
