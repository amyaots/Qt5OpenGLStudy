#ifndef SCENE_H
#define SCENE_H

#include "abstractscene.h"

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_4_3_Core>


class Scene : public AbstractScene, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT

public:
    Scene(QObject* parent = 0);

    virtual void initialise();
    virtual void update(float t);
    virtual void render(float w, float h);
    virtual void resize(int w, int h);
    virtual void cleanup();
    void setFOV(const quint32 inFOV) {m_fov = inFOV;}
    quint32 getFOV()const {return m_fov;}

private:
    void prepareShaderProgram();
    void prepareVertexBuffers();
    void prepareVertexArrayObject();
    void quad( int a, int b, int c, int d );
    void colorcube();
    QVector<QVector4D>          pointsQuad;
    QVector4D*                  m_vertex;
    QVector4D*                  m_vColor;
    QVector<QVector4D>          colorsQuad;
    QOpenGLShaderProgram        m_shaderProgram;
    QOpenGLBuffer               m_vertexBuffer;
    //GLuint                      m_vbo;
    QOpenGLVertexArrayObject    m_vao;
    int m_frame;
    quint32 m_fov;
};

#endif // SCENE_H
