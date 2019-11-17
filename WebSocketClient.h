#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QtNetwork/QAbstractSocket>

class WebSocketClient : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketClient(const QUrl& url, bool debug = false, QObject *parent = nullptr);

    void closeConnection();
    void sendMessage(QString message);

signals:
    void closed();
    void connected();
    void disconnected();
    void newMessageReceived(QString message);

private slots:
    void onConnected();
    void onMessageReceived(QString message);

private:
    QWebSocket m_webSocket;
    QUrl m_url;
    bool m_debug;
};

#endif // WEBSOCKETCLIENT_H
