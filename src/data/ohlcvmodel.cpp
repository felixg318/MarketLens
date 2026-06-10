#include <ohlcvmodel.h>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLocale>

OHLCVModel::OHLCVModel(QObject *parent) : QAbstractTableModel(parent) {}

void OHLCVModel::setData(const QVector<OHLCV> &rows) {
    beginResetModel();
    m_rows = rows;
    endResetModel();
    
    // Emit signal for views that need it (e.g., CandlestickChart)
    if (!m_rows.isEmpty()) {
        emit dataChanged(0, m_rows.size() - 1);
    }
}

void OHLCVModel::exportToCsv(QUrl pathToFile) {
    QFile file(pathToFile.toDisplayString(QUrl::RemoveScheme));

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        stream << "timestamp,open,high,low,close,volume" << Qt::endl;
        for (const auto &row : m_rows)
            stream << row.timestamp << ","
                   << row.open << "," << row.high << ","
                   << row.low << "," << row.close << ","
                   << row.volume << Qt::endl;

        file.close();
        qDebug() << "Writing finished";
    }
}

void OHLCVModel::exportToJson(QUrl pathToFile) {
    QFile file(pathToFile.toDisplayString(QUrl::RemoveScheme));

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QJsonArray arr;
        for (const auto &row : m_rows) {
            QJsonObject obj;
            obj["timestamp"] = row.timestamp;
            obj["open"] = row.open;
            obj["high"] = row.high;
            obj["low"] = row.low;
            obj["close"] = row.close;
            obj["volume"] = row.volume;
            arr.append(obj);
        }
        file.write(QJsonDocument(arr).toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "Writing finished";
    }
}

//QML TableView related functions
const QVector<OHLCV> &OHLCVModel::rows() const {
    return m_rows;
}

int OHLCVModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_rows.size();
}

int OHLCVModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 6;
}

QVariant OHLCVModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    const auto &row = m_rows.at(index.row());
    switch (index.column()) {
        case 0: return row.timestamp;
        case 1: return row.open;
        case 2: return row.high;
        case 3: return row.low;
        case 4: return row.close;
        case 5: return QLocale().toString(row.volume);
    }

    return QVariant();
}

QVariant OHLCVModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();
    switch (section) {
        case 0: return QStringLiteral("Date");
        case 1: return QStringLiteral("Open");
        case 2: return QStringLiteral("High");
        case 3: return QStringLiteral("Low");
        case 4: return QStringLiteral("Close");
        case 5: return QStringLiteral("Volume");
    }
    return QVariant();
}

QHash<int, QByteArray> OHLCVModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    return roles;
}
