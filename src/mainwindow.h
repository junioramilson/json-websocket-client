#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "JSONConfigParser.h"
#include "WSClientTabWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_sessionsTabWidget_tabCloseRequested(int index);
    void on_clientsActionNew_triggered();
    void on_actionClose_all_triggered();

    void on_actionEditMessages_triggered();
    void on_actionResponse_message_settings_triggered();

    void on_newMessageBtn_clicked();
    void on_messagesListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_saveMessagesBtn_clicked();

private:
    void loadConfig(WSClientTabWidget* pTabWidget);
    void notifyAllTabs();


    Ui::MainWindow *ui;
    int m_tabs = 0;

    QMap <QString, QString> m_messageListMap;

    JsonConfigParser* m_jsonParser = nullptr;
};
#endif // MAINWINDOW_H
