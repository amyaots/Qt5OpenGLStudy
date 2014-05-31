#version 430 core
//in vec4 vColor;
struct LightInfo
{
    vec4 position;  // Light position in eye coords.
    vec3 intensity; // A,D,S intensity
};
uniform LightInfo light;

struct MaterialInfo
{
    vec3 ka;            // Ambient reflectivity
    vec3 ks;            // Specular reflectivity
    float shininess;    // Specular shininess factor
};
uniform MaterialInfo material;

in vec3 position;
in vec3 normal;
out vec4 fragColor;

vec3 phongModel( vec3 pos, vec3 norm )
{
    vec3 s = normalize( vec3(light.position ) - pos );
    vec3 v = normalize( -pos.xyz );
    vec3 r = reflect( -s, norm );
    vec3 ambient = light.intensity * material.ka;
    float sDotN = max( dot( s, norm ), 0.0 );
    vec3 diffuse = light.intensity * vec3(0.1,0.0,0.5) * sDotN;
    vec3 spec = vec3( 0.0 );
    if ( sDotN > 0.0 )
    {
        spec = light.intensity * material.ks *
               pow( max( dot( r, v ), 0.0 ), material.shininess );
    }

    return ambient + diffuse + spec;
}

void main( void )
{
    fragColor = vec4( normalize(normal), 1.0 );
}
