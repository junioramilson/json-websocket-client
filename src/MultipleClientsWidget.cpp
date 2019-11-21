#include "MultipleClientsWidget.h"
#include "ui_MultipleClientsWidget.h"

MultipleClientsWidget::MultipleClientsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MultipleClientsWidget)
{
    ui->setupUi(this);
}

MultipleClientsWidget::~MultipleClientsWidget()
{
    delete ui;
}
