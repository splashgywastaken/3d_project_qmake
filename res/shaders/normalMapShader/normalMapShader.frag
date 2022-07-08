varying vec3 m_vertex;
varying vec3 m_normal;
uniform vec3 u_objectColor;

void main(void)
{
    vec3 color = u_objectColor;
    gl_FragColor = vec4(m_normal, 1.0);
}
