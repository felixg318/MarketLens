#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QMap>
#include <QSqlDatabase>
#include <QString>

//make changes to database related files here and in ../../config/
struct StockData {
    int     sqliteId;
    QString ticker;
    QString country;
    QString sector;
    QString name;
    QString exchange;
};

class DBManager
{
public:
    DBManager();
    DBManager(const DBManager&) = delete;
    DBManager& operator=(const DBManager&) = delete;

    void getDBSymbols();
    void addSymbol(const QSqlQuery &q);

    const QMap<QString, StockData>& qStockCache() const { return m_stockCache; }

private:
    QSqlDatabase m_db;
    QMap<QString, StockData> m_stockCache;
};

#endif // DBMANAGER_H
