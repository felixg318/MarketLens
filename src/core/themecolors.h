#ifndef THEMECOLORS_H
#define THEMECOLORS_H

#include <QColor>
#include <QObject>

class ThemeColors : public QObject {
    Q_OBJECT
    Q_PROPERTY(QColor windowBg READ windowBg NOTIFY colorsChanged)
    Q_PROPERTY(QColor panelBg READ panelBg NOTIFY colorsChanged)
    Q_PROPERTY(QColor surfaceBg READ surfaceBg NOTIFY colorsChanged)
    Q_PROPERTY(QColor popupBg READ popupBg NOTIFY colorsChanged)
    Q_PROPERTY(QColor itemBg READ itemBg NOTIFY colorsChanged)
    Q_PROPERTY(QColor headerBg READ headerBg NOTIFY colorsChanged)
    Q_PROPERTY(QColor pressedBg READ pressedBg NOTIFY colorsChanged)
    Q_PROPERTY(QColor checkedBg READ checkedBg NOTIFY colorsChanged)
    Q_PROPERTY(QColor textColor READ textColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor labelText READ labelText NOTIFY colorsChanged)
    Q_PROPERTY(QColor secondaryText READ secondaryText NOTIFY colorsChanged)
    Q_PROPERTY(QColor borderColor READ borderColor NOTIFY colorsChanged)
    Q_PROPERTY(QColor headerBorder READ headerBorder NOTIFY colorsChanged)
    Q_PROPERTY(QColor panelBorder READ panelBorder NOTIFY colorsChanged)

public:
    explicit ThemeColors(QObject *parent = nullptr);

    QColor windowBg() const;
    QColor panelBg() const;
    QColor surfaceBg() const;
    QColor popupBg() const;
    QColor itemBg() const;
    QColor headerBg() const;
    QColor pressedBg() const;
    QColor checkedBg() const;
    QColor textColor() const;
    QColor labelText() const;
    QColor secondaryText() const;
    QColor borderColor() const;
    QColor headerBorder() const;
    QColor panelBorder() const;

public slots:
    void setTheme(int theme);

signals:
    void colorsChanged();

private:
    int m_theme{0};
};

#endif // THEMECOLORS_H
