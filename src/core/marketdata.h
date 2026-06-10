#ifndef MARKETDATA_H
#define MARKETDATA_H

#include <QObject>
#include <QQmlEngine>
#include <QVariantList>
#include <memory>

#include <apihandler.h>
#include <ohlcv.h>
#include <dbmanager.h>
#include <networkmanager.h>
#include <ohlcvmodel.h>
#include <profile.h>

//TODO:
//      1) med priority
//      set interval added to api handling by giving front-end options
//      interval enum needed to avoid string comparisons (what is used rn)
//      enum -> string conversion helper for frontend

class MarketData : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString lookupSymbol READ lookupSymbol WRITE setLookupSymbol NOTIFY lookupSymbolChanged)
    Q_PROPERTY(QString userSymbol READ userSymbol WRITE setUserSymbol NOTIFY userSymbolChanged)
    Q_PROPERTY(QString interval READ interval NOTIFY intervalChanged)
    Q_PROPERTY(QVariantList symbolFilterList READ symbolFilterList NOTIFY symbolFilterListChanged)
    Q_PROPERTY(OHLCVModel *ohlcvModel READ ohlcvModel CONSTANT)
public:
    MarketData(Profile *settings, QObject *parent = nullptr);
    QString userSymbol() const;
    QString lookupSymbol() const;
    QVariantList symbolFilterList() const;
    OHLCVModel *ohlcvModel() const;
    QString interval() const;
    void setFilteredSearchList();
    void fetchSymbolData();
signals:
    void userSymbolChanged();
    void symbolFilterListChanged();
    void lookupSymbolChanged();
    void intervalChanged();
public slots:
    void setUserSymbol(const QString &symbol);
    void setLookupSymbol(const QString &symbol);
private slots:
    void onProviderChanged();
    void onAPIResponse(const QByteArray &data);
    void onAPIError(const QString &error);
private:
    void setInterval(const QString &interval);

    std::unique_ptr<APIHandler> m_apiHandler;
    Profile *m_settings;
    DBManager m_db;
    NetworkManager m_net;
    OHLCVModel *m_ohlcvModel;

    QString m_lookupSymbol {};
    QString m_userSymbol {};
    QString m_interval {};

    QVariantList m_symbolFilterList;
};

#endif
