#ifndef GENERICTEXTEDITORWINDOW_H
#define GENERICTEXTEDITORWINDOW_H

#include <QDialog>

namespace Ui {
class GenericTextEditorWindow;
}

class GenericTextEditorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GenericTextEditorWindow(QWidget *parent = nullptr);
    ~GenericTextEditorWindow();

private:
    Ui::GenericTextEditorWindow *ui;
};

#endif // GENERICTEXTEDITORWINDOW_H
