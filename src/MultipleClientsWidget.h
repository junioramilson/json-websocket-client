#ifndef MULTIPLECLIENTSWIDGET_H
#define MULTIPLECLIENTSWIDGET_H

#include <QWidget>

namespace Ui {
class MultipleClientsWidget;
}

class MultipleClientsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MultipleClientsWidget(QWidget *parent = nullptr);
    ~MultipleClientsWidget();

private:
    Ui::MultipleClientsWidget *ui;
};

#endif // MULTIPLECLIENTSWIDGET_H
