#include "WebSocketClient.h"

WebSocketClient::WebSocketClient(const QUrl& url, QMap<QString, QString> rawHeaders, QObject *parent)
    : QObject(parent)
    , m_url(url)
{

    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::closeConnection);

    QNetworkRequest netRequest;

    netRequest.setUrl(QUrl(url));

    for (auto key : rawHeaders.keys())
    {
        const QString value = rawHeaders.value(key);
        netRequest.setRawHeader(key.toUtf8(), value.toUtf8());
    }

    m_webSocket.open(netRequest);
}

void WebSocketClient::closeConnection()
{
    m_webSocket.close(QWebSocketProtocol::CloseCode::CloseCodeNormal);
    emit disconnected();
}

void WebSocketClient::sendMessage(QString message)
{
    m_webSocket.sendTextMessage(message);
}

void WebSocketClient::open(QString url)
{
    m_webSocket.open(QUrl(url));
}

void WebSocketClient::onConnected()
{
    emit connected();
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onMessageReceived);
}

void WebSocketClient::onMessageReceived(QString message)
{
    emit newMessageReceived(message);
}
