#include "WSClientTabWidget.h"
#include "ui_CustomTabWidget.h"
#include <iostream>
#include <QJsonDocument>

WSClientTabWidget::WSClientTabWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WSClientTabWidget)
{
    ui->setupUi(this);

    m_stringListModel = new QStringListModel(QStringList());

    m_btnConnectDisconnectState = EButtonConnectDisconnectState::CONNECT;
}

WSClientTabWidget::~WSClientTabWidget()
{
    delete ui;
    delete m_pWebSocketClient;
}

void WSClientTabWidget::closeConnection()
{
    if (m_pWebSocketClient)
        m_pWebSocketClient->closeConnection();
}

void WSClientTabWidget::loadLastUrl(QString lastUrl)
{
    ui->urlTextInput->setText(lastUrl);
}

void WSClientTabWidget::loadMessages(const QMap<QString, QString> &messagesMap)
{
    m_messagesMap = messagesMap;

    ui->comboBox->clear();

    for (auto it = messagesMap.begin(); it != messagesMap.end(); it++)
    {
        ui->comboBox->addItem(it.key());
    }

    on_formatMessageBtn_clicked();
}

QString WSClientTabWidget::GetUpdatedConfig() const
{
    return ui->messageTextInput->toPlainText();
}

void WSClientTabWidget::on_connectBtn_clicked()
{
    if (m_btnConnectDisconnectState == EButtonConnectDisconnectState::CONNECT)
    {
        appendResponseMsg(QString("Connecting to %1 ...").arg(ui->urlTextInput->text()));

        QString url = ui->urlTextInput->text();

        if (!m_pWebSocketClient)
        {
            m_pWebSocketClient = new WebSocketClient(url);

            connect(m_pWebSocketClient, &WebSocketClient::newMessageReceived, this, &WSClientTabWidget::on_new_message_received);
            connect(m_pWebSocketClient, &WebSocketClient::connected, this, &WSClientTabWidget::on_connected);
            connect(m_pWebSocketClient, &WebSocketClient::disconnected, this, &WSClientTabWidget::on_disconnected);
        }
        else
        {
            m_pWebSocketClient->open(url);
        }

        m_btnConnectDisconnectState = EButtonConnectDisconnectState::DISCONNECT;
        ui->connectBtn->setText("Disconnect");
        ui->connectBtn->setEnabled(false);
    }
    else
    {
        m_pWebSocketClient->closeConnection();

        m_btnConnectDisconnectState = EButtonConnectDisconnectState::CONNECT;
        ui->connectBtn->setText("Connect");
        ui->connectBtn->setEnabled(true);
    }
}

void WSClientTabWidget::on_connected()
{
    QString msg = "Connected!";
    ui->connectBtn->setEnabled(true);
    appendResponseMsg(msg);
}

void WSClientTabWidget::on_disconnected()
{
    appendResponseMsg(QString("Disconnected!"));
    ui->connectBtn->setEnabled(true);
}

void WSClientTabWidget::on_new_message_received(QString message)
{
    QJsonDocument jdoc = QJsonDocument::fromJson(message.toUtf8());
    QString identedJson = jdoc.toJson(QJsonDocument::Indented);

    appendResponseMsg(identedJson);
}

void WSClientTabWidget::on_sendMessageBtn_clicked()
{
    QString messageStr = ui->messageTextInput->toPlainText();
    if (m_pWebSocketClient)
    {
        m_pWebSocketClient->sendMessage(messageStr);
    }
}

void WSClientTabWidget::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->messageTextInput->document()->setPlainText(m_messagesMap[arg1]);
    on_formatMessageBtn_clicked();
}

void WSClientTabWidget::on_formatMessageBtn_clicked()
{
    QJsonDocument jdoc = QJsonDocument::fromJson(ui->messageTextInput->toPlainText().toUtf8());
    QString identedJson = jdoc.toJson(QJsonDocument::Indented);

    ui->messageTextInput->setPlainText(identedJson);
}

void WSClientTabWidget::appendResponseMsg(QString message)
{
    QStringList stringList = m_stringListModel->stringList();
    stringList.append(message);

    m_stringListModel->setStringList(stringList);

    ui->responsePlainText->appendPlainText(message);
}
