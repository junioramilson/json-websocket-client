#include "MainWindow.h"
#include "WSClientTabWidget.h"
#include "ui_Main.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <iostream>
#include <QInputDialog>
#include <QPlainTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    WSClientTabWidget* programTab = new WSClientTabWidget();
    ui->sessionsTabWidget->addTab(programTab, tr("WS Client 1"));
    m_tabs++;

    loadConfig(programTab);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_sessionsTabWidget_tabCloseRequested(int index)
{
    QWidget* pWidget = ui->sessionsTabWidget->widget(index);

    if (WSClientTabWidget* pProgramTab = dynamic_cast<WSClientTabWidget*>(pWidget))
    {
        pProgramTab->closeConnection();
        pProgramTab->close();
    }
    ui->sessionsTabWidget->removeTab(index);
}

void MainWindow::on_clientsActionNew_triggered()
{
    m_tabs++;

    bool ok;
    QString text = QInputDialog::getText(this, tr("Set tab name"),
                                            tr("Tab name"), QLineEdit::Normal,
                                            "Ws Client", &ok);
    if (ok && !text.isEmpty()) {
       WSClientTabWidget* programTab = new WSClientTabWidget();
       const int index = ui->sessionsTabWidget->addTab(programTab, text);
       ui->sessionsTabWidget->setCurrentIndex(index);

       loadConfig(programTab);
    }
}


void MainWindow::on_actionClose_all_triggered()
{
    for (int index = 0; index < ui->sessionsTabWidget->count(); ++index)
    {
        QWidget* pWidget = ui->sessionsTabWidget->widget(index);

        if (WSClientTabWidget* pProgramTab = dynamic_cast<WSClientTabWidget*>(pWidget))
        {
            pProgramTab->closeConnection();
            pProgramTab->close();
        }

        ui->sessionsTabWidget->removeTab(index);
    }

     ui->sessionsTabWidget->removeTab(ui->sessionsTabWidget->currentIndex());
}

void MainWindow::on_actionEditMessages_triggered()
{
    QString actualConfigFileContents = m_jsonParser->GetJsonDocument().toJson(QJsonDocument::Indented);
    QInputDialog editMessageDialog(this);
    editMessageDialog.setInputMode(QInputDialog::InputMode::TextInput);
    editMessageDialog.setOption(QInputDialog::InputDialogOption::UsePlainTextEditForTextInput);
    editMessageDialog.setLabelText("Edit message");
    editMessageDialog.setTextValue(actualConfigFileContents);
    editMessageDialog.resize(800, 600);

    editMessageDialog.exec();

    QString editedText = editMessageDialog.textValue();
    if (!editedText.isEmpty() && actualConfigFileContents != editedText) {
        QString fileName = QDir::currentPath() + "/config.json";

        if (fileName.isEmpty())
            return;
        else
        {
            QFile file(fileName);
            if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
            {
                QMessageBox::information(this, tr("Unable to open file"), file.errorString());
                return;
            }

            file.write(editedText.toUtf8());
            file.close();
        }
    }

    if (WSClientTabWidget* tabWidget = dynamic_cast<WSClientTabWidget*>(ui->sessionsTabWidget->currentWidget()))
    {
        loadConfig(tabWidget);
    }
}

void MainWindow::loadConfig(WSClientTabWidget* pTabWidget)
{
    QString fileName = QDir::currentPath() + "/config.json";

    if (fileName.isEmpty())
        return;
    else
    {
        QFile file(fileName);

        if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }

        QString fileContentStr = file.readAll();

        file.close();

        m_jsonParser = new JsonConfigParser(fileContentStr);

        auto messagesMap = m_jsonParser->GetMessages();
        pTabWidget->loadMessages(messagesMap);
        pTabWidget->loadLastUrl(m_jsonParser->GetLastUrl());
    }
}

void MainWindow::notifyAllTabs()
{
    for (int i = 0; i < ui->sessionsTabWidget->count(); i++)
    {
        WSClientTabWidget* tabWidget = dynamic_cast<WSClientTabWidget*>(ui->sessionsTabWidget->widget(i));
    }
}

void MainWindow::on_actionResponse_message_settings_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Ignore messages"),
                                            tr("Ignore messages that contains:"), QLineEdit::Normal,
                                            "heartbeat,test", &ok);
    if (ok && !text.isEmpty())
    {
        if (WSClientTabWidget* tabWidget = dynamic_cast<WSClientTabWidget*>(ui->sessionsTabWidget->currentWidget()))
        {
            tabWidget->setIgnoreResponseTexts(text.split(","));
        }
    }
}

void MainWindow::on_newMessageBtn_clicked()
{
    QInputDialog editMessageDialog(this);
    editMessageDialog.setInputMode(QInputDialog::InputMode::TextInput);
    editMessageDialog.setOption(QInputDialog::InputDialogOption::UsePlainTextEditForTextInput);
    editMessageDialog.setLabelText("New JSON Message below");
    editMessageDialog.resize(800, 600);

    editMessageDialog.exec();

    QString newMessage = editMessageDialog.textValue();

    QJsonDocument jdoc = QJsonDocument::fromJson(newMessage.toUtf8());

    if (jdoc.isObject())
    {
        QString compactedJson = jdoc.toJson(QJsonDocument::Compact);

        bool readyToAddMessage = false;
        while (!readyToAddMessage)
        {
            bool okMessageName;
            QString messageNameText = QInputDialog::getText(this, tr("Message name"),
                                      tr("Write the message name that will appear in the left panel"), QLineEdit::Normal,
                                      "", &okMessageName);

            QList<QListWidgetItem*> listItems = ui->messagesListWidget->findItems(messageNameText, Qt::MatchExactly);
            if (listItems.length() > 0)
            {
                QMessageBox::warning(this, "Invalid name", "Cannot add duplicated message names.", QMessageBox::Ok | QMessageBox::Cancel);
                continue;
            }

            if (okMessageName)
            {
                QListWidgetItem* item = new QListWidgetItem(ui->messagesListWidget);
                item->setText(messageNameText);

                m_messageListMap.insert(messageNameText, compactedJson);

                ui->messagesListWidget->addItem(item);
                readyToAddMessage = true;
            }
            else
            {
                QMessageBox::warning(this, "Invalid name", "Message name cannot be empty", QMessageBox::Ok | QMessageBox::Cancel);
                continue;
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "Invalid json", "Invalid JSON. Please check your JSON document.");
    }
}

void MainWindow::on_messagesListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    auto pair = m_messageListMap.find(item->text().toUtf8());

    QJsonDocument jdoc = QJsonDocument::fromJson(pair.value().toUtf8());
    if (WSClientTabWidget* tabWidget = dynamic_cast<WSClientTabWidget*>(ui->sessionsTabWidget->currentWidget()))
    {
        tabWidget->setRequestMessageValue(jdoc.toJson(QJsonDocument::Indented));
    }
}

void MainWindow::on_saveMessagesBtn_clicked()
{
    if (WSClientTabWidget* tabWidget = dynamic_cast<WSClientTabWidget*>(ui->sessionsTabWidget->currentWidget()))
    {
        for (QString test : m_messageListMap)
        {
            std::cout << test.toStdString() << "\n";
        }

        //tabWidget->GetCurrentRequestMessageText();
    }
}
