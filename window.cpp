#include "window.h"
#include "scene.h"

#include <QTimer>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QOpenGLContext>

Window::Window(QScreen *screen): QWindow(screen), m_scene(new Scene)
{
    setSurfaceType(OpenGLSurface);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setVersion(4, 3);
    format.setSamples(4);
    format.setProfile(QSurfaceFormat::CoreProfile);

    resize(512, 512);
    setFormat(format);
    create();
    m_context = new QOpenGLContext;
    m_context->setFormat(format);
    m_context->create();

    m_scene->setContext(m_context);

    initializeGL();

    connect(this, SIGNAL(widthChanged(int)), this, SLOT(reziseGL()));
    connect(this, SIGNAL(heightChanged(int)), this, SLOT(reziseGL()));
    reziseGL();

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateScene()));
    timer->start(16);

    m_time.start();
}

void Window::initializeGL()
{
    m_context->makeCurrent(this);
    m_scene->initialise();
}

void Window::paintGL()
{
    m_context->makeCurrent(this);
    m_scene->render();
    m_context->swapBuffers(this);
}

void Window::reziseGL()
{
    m_context->makeCurrent(this);
    m_scene->resize(width(), height());
}

void Window::updateScene()
{
    float time = m_time.elapsed()/1000.0f;
    m_scene->update(time);
    paintGL();
}

void Window::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_Escape:
            QCoreApplication::instance()->quit();
            break;
        case Qt::Key_F1:
            if(windowState()==Qt::WindowFullScreen)
                setWindowState(Qt::WindowNoState);
            else setWindowState(Qt::WindowFullScreen);
            break;
        default:
            Window::keyPressEvent(e);
    }
}
