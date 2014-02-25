#ifndef SCENE_H
#define SCENE_H

#include "abstractscene.h"

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

class QOpenGLFunctions_4_3_Core;

class Scene : public AbstractScene
{
    Q_OBJECT

public:
    Scene(QObject* parent = 0);

    virtual void initialise();
    virtual void update(float t);
    virtual void render();
    virtual void resize(int w, int h);
    virtual void cleanup();

private:
    void prepareShaderProgram();
    void prepareVertexBuffers();
    void prepareVertexArrayObject();
    void quad( int a, int b, int c, int d );
    void colorcube();
    QVector4D                   pointsQuad[36];
    QVector4D*                  m_vertex;
    QVector4D*                  m_vColor;
    QVector4D                   colorsQuad[36];
    QOpenGLShaderProgram        m_shaderProgram;
    QOpenGLBuffer               m_vertexBuffer;
    QOpenGLVertexArrayObject    m_vao;
    QOpenGLFunctions_4_3_Core*  m_funcs;
    int m_frame;
};

#endif // SCENE_H
