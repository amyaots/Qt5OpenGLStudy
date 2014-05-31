#include "scene.h"
#include <QOpenGLContext>

#define _USE_MATH_DEFINES
#include <math.h>

const double pi = M_PI;
const double twoPi = 2.0 * pi;

Scene::Scene(QObject* parent)
    : AbstractScene(parent),
      m_shaderProgram(),
      m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_normalBuffer(QOpenGLBuffer::VertexBuffer),
      m_indexBuffer(QOpenGLBuffer::IndexBuffer),
      m_frame(0),
      m_radius(0.2f),
      m_rings(30),
      m_slices(30)
{
}

void Scene::genSphere()
{
    int faces = (m_slices - 2) * m_rings +           // Number of "rectangular" faces
            (m_rings * 2); // and one ring for the top and bottom caps
    int nVerts  = ( m_slices + 1 ) * ( m_rings + 1 ); // One extra line of latitude
    QVector<float>& vertices = v;
    QVector<unsigned int>& indices = el;
    QVector<float>& normals = n;
    vertices.resize( 3 * nVerts );
    indices.resize( 6 * faces );
    normals.resize( 3 * nVerts );
    const float dTheta = twoPi / static_cast<float>( m_slices );
    const float dPhi = pi / static_cast<float>( m_rings );

    // Iterate over latitudes (rings)
    int index = 0;
    for ( int lat = 0; lat < m_rings + 1; ++lat )
    {
        const float phi = pi / 2.0f - static_cast<float>( lat ) * dPhi;
        const float cosPhi = cosf( phi );
        const float sinPhi = sinf( phi );

        // Iterate over longitudes (slices)
        for ( int lon = 0; lon < m_slices + 1; ++lon )
        {
            const float theta = static_cast<float>( lon ) * dTheta;
            const float cosTheta = cosf( theta );
            const float sinTheta = sinf( theta );

            vertices[index]   = m_radius * cosTheta * cosPhi;
            vertices[index+1] = m_radius * sinPhi;
            vertices[index+2] = m_radius * sinTheta * cosPhi;

            normals[index]   = cosTheta * cosPhi;
            normals[index+1] = sinPhi;
            normals[index+2] = sinTheta * cosPhi;

            index += 3;
        }
    }
    int elIndex = 0;

    // top cap
    {
        const int nextRingStartIndex = m_slices + 1;
        for ( int j = 0; j < m_slices; ++j )
        {
            indices[elIndex] = nextRingStartIndex + j;
            indices[elIndex+1] = 0;
            indices[elIndex+2] = nextRingStartIndex + j + 1;
            elIndex += 3;
        }
    }

    for ( int i = 1; i < (m_rings - 1); ++i )
    {
        const int ringStartIndex = i * ( m_slices + 1 );
        const int nextRingStartIndex = ( i + 1 ) * ( m_slices + 1 );

        for ( int j = 0; j < m_slices; ++j )
        {
            // Split the quad into two triangles
            indices[elIndex]   = ringStartIndex + j;
            indices[elIndex+1] = ringStartIndex + j + 1;
            indices[elIndex+2] = nextRingStartIndex + j;
            indices[elIndex+3] = nextRingStartIndex + j;
            indices[elIndex+4] = ringStartIndex + j + 1;
            indices[elIndex+5] = nextRingStartIndex + j + 1;

            elIndex += 6;
        }
    }

    // bottom cap
    {
        const int ringStartIndex = (m_rings - 1) * ( m_slices + 1);
        const int nextRingStartIndex = (m_rings) * ( m_slices + 1);
        for ( int j = 0; j < m_slices; ++j )
        {
            indices[elIndex] = ringStartIndex + j;
            indices[elIndex+1] = nextRingStartIndex;
            indices[elIndex+2] = ringStartIndex + j + 1;
            elIndex += 3;
        }
    }
}

void Scene::initialise()
{
    //m_funcs = m_context->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if (!m_context->versionFunctions<QOpenGLFunctions_4_3_Core>())
    {
        qFatal("Requires OpenGL >= 4.3");
        exit(1);
    }    
    initializeOpenGLFunctions();
    // get context opengl-version
    qDebug() << "Context valid: " << context()->isValid();
    qDebug() << "Used OpenGL: " << context()->format().majorVersion() << "." << context()->format().minorVersion();
    qDebug() << "OpenGL information: VENDOR:       " << (const char*)glGetString(GL_VENDOR);
    qDebug() << "                    RENDERDER:    " << (const char*)glGetString(GL_RENDERER);
    qDebug() << "                    VERSION:      " << (const char*)glGetString(GL_VERSION);
    qDebug() << "                    GLSL VERSION: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    setFOV(52);
    //Init resources
    prepareShaderProgram();
    prepareVertexBuffers();
    prepareVertexArrayObject();
    glClearColor(0.2f, 0.0f, 0.5f, 1.0f);    //
    glEnable(GL_DEPTH_TEST |GL_CULL_FACE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Scene::update(float t)
{
    Q_UNUSED(t);
}

void Scene::render(float w, float h)
{
    glDepthMask( true );
    glEnable(GL_DEPTH_TEST );
    glClearColor(0.12f, 0.22f, 0.23f, 1.0f);
    QOpenGLVertexArrayObject::Binder binder( &m_vao );
    // Clear the buffer with the current clearing color
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shaderProgram.bind();
    QMatrix4x4 matrix, model2, model3, rot; QMatrix4x4 view; QMatrix4x4 proj;
    matrix.setToIdentity();
    model2.setToIdentity();
    model3.setToIdentity();
    rot.setToIdentity();
    rot.translate(-0.05f,0.2f,0.0f);
    rot.rotate(100*m_frame/80, 0.2f, 1, 0);
    proj.perspective(getFOV(), w/h, 0.1f, 100.f);
    view.lookAt(QVector3D(0.f,0.f,2.f),QVector3D(0.f,0.f,0.f),QVector3D(0.f,1.f,0.f));
    m_shaderProgram.setUniformValue("proj", proj);
    m_shaderProgram.setUniformValue("view", view);
    m_shaderProgram.setUniformValue( "material.diff", QVector3D( 0.1f, 0.0f, 0.5f ) );
    // Draw stuff
    //glDrawArrays( GL_TRIANGLES, 0, 36 );
    matrix.translate(0.05f,-0.2f,-0.3f);
    //matrix.rotate(100*m_frame/80, 0, 1, 0);
    //matrix.translate(0.6f,0.0f,0.0f);
    m_shaderProgram.setUniformValue("model", rot*matrix);
    glDrawElements( GL_TRIANGLES, indexCount(), GL_UNSIGNED_INT, 0 );
    model2.translate(0.05f,-0.2f,0.3f);
    m_shaderProgram.setUniformValue("model", rot*model2);
    glDrawElements( GL_TRIANGLES, indexCount(), GL_UNSIGNED_INT, 0 );
    model3.translate(-0.05f,0.2f,0.0f);
    model3.scale(1.2f);
    m_shaderProgram.setUniformValue("model", model3);
    m_shaderProgram.setUniformValue( "material.diff", QVector3D( 0.5f, 0.0f, 0.1f ) );
    glDrawElements( GL_TRIANGLES, indexCount(), GL_UNSIGNED_INT, 0 );
    m_shaderProgram.release();
    if(++m_frame==288)
       m_frame = 0;
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
    genSphere();
    m_vertexBuffer.create();
    m_vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    if(!m_vertexBuffer.bind())
    {
        qWarning()<<"Could not bind vertex buffer to the context";
        return;
    }
    m_vertexBuffer.allocate( v.constData(), v.size() * sizeof( float ) );
    m_vertexBuffer.release();
    m_normalBuffer.create();
    m_normalBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    if(!m_normalBuffer.bind())
    {
        qWarning()<<"Could not bind vertex buffer to the context";
        return;
    }
    m_normalBuffer.allocate( n.constData(), n.size() * sizeof( float ) );
    m_indexBuffer.create();
    m_indexBuffer.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_indexBuffer.bind();
    m_indexBuffer.allocate( el.constData(), el.size() * sizeof( unsigned int ) );
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
    if(!m_vertexBuffer.bind())
    {
        qWarning()<<"Could not bind vertex buffer to the context";
        return;
    }
    m_shaderProgram.enableAttributeArray("vertex");
    m_shaderProgram.setAttributeBuffer("vertex", GL_FLOAT, 0, 3);
    m_vertexBuffer.release();
    if(!m_normalBuffer.bind())
    {
        qWarning()<<"Could not bind normal buffer to the context";
        return;
    }
    m_shaderProgram.enableAttributeArray( "vertexNormal" );
    m_shaderProgram.setAttributeBuffer( "vertexNormal", GL_FLOAT, 0, 3 );
    m_normalBuffer.release();
    m_shaderProgram.setUniformValue( "light.position", QVector4D( -0.5f, 1.f, 0.0f, 0.0f ) );
    m_shaderProgram.setUniformValue( "light.intensity", QVector3D( 1.0f, 1.0f, 1.0f ) );
    m_shaderProgram.setUniformValue( "material.ks", QVector3D( 0.90f, 0.90f, 0.90f ) );
    m_shaderProgram.setUniformValue( "material.ka", QVector3D( 0.1f, 0.1f, 0.1f ) );
    m_shaderProgram.setUniformValue( "material.shininess", 50.0f );
    m_shaderProgram.release();
    m_vertexBuffer.release();
    m_indexBuffer.bind();
}

void Scene::cleanup()
{
    //glInvalidateBufferData(m_vbo);
    m_vertexBuffer.destroy();
    //delete m_funcs;
}
