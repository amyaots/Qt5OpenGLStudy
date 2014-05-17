#include "scene.h"
#include "quickwindow.h"

#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QTimer>
#include <QCoreApplication>


QuickWindow::QuickWindow(QWindow *parent):
    QQuickView(parent),
    m_scene(new Scene(this))
{
    setClearBeforeRendering( false );

    QObject::connect( this, &QuickWindow::sceneGraphInitialized,
                      this, &QuickWindow::initialiseOpenGLScene, Qt::DirectConnection);
    QObject::connect( this, &QuickWindow::beforeRendering,
                      this, &QuickWindow::renderOpenGLScene, Qt::DirectConnection );
    QObject::connect( this, &QuickWindow::sceneGraphInvalidated,
                      this, &QuickWindow::cleanupOpenGLScene, Qt::DirectConnection);

    QTimer* timer = new QTimer( this );
    connect( timer, &QTimer::timeout, this, &QuickWindow::update );
    timer->start( 16 );

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setVersion(4, 3);
    format.setSamples(4);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);

    m_time.start();
}

void QuickWindow::initialiseOpenGLScene()
{
    m_scene->setContext( openglContext() );
    m_scene->initialise();
    m_scene->resize( width(), height() );
}

void QuickWindow::renderOpenGLScene()
{
    m_scene->render( width(), height() );
}

void QuickWindow::cleanupOpenGLScene()
{
    m_scene->cleanup();
}

void QuickWindow::resizeEvent( QResizeEvent* e )
{
    m_scene->resize( width(), height() );
    QQuickView::resizeEvent( e );
}

void QuickWindow::update()
{
    float time = m_time.elapsed()/1000.0f;
    static float lastTime = 0.0;
    static float frames = 0.0;
    ++frames;
    if(time - lastTime > 1.0)
    {
        lastTime = time;
        m_fps = frames;
        emit fpsChanged();
        frames = 0;
    }
    m_scene->update(time);
    QQuickView::update();
}

void QuickWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_Escape:
            QCoreApplication::instance()->quit();
            break;
        case Qt::Key_F1:
            if(windowState()== Qt::WindowMaximized)
                setWindowState(Qt::WindowNoState);
            else setWindowState(Qt::WindowMaximized);
            break;
        case Qt::Key_F2:
            if(windowState()== Qt::WindowFullScreen)
                setWindowState(Qt::WindowNoState);
            else setWindowState(Qt::WindowFullScreen);
            break;
        default:
            QQuickView::keyPressEvent(e);
    }
}

void QuickWindow::wheelEvent(QWheelEvent *e)
{
    //qDebug()<<"Test FOV: "<<m_scene->getFOV();
    if(m_scene->getFOV()>=30 && m_scene->getFOV()<=170)
    {
        m_scene->setFOV(m_scene->getFOV()+(e->delta()/40));
        if(m_scene->getFOV()>170)
        {
            m_scene->setFOV(170);
        }
        if(m_scene->getFOV()<30)
        {
            m_scene->setFOV(30);
        }
    }
}

