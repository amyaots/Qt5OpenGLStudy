#include "quickwindow.h"              //QtQuick Window with OpenGL conext 4.3 Core profile
//#include "window.h"                 //basic window with OpenGL context 4.3 Core profile
#include <QApplication>
#include <QQmlContext>
#include <QQmlProperty>
#include <QQuickItem>


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QuickWindow* window = new QuickWindow();
    //window->setPosition(200, 100);
    window->setMinimumSize( QSize( 1024, 612 ) );
    window->rootContext()->setContextProperty("windowq", window);       //In QML
    window->setResizeMode( QQuickView::SizeRootObjectToView );
    window->setTitle("Qt5 OpenGL 4.3 Study");
    window->setFlags(Qt::FramelessWindowHint);
    window->setSource(QUrl("qrc:///qml/main.qml"));
    window->show();

    QQuickItem *object = window->rootObject();                          //From QML
    QObject *text = object->findChild<QObject*>("text1");
    qDebug() << "Property value:" << QQmlProperty::read(text, "text").toString();
    return app.exec();
}
