#ifndef WS_CLIENT_TAB_WIDGET_H
#define WS_CLIENT_TAB_WIDGET_H

#include <QWidget>
#include "../WebSocket/WebSocketClient.h"
#include <QStringListModel>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "JSSintaxHighlighter.h"

namespace Ui {
class WSClientTabWidget;
}

class WSClientTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WSClientTabWidget(QWidget *parent = nullptr);
    ~WSClientTabWidget();

    void closeConnection();
    void loadLastUrl(QString lastUrl);
    void loadMessages(const QMap<QString, QString>& messagesMap);

    QString GetUpdatedConfig() const;

    void setIgnoreResponseTexts(QStringList listText);
    void setRequestMessageValue(QString value);

    QString GetCurrentRequestMessageText() const;

private slots:
    void on_connectBtn_clicked();
    void on_connected();
    void on_disconnected();
    void on_new_message_received(QString message);
    void on_sendMessageBtn_clicked();
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_formatMessageBtn_clicked();
    void on_clearResponsesBtn_clicked();
    void on_saveToTxtBtn_clicked();

private:
    void appendResponseMsg(QString message);

private:
    enum class EButtonConnectDisconnectState {
        CONNECT = 0,
        DISCONNECT
    };

    Ui::WSClientTabWidget *ui;
    QStringListModel* m_stringListModel = nullptr;
    WebSocketClient* m_pWebSocketClient = nullptr;
    Highlighter* m_highlighter = nullptr;

    EButtonConnectDisconnectState m_btnConnectDisconnectState;

    QMap<QString, QString> m_messagesMap;
    QStringList m_ignoredResponseTexts;
};

#endif // WS_CLIENT_TAB_WIDGET_H
