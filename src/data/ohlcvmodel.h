#ifndef OHLCVMODEL_H
#define OHLCVMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QUrl>

#include "ohlcv.h"

class OHLCVModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit OHLCVModel(QObject *parent = nullptr);
    void setData(const QVector<OHLCV> &rows);
    const QVector<OHLCV> &rows() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void exportToCsv(QUrl pathToFile);
    Q_INVOKABLE void exportToJson(QUrl pathToFile);

signals:
    void dataChanged(int firstRow, int lastRow);

private:
    QVector<OHLCV> m_rows;
};

#endif // OHLCVMODEL_H
