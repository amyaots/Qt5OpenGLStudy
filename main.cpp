#include "window.h"

#include <QGuiApplication>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    Window window;
    window.setPosition(200, 100);
    window.setTitle("Qt5 OpenGL 4.3 Study");
    window.show();

    return app.exec();
}
