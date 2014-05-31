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
    void genSphere();
    QOpenGLShaderProgram        m_shaderProgram;
    QOpenGLBuffer               m_vertexBuffer;
    QOpenGLBuffer               m_indexBuffer;
    QOpenGLBuffer               m_normalBuffer;
    //GLuint                      m_vbo;
    QVector<unsigned int> el;
    QVector<float> v;
    QVector<float> n;         // Normals
    int indexCount() const { return 6 * m_slices * m_rings; }
    float m_radius;
    int m_rings;  // Rings of latitude
    int m_slices; // Longitude
    QOpenGLVertexArrayObject    m_vao;
    int m_frame;
    quint32 m_fov;
};

#endif // SCENE_H
