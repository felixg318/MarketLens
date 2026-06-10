#ifndef OHLCV_H
#define OHLCV_H

#include <QDateTime>
#include <QString>
#include <QVariantMap>

struct OHLCV {
    double    open{0};
    double    high{0};
    double    low{0};
    double    close{0};
    int       volume{0};
    QString timestamp{};

    QVariantMap toVariantMap() const {
        return {
            {"open", open},
            {"high", high},
            {"low",  low},
            {"close", close},
            {"volume", volume},
            {"timestamp", timestamp},
        };
    }
};

#endif // OHLCV_H
