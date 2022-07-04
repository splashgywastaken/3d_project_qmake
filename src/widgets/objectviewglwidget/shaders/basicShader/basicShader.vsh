//uniform mat4 modelViewMatrix;
//uniform mat4 projectionMatrix;
//uniform mat4 normalMatrix;
//attribute vec3 vertex;
//attribute vec3 normal;

//varying vec3 m_vertex;
//varying vec3 m_normal;

//void main(void)
//{
//    m_vertex = vec4(modelViewMatrix * vec4(vertex, 1.0)).xyz;
//    m_normal = vec4(normalMatrix * vec4(normal, 1.0)).xyz;
//    gl_Position = projectionMatrix * vec4(m_vertex, 1.0);
//}

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
attribute vec3 vertex;

void main(void)
{
    gl_Position = projectionMatrix * modelViewMatrix * vec4(vertex, 1.0);
}
