#ifndef CANDLESTICKCHART_H
#define CANDLESTICKCHART_H

#include <QQuickPaintedItem>
#include <QColor>
#include <ohlcvmodel.h>
#include <profile.h>

class CandlestickChart : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(OHLCVModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(Profile *profile READ profile WRITE setProfile NOTIFY profileChanged)
    Q_PROPERTY(double candleWidth READ candleWidth WRITE setCandleWidth)
    Q_PROPERTY(double spacing READ spacing WRITE setSpacing)
    Q_PROPERTY(QColor upColor READ upColor WRITE setUpColor)
    Q_PROPERTY(QColor downColor READ downColor WRITE setDownColor)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor gridColor READ gridColor WRITE setGridColor)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)

public:
    explicit CandlestickChart(QQuickItem *parent = nullptr);
    ~CandlestickChart();

    OHLCVModel *model() const { return m_model; }
    void setModel(OHLCVModel *m);

    Profile *profile() const { return m_profile; }
    void setProfile(Profile *p);

    double candleWidth() const { return m_candleWidth; }
    void setCandleWidth(double width) {
        if (m_candleWidth != width) {
            m_candleWidth = width;
            QQuickItem::update();
        }
    }

    double spacing() const { return m_spacing; }
    void setSpacing(double s) {
        if (m_spacing != s) {
            m_spacing = s;
            QQuickItem::update();
        }
    }

    QColor upColor() const { return m_upColor; }
    void setUpColor(const QColor &color) {
        if (m_upColor != color) {
            m_upColor = color;
            QQuickItem::update();
        }
    }

    QColor downColor() const { return m_downColor; }
    void setDownColor(const QColor &color) {
        if (m_downColor != color) {
            m_downColor = color;
            QQuickItem::update();
        }
    }

    QColor backgroundColor() const { return m_backgroundColor; }
    void setBackgroundColor(const QColor &color) {
        if (m_backgroundColor != color) {
            m_backgroundColor = color;
            QQuickItem::update();
        }
    }

    QColor gridColor() const { return m_gridColor; }
    void setGridColor(const QColor &color) {
        if (m_gridColor != color) {
            m_gridColor = color;
            QQuickItem::update();
        }
    }

    QColor textColor() const { return m_textColor; }
    void setTextColor(const QColor &color) {
        if (m_textColor != color) {
            m_textColor = color;
            QQuickItem::update();
        }
    }

    void paint(QPainter *painter) override;

private slots:
    void onModelDataChanged();

signals:
    void modelChanged();
    void profileChanged();

private:
    OHLCVModel *m_model = nullptr;
    Profile *m_profile = nullptr;
    double m_candleWidth{8.0};
    double m_spacing{2.0};
    QColor m_upColor{0x26, 0xa6, 0x9a};      // green
    QColor m_downColor{0xef, 0x53, 0x50};    // red
    QColor m_backgroundColor{20, 20, 25};    // dark background
    QColor m_gridColor{50, 50, 60};          // grid lines
    QColor m_textColor{150, 150, 160};       // axis labels
};

#endif // CANDLESTICKCHART_H
