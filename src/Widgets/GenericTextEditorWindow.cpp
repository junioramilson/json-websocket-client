#include "GenericTextEditorWindow.h"
#include "ui_GenericTextEditorWindow.h"

GenericTextEditorWindow::GenericTextEditorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenericTextEditorWindow)
{
    ui->setupUi(this);

    m_highlighter = new Highlighter(ui->plainTextEdit->document());
}

GenericTextEditorWindow::~GenericTextEditorWindow()
{
    delete ui;
}

void GenericTextEditorWindow::setContent(QString content)
{
    ui->plainTextEdit->setPlainText(content);
}

void GenericTextEditorWindow::on_buttonBox_accepted()
{
    emit onSubmit(ui->plainTextEdit->toPlainText());
}
