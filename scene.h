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

private:
    void prepareShaderProgram();
    void prepareVertexBuffers();
    void prepareVertexArrayObject();

    QOpenGLShaderProgram        m_shaderProgram;
    QOpenGLBuffer               m_vertexPositionBuffer;
    QOpenGLVertexArrayObject    m_vao;
    QOpenGLFunctions_4_3_Core*  m_funcs;
};

#endif // SCENE_H
