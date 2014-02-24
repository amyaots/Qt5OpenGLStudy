#version 430
in vec4 color;
in vec4 vertex;
out vec4 vColor;
uniform mat4 matrix;

void main( void )
{
    gl_Position = matrix*vertex;
    vColor = color;
}
