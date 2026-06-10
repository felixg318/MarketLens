#include "themecolors.h"

#include <QDebug>

ThemeColors::ThemeColors(QObject *parent) : QObject(parent), m_theme(0) {}

QColor ThemeColors::windowBg() const { return m_theme == 1 ? "#f5f5f5" : "#212121"; }
QColor ThemeColors::panelBg() const { return m_theme == 1 ? "#e8e8e8" : "#1b1b1b"; }
QColor ThemeColors::surfaceBg() const { return m_theme == 1 ? "#ffffff" : "#2e2e2e"; }
QColor ThemeColors::popupBg() const { return m_theme == 1 ? "#f0f0f0" : "#1f1f1f"; }
QColor ThemeColors::itemBg() const { return m_theme == 1 ? "#e0e0e0" : "#2a2a2a"; }
QColor ThemeColors::headerBg() const { return m_theme == 1 ? "#d0d0d0" : "#383838"; }
QColor ThemeColors::pressedBg() const { return m_theme == 1 ? "#d0d0d0" : "#3a3a3a"; }
QColor ThemeColors::checkedBg() const { return m_theme == 1 ? "#e0e0e0" : "#3a3a3a"; }
QColor ThemeColors::textColor() const { return m_theme == 1 ? "#000000" : "#ffffff"; }
QColor ThemeColors::labelText() const { return m_theme == 1 ? "#333333" : "#e0e0e0"; }
QColor ThemeColors::secondaryText() const { return m_theme == 1 ? "#666666" : "#dcdcdc"; }
QColor ThemeColors::borderColor() const { return m_theme == 1 ? "#cccccc" : "#3a3a3a"; }
QColor ThemeColors::headerBorder() const { return m_theme == 1 ? "#c0c0c0" : "#484848"; }
QColor ThemeColors::panelBorder() const { return m_theme == 1 ? "#dddddd" : "#282828"; }

void ThemeColors::setTheme(int theme) {
    if (m_theme != theme) {
        m_theme = theme;
        emit colorsChanged();
        qDebug("ThemeColors: theme set to %d", m_theme);
    }
}
