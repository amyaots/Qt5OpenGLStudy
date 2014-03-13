#ifndef QUICKWINDOW_H
#define QUICKWINDOW_H

#include <QQuickView>
#include <QTime>

class AbstractScene;

class QuickWindow : public QQuickView
{
    Q_OBJECT    
    Q_PROPERTY(float fps READ fps NOTIFY fpsChanged)
public:
    explicit QuickWindow(QWindow* parent = 0);
    AbstractScene* scene() const { return m_scene; }
    float fps() const {return m_fps;}

public slots:
    void initialiseOpenGLScene();
    void renderOpenGLScene();
    void cleanupOpenGLScene();
    void update();
signals:
    void fpsChanged();

protected:
    void resizeEvent( QResizeEvent* e );
    void keyPressEvent(QKeyEvent *e);

private:
    AbstractScene*  m_scene;
    QTime           m_time;
    float           m_fps;
};

#endif // QUICKWINDOW_H
