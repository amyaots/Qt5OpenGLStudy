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

public slots:
    void initialiseOpenGLScene();
    void renderOpenGLScene();
    void cleanupOpenGLScene();
    void update();

protected:
    void keyPressEvent(QKeyEvent *);

private:
    AbstractScene*   m_scene;
    QTime           m_time;
};

#endif // QUICKWINDOW_H
