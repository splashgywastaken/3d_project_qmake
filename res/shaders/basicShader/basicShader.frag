uniform vec3 u_objectColor;

void main(void)
{
    gl_FragColor = vec4(u_objectColor, 1.0);
}
