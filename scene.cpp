#include "scene.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>

Scene::Scene(QObject* parent)
    : AbstractScene(parent),
      m_shaderProgram(),
      m_vertexPositionBuffer(QOpenGLBuffer::VertexBuffer)
{
}

void Scene::initialise()
{
    m_funcs = m_context->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if (!m_funcs)
    {
        qFatal("Requires OpenGL >= 4.3");
        exit(1);
    }
    m_funcs->initializeOpenGLFunctions();
    // get context opengl-version
    qDebug() << "Context valid: " << context()->isValid();
    qDebug() << "Used OpenGL: " << context()->format().majorVersion() << "." << context()->format().minorVersion();
    qDebug() << "OpenGL information: VENDOR:       " << (const char*)glGetString(GL_VENDOR);
    qDebug() << "                    RENDERDER:    " << (const char*)glGetString(GL_RENDERER);
    qDebug() << "                    VERSION:      " << (const char*)glGetString(GL_VERSION);
    qDebug() << "                    GLSL VERSION: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

    //Init resources
    prepareShaderProgram();
    prepareVertexBuffers();
    prepareVertexArrayObject();
    glClearColor(0.2f, 0.0f, 0.5f, 1.0f);
    //m_funcs->glDispatchCompute( 512 / 16, 512 / 16, 1 );           //test function in OpenGL 4.3
    //
}

void Scene::update(float t)
{
    Q_UNUSED(t);
}

void Scene::render()
{
    // Clear the buffer with the current clearing color
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_shaderProgram.bind();

    QOpenGLVertexArrayObject::Binder binder( &m_vao );
    // Draw stuff
    glDrawArrays( GL_TRIANGLES, 0, 6 );
    m_shaderProgram.release();
}

void Scene::resize(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Scene::prepareShaderProgram()
{
    //Load and compile the vertex shader
    if(!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/basic.vert"))
        qCritical()<<"Could not compile vertex shader. Log: "<< m_shaderProgram.log();
    //Load and compile the fragment shader
    if(!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/basic.frag"))
        qCritical()<<"Could not compile fragment shader. Log: "<< m_shaderProgram.log();
    //Link the shaders together into a complete shader program (pipeline)
    if(!m_shaderProgram.link())
        qCritical()<<"Could not link shader program. Log: "<< m_shaderProgram.log();
}

void Scene::prepareVertexBuffers()
{
    float points[] = {   -0.90f, -0.90f ,  // Triangle 1
                          0.85f, -0.90f,
                         -0.90f,  0.85f ,
                          0.90f, -0.85f ,  // Triangle 2
                          0.90f,  0.90f ,
                         -0.85f,  0.90f  };
    m_vertexPositionBuffer.create();
    m_vertexPositionBuffer.setUsagePattern(QOpenGLBuffer::StreamDraw);
    if(!m_vertexPositionBuffer.bind())
    {
        qWarning()<<"Could not bind vertex buffer to the context";
        return;
    }
    m_vertexPositionBuffer.allocate(points, sizeof(points));
    m_vertexPositionBuffer.release();
}

void Scene::prepareVertexArrayObject()
{
    //Create a VAO
    QOpenGLVertexArrayObject::Binder binder(&m_vao);
    // Bind the shader program so that we can associate variables from
    // our application to the shaders
    if(!m_shaderProgram.bind())
    {
        qWarning() << "Could not bind shader program to context";
        return;
    }
    m_vertexPositionBuffer.bind();
    m_shaderProgram.setAttributeBuffer("vertex", GL_FLOAT, 0, 2);
    m_shaderProgram.enableAttributeArray("vertex");
}

void Scene::cleanup()
{
    delete m_funcs;
}
