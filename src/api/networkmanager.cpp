#include <networkmanager.h>
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent) { m_manager = new QNetworkAccessManager(this); }

void NetworkManager::setTimeout(int ms) { m_timeoutMs = ms; }

void NetworkManager::getRequest(const QUrl &url) {
    m_request.setUrl(url);
    m_request.setRawHeader("User-Agent", "Mozilla/5.0");
    m_request.setTransferTimeout(m_timeoutMs);

    m_reply = m_manager->get(m_request);
    connect(m_reply, &QNetworkReply::finished, this, &NetworkManager::onReplyFinished);
}

void NetworkManager::onReplyFinished() {
    QByteArray data = m_reply->readAll();
    //qDebug() << "NetworkManager response:" << data;

    if (m_reply->error() == QNetworkReply::NoError)
        emit responseReady(data);
    else
        emit errorOccurred(m_reply->errorString());

    m_reply->deleteLater();
    m_reply = nullptr;
}
