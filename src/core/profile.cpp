#include <profile.h>
#include <themecolors.h>

#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDevice>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QSettings>

Profile::Profile(QObject *parent) : QObject(parent), m_key(""), m_provider(0), m_theme(0), m_apiFilePath(QDir(QCoreApplication::applicationDirPath()).filePath("../config/api_keys.json")), m_colors(new ThemeColors(this)) {
    QSettings settings;
    m_theme = settings.value("theme", 0).toInt();
    m_colors->setTheme(m_theme);
}

QString Profile::key() const { return m_key; }
int Profile::provider() const { return m_provider; }
int Profile::theme() const { return m_theme; }
ThemeColors* Profile::colors() const { return m_colors; }

void Profile::setProvider(int newProvider) {
    if (m_provider != newProvider) {
        m_provider = newProvider;
        emit providerChanged();
        qDebug("API provider set to: %d", m_provider);
    }
}

void Profile::setTheme(int newTheme) {
    if (m_theme != newTheme) {
        m_theme = newTheme;
        QSettings settings;
        settings.setValue("theme", m_theme);
        m_colors->setTheme(m_theme);
        emit themeChanged();
    }
}

void Profile::setKey(QString newKey){
    if (m_key != newKey) {
        m_key = newKey;
        emit keyChanged();
    }
}

void Profile::saveProviderToFile() {
    QFile apiFile(m_apiFilePath);

    if (!apiFile.open(QIODeviceBase::ReadWrite)) {
        qWarning("saveProviderToFile: error accessing and writing to config directory");
        return;
    }

    QJsonObject root = QJsonDocument::fromJson(apiFile.readAll()).object();
    QJsonObject keys = root["keys"].toObject();

    keys[QString::number(m_provider)] = m_key;
    root["keys"] = keys;

    apiFile.resize(0);
    apiFile.write(QJsonDocument(root).toJson());
}

bool Profile::keyExists(int provider) {
    QFile apiFile(m_apiFilePath);
    if (!apiFile.open(QIODevice::ReadOnly))
        return false;

    QJsonObject keys = QJsonDocument::fromJson(apiFile.readAll()).object()["keys"].toObject();
    QString key = keys.value(QString::number(provider)).toString();

    qDebug() << "keyExists: {" << provider  << " : " << key << "}";
    setKey(key);
    return !key.isEmpty();
}

//does nothing for now
bool Profile::validate(){
    qDebug("API provider: %d", m_provider);
    qDebug("API key: %s", qPrintable(m_key));

    saveProviderToFile();
    return true; 
}

