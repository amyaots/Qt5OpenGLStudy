#version 430
in vec4 color;
in vec4 vertex;
out vec4 vColor;

void main( void )
{
    gl_Position = vertex;
    vColor = color;
}
