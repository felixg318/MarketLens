#include <dbmanager.h>
#include <QCoreApplication>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>

DBManager::DBManager() {
    const QString dbPath = QDir(QCoreApplication::applicationDirPath()).filePath("../config/master.db");

    m_db = QSqlDatabase::addDatabase("QSQLITE", "market");

    if (!m_db.isValid())
        qFatal("Cannot add database: %s", qPrintable(m_db.lastError().text()));

    m_db.setDatabaseName(dbPath);

    if (m_db.open())
        getDBSymbols();
    else
        qFatal("Cannot open database: %s", qPrintable(m_db.lastError().text()));
}

void DBManager::getDBSymbols() {
    QSqlQuery q(m_db);
    QString stmt {"SELECT id, ticker, country, sector, name FROM symbol"};

    q.exec(stmt);
    while (q.next()) {
        addSymbol(q);
    }
    qInfo() << "Loaded" << m_stockCache.size() << "symbols";
}

//guaranteed
void DBManager::addSymbol(const QSqlQuery &q){
    int i = 0;
    StockData symbol;

    symbol.sqliteId     = q.value(i).toInt();
    symbol.ticker       = q.value(i + 1).toString();
    symbol.country      = q.value(i + 2).toString();
    symbol.sector       = q.value(i + 3).toString();
    symbol.name         = q.value(i + 4).toString();
    symbol.exchange     = q.value(i + 5).toString();

    m_stockCache[symbol.ticker.toLower()] = symbol; //can potentially cause collisions
}