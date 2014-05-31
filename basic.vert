#version 430 core
//in vec4 color;
in vec3 vertex;
in vec3 vertexNormal;

out vec3 position;
out vec3 normal;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;
uniform mat3 normalMatrix;

void main( void )
{
    normal = normalMatrix * vertexNormal;
    position = vec4(view*model*vec4(vertex,1.0)).xyz;
    gl_Position = proj*vec4(position,1.0);

}
