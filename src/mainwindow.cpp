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
#include "MultipleClientsWidget.h"

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
    else if (MultipleClientsWidget* pProgramTab = dynamic_cast<MultipleClientsWidget*>(pWidget))
    {
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

        if (MultipleClientsWidget* pProgramTab = dynamic_cast<MultipleClientsWidget*>(pWidget))
        {
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

void MainWindow::on_actionCreate_multiple_triggered()
{
     MultipleClientsWidget* multipleClientsWindow = new MultipleClientsWidget(this);
     const int index = ui->sessionsTabWidget->addTab(multipleClientsWindow, QString("Multiple clients"));
     ui->sessionsTabWidget->setCurrentIndex(index);
}
