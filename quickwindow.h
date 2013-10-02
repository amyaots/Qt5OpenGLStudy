#ifndef QUICKWINDOW_H
#define QUICKWINDOW_H

#include <QQuickView>
#include <QTime>

class AbstractScene;

class QuickWindow : public QQuickView
{
    Q_OBJECT
public:
    explicit QuickWindow(QWindow* parent = 0);
    AbstractScene* scene() const { return m_scene; }

public slots:
    void initialiseOpenGLScene();
    void renderOpenGLScene();
    void cleanupOpenGLScene();
    void update();

protected:
    void resizeEvent( QResizeEvent* e );
    void keyPressEvent(QKeyEvent *e);

private:
    AbstractScene*  m_scene;
    QTime           m_time;
};

#endif // QUICKWINDOW_H
