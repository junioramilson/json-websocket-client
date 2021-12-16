#include "generictexteditorwindow.h"
#include "ui_generictexteditorwindow.h"

GenericTextEditorWindow::GenericTextEditorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenericTextEditorWindow)
{
    ui->setupUi(this);
}

GenericTextEditorWindow::~GenericTextEditorWindow()
{
    delete ui;
}
