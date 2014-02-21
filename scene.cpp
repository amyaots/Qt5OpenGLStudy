#include "scene.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>

Scene::Scene(QObject* parent)
    : AbstractScene(parent),
      m_shaderProgram(),
      m_vertexPositionBuffer(QOpenGLBuffer::VertexBuffer),
      m_vertexColorBuffer(QOpenGLBuffer::VertexBuffer)
{
}

void Scene::quad(int a, int b, int c, int d)
{
    static int Index = 0;
    colorsQuad[Index] = m_vColor[a]; pointsQuad[Index] = m_vertex[a]; Index++;
    colorsQuad[Index] = m_vColor[b]; pointsQuad[Index] = m_vertex[b]; Index++;
    colorsQuad[Index] = m_vColor[c]; pointsQuad[Index] = m_vertex[c]; Index++;
    colorsQuad[Index] = m_vColor[a]; pointsQuad[Index] = m_vertex[a]; Index++;
    colorsQuad[Index] = m_vColor[c]; pointsQuad[Index] = m_vertex[c]; Index++;
    colorsQuad[Index] = m_vColor[d]; pointsQuad[Index] = m_vertex[d]; Index++;
}

void Scene::colorcube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
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
    //glClearColor(0.2f, 0.0f, 0.5f, 1.0f);
    //m_funcs->glDispatchCompute( 512 / 16, 512 / 16, 1 );           //test function in OpenGL 4.3
    //
    //glEnable( GL_DEPTH_TEST );
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
    glDrawArrays( GL_TRIANGLES, 0, 36 );
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
    /*QVector2D points[] = {   QVector2D(-0.90, -0.90) ,  // Triangle 1
                          QVector2D(0.85, -0.90),
                         QVector2D(-0.90,  0.85) ,
                          QVector2D(0.90, -0.85) ,  // Triangle 2
                          QVector2D(0.90,  0.90) ,
                         QVector2D(-0.85,  0.90)  };*/
    QVector4D vertex[8] = {
        QVector4D( -0.5, -0.5,  0.5, 1.0 ),
        QVector4D( -0.5,  0.5,  0.5, 1.0 ),
        QVector4D(  0.5,  0.5,  0.5, 1.0 ),
        QVector4D(  0.5, -0.5,  0.5, 1.0 ),
        QVector4D( -0.5, -0.5, -0.5, 1.0 ),
        QVector4D( -0.5,  0.5, -0.5, 1.0 ),
        QVector4D(  0.5,  0.5, -0.5, 1.0 ),
        QVector4D(  0.5, -0.5, -0.5, 1.0 )
    };
    QVector4D vColor[8] = {
            QVector4D( 0.0, 0.0, 0.0, 1.0 ),  // black
            QVector4D( 1.0, 0.0, 0.0, 1.0 ),  // red
            QVector4D( 1.0, 1.0, 0.0, 1.0 ),  // yellow
            QVector4D( 0.0, 1.0, 0.0, 1.0 ),  // green
            QVector4D( 0.0, 0.0, 1.0, 1.0 ),  // blue
            QVector4D( 1.0, 0.0, 1.0, 1.0 ),  // magenta
            QVector4D( 1.0, 1.0, 1.0, 1.0 ),  // white
            QVector4D( 0.0, 1.0, 1.0, 1.0 )   // cyan
        };
    m_vertex = vertex;
    m_vColor = vColor;
    colorcube();
    m_vertexPositionBuffer.create();
    m_vertexPositionBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    if(!m_vertexPositionBuffer.bind())
    {
        qWarning()<<"Could not bind vertex buffer to the context";
        return;
    }
    m_vertexPositionBuffer.allocate(pointsQuad, sizeof(pointsQuad));
    m_vertexPositionBuffer.release();
    m_vertexColorBuffer.create();
    m_vertexColorBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    if(!m_vertexColorBuffer.bind())
    {
        qWarning()<<"Could not bind vertex buffer to the context";
        return;
    }
    m_vertexColorBuffer.allocate(colorsQuad, sizeof(colorsQuad));
    m_vertexColorBuffer.release();
}

void Scene::prepareVertexArrayObject()
{
    //Create a VAO binder
    QOpenGLVertexArrayObject::Binder binder(&m_vao);
    // Bind the shader program so that we can associate variables from
    // our application to the shaders
    if(!m_shaderProgram.bind())
    {
        qWarning() << "Could not bind shader program to context";
        return;
    }
    if(!m_vertexPositionBuffer.bind())
    {
        qWarning()<<"Could not bind vertex buffer to the context";
        return;
    }
    m_shaderProgram.setAttributeBuffer("vertex", GL_FLOAT, 0, 4);
    m_shaderProgram.enableAttributeArray("vertex");
    if(!m_vertexColorBuffer.bind())
    {
        qWarning()<<"Could not bind vertex buffer to the context";
        return;
    }

    m_shaderProgram.setAttributeBuffer("color", GL_FLOAT, 0, 4);    
    m_shaderProgram.enableAttributeArray("color");
}

void Scene::cleanup()
{
    delete m_funcs;
}
