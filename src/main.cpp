#include "Widgets/WSMainWindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set style
    qApp->setStyle(QStyleFactory::create("Fusion"));

    // modify palette to dark
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window,QColor(43,43,43));
    darkPalette.setColor(QPalette::WindowText,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::WindowText,QColor(117,117,117));
    darkPalette.setColor(QPalette::Base,QColor(32,32,32));
    darkPalette.setColor(QPalette::AlternateBase,QColor(56,56,56));
    darkPalette.setColor(QPalette::ToolTipBase,Qt::white);
    darkPalette.setColor(QPalette::ToolTipText,Qt::white);
    darkPalette.setColor(QPalette::Text,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::Text,QColor(117,117,117));
    darkPalette.setColor(QPalette::Dark,QColor(20,20,20));
    darkPalette.setColor(QPalette::Shadow,QColor(10,10,10));
    darkPalette.setColor(QPalette::Button,QColor(43,43,43));
    darkPalette.setColor(QPalette::ButtonText,Qt::white);
    darkPalette.setColor(QPalette::Disabled,QPalette::ButtonText,QColor(117,117,117));
    darkPalette.setColor(QPalette::BrightText,Qt::red);
    darkPalette.setColor(QPalette::Link,QColor(32,120,208));
    darkPalette.setColor(QPalette::Highlight,QColor(42,130,218));
    darkPalette.setColor(QPalette::Disabled,QPalette::Highlight,QColor(70,70,70));
    darkPalette.setColor(QPalette::HighlightedText,Qt::black);
    darkPalette.setColor(QPalette::Disabled,QPalette::HighlightedText,QColor(117,117,117));

    qApp->setPalette(darkPalette);

    MainWindow w;
    w.setWindowTitle("JSON WebSocket Client");
    w.show();

    return a.exec();
}
