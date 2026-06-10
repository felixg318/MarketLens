#include <candlestickchart.h>
#include <profile.h>
#include <QPainter>
#include <algorithm>
#include <cmath>

CandlestickChart::CandlestickChart(QQuickItem *parent) : QQuickPaintedItem(parent) {
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
    setAntialiasing(true);
}

CandlestickChart::~CandlestickChart() = default;

void CandlestickChart::setModel(OHLCVModel *m) {
    if (m_model) {
        disconnect(m_model, nullptr, this, nullptr);
    }
    m_model = m;
    if (m_model) {
        connect(m_model, &OHLCVModel::dataChanged, 
                this, &CandlestickChart::onModelDataChanged);
    }
    emit modelChanged();
    QQuickItem::update();
}

void CandlestickChart::setProfile(Profile *p) {
    if (m_profile) {
        disconnect(m_profile, nullptr, this, nullptr);
    }
    m_profile = p;
    if (m_profile) {
        // Update colors from profile when theme changes
        connect(m_profile, &Profile::themeChanged, this, [this]() {
            if (m_profile && m_profile->colors()) {
                m_backgroundColor = m_profile->colors()->panelBg();
                m_textColor = m_profile->colors()->textColor();
                QQuickItem::update();
            }
        });
        // Set initial colors
        m_backgroundColor = m_profile->colors()->panelBg();
        m_textColor = m_profile->colors()->textColor();
    }
    emit profileChanged();
    QQuickItem::update();
}

void CandlestickChart::onModelDataChanged() {
    QQuickItem::update();
}

void CandlestickChart::paint(QPainter *painter) {
    QRect bounds(this->x(), this->y(), this->width(), this->height());
    painter->fillRect(bounds, m_backgroundColor);
    
    if (!m_model || m_model->rows().isEmpty()) {
        return;
    }
    const QVector<OHLCV> &rows = m_model->rows();
    if (rows.isEmpty()) {
        return;
    }

    // Calculate price range for scaling
    double minPrice = std::numeric_limits<double>::max();
    double maxPrice = std::numeric_limits<double>::lowest();   
    for (const auto &candle : rows) {
        minPrice = std::min(minPrice, candle.low);
        maxPrice = std::max(maxPrice, candle.high);
    }

    // Add some padding to price range
    double priceRange = maxPrice - minPrice;
    if (priceRange < 0.01) priceRange = 0.01;  // Avoid division by zero
    
    minPrice -= priceRange * 0.1;
    maxPrice += priceRange * 0.1;
    priceRange = maxPrice - minPrice;

    // Define margins for axes
    const double leftMargin = 50.0;   // Space for Y-axis labels
    const double rightMargin = 10.0;  // Right padding
    const double topMargin = 10.0;    // Top padding
    const double bottomMargin = 30.0; // Space for X-axis

    // Calculate chart area
    double chartLeft = bounds.left() + leftMargin;
    double chartTop = bounds.top() + topMargin;
    double chartWidth = bounds.width() - leftMargin - rightMargin;
    double chartHeight = bounds.height() - topMargin - bottomMargin;

    // Scale candles to fit available space
    double totalCandleSpace = rows.size() * (m_candleWidth + m_spacing);
    double xScale = chartWidth / totalCandleSpace;
    double candleWidthScaled = m_candleWidth * xScale;
    double spacingScaled = m_spacing * xScale;

    // Draw gridlines and price labels
    painter->setPen(QPen(m_gridColor, 0.5));
    painter->setFont(QFont("Arial", 7));
    painter->setPen(QPen(m_textColor, 1));
    
    for (int i = 0; i <= 5; ++i) {
        double y = chartTop + (chartHeight / 5.0) * i;
        // Draw horizontal gridline
        painter->setPen(QPen(m_gridColor, 0.5));
        painter->drawLine(chartLeft, y, chartLeft + chartWidth, y);
        
        // Draw price label on left axis
        double price = minPrice + (maxPrice - minPrice) * (5 - i) / 5.0;
        painter->setPen(QPen(m_textColor, 1));
        painter->setFont(QFont("Arial", 7));
        painter->drawText(QRectF(bounds.left() + 2, y - 8, leftMargin - 8, 16), 
                         Qt::AlignRight | Qt::AlignVCenter,
                         QString::number(price, 'f', 2));
    }

    // Draw Y-axis line
    painter->setPen(QPen(m_textColor, 1));
    painter->drawLine(chartLeft, chartTop, chartLeft, chartTop + chartHeight);

    // Draw X-axis line
    painter->drawLine(chartLeft, chartTop + chartHeight, chartLeft + chartWidth, chartTop + chartHeight);

    // Draw candles
    double xPos = chartLeft;
    for (const auto &candle : rows) {
        double open = candle.open;
        double high = candle.high;
        double low = candle.low;
        double close = candle.close;

        // Normalize to 0-1 range
        double highNorm = (high - minPrice) / priceRange;
        double lowNorm = (low - minPrice) / priceRange;
        double openNorm = (open - minPrice) / priceRange;
        double closeNorm = (close - minPrice) / priceRange;

        // Convert to pixel coordinates (inverted Y because Qt Y increases downward)
        double highY = chartTop + chartHeight - (highNorm * chartHeight);
        double lowY = chartTop + chartHeight - (lowNorm * chartHeight);
        double openY = chartTop + chartHeight - (openNorm * chartHeight);
        double closeY = chartTop + chartHeight - (closeNorm * chartHeight);

        // Draw wick (high-low line)
        painter->setPen(QPen(QColor(100, 100, 110), 1));
        painter->drawLine(xPos + candleWidthScaled / 2.0, highY,
                         xPos + candleWidthScaled / 2.0, lowY);

        // Determine candle color and body range
        QColor bodyColor = (close >= open) ? m_upColor : m_downColor;
        double bodyTopY = std::min(openY, closeY);
        double bodyBottomY = std::max(openY, closeY);
        double bodyHeight = bodyBottomY - bodyTopY;

        // Ensure minimum body height for visibility
        if (bodyHeight < 2.0) {
            bodyTopY -= 1.0;
            bodyHeight = 2.0;
        }

        // Draw body (open-close rectangle)
        painter->fillRect(QRectF(xPos, bodyTopY, candleWidthScaled, bodyHeight), bodyColor);
        painter->setPen(QPen(bodyColor, 1));
        painter->drawRect(QRectF(xPos, bodyTopY, candleWidthScaled, bodyHeight));

        xPos += candleWidthScaled + spacingScaled;
    }
}
