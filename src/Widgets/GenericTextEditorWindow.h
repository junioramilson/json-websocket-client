#ifndef GENERICTEXTEDITORWINDOW_H
#define GENERICTEXTEDITORWINDOW_H

#include <QDialog>
#include "JSSintaxHighlighter.h"

namespace Ui {
class GenericTextEditorWindow;
}

class GenericTextEditorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GenericTextEditorWindow(QWidget *parent = nullptr);
    ~GenericTextEditorWindow();

    void setContent(QString content);

signals:
    void onSubmit(const QString& modifiedContent);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::GenericTextEditorWindow *ui;

    QString m_content;
    Highlighter* m_highlighter = nullptr;
};

#endif // GENERICTEXTEDITORWINDOW_H
