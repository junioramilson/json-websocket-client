#include "WebSocketClient.h"

WebSocketClient::WebSocketClient(const QUrl& url, bool debug, QObject *parent)
    : QObject(parent)
    , m_url(url)
    , m_debug(debug)
{

    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::closeConnection);

    m_webSocket.open(QUrl(url));
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
