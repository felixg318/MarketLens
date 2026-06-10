#ifndef PROFILE_H
#define PROFILE_H

#include <QObject>
#include <QString>
#include <memory>
#include <themecolors.h>

class Profile : public QObject {
    Q_OBJECT
    Q_PROPERTY (QString key READ key WRITE setKey NOTIFY keyChanged)
    Q_PROPERTY (int provider READ provider WRITE setProvider NOTIFY providerChanged)
    Q_PROPERTY (int theme READ theme WRITE setTheme NOTIFY themeChanged)
    Q_PROPERTY(ThemeColors* colors READ colors CONSTANT)
public:
    explicit Profile(QObject *parent = nullptr);
 	QString key() const;
 	int provider() const;
 	int theme() const;
    ThemeColors* colors() const;

    Q_INVOKABLE bool validate();
    Q_INVOKABLE bool keyExists(int provider);

 signals:
 	void keyChanged();
    void providerChanged();
 	void themeChanged();

 public slots:
 	void setKey(QString key);
 	void setProvider(int provider);
 	void setTheme(int theme);

 private:
     void saveProviderToFile();

     QString m_key {};
     int m_provider {};
     int m_theme {};
     QString m_apiFilePath;
     ThemeColors *m_colors;
};

#endif
