#include "quickwindow.h"
//#include "window.h"   basic window with OpenGL context 4.3 Core profile
#include <QApplication>
#include <QtQuick/QQuickView>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QuickWindow* window = new QuickWindow();
    //window->setPosition(200, 100);
    window->setMinimumSize( QSize( 800, 600 ) );
    window->setResizeMode( QQuickView::SizeRootObjectToView );
    window->setTitle("Qt5 OpenGL 4.3 Study");
    window->setSource(QUrl("qrc:///qml/main.qml"));
    window->show();

    return app.exec();
}
