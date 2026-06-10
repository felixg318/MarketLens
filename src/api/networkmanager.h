#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QString>

class NetworkManager : public QObject {
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);
    void getRequest(const QUrl &url);
    void setTimeout(int ms);

signals:
    void responseReady(const QByteArray&);
    void errorOccurred(const QString&);

private slots:
    void onReplyFinished();

private:
    QNetworkAccessManager *m_manager;
    QNetworkRequest m_request;
    QNetworkReply *m_reply = nullptr;
    int m_timeoutMs{10000};
};

#endif // NETWORKMANAGER_H
