#version 130

in vec4 vertex;
in vec4 normal;
uniform mat4 mvp_matrix;

varying vec4 normal_frag;

void main()
{
    gl_Position = mvp_matrix * vertex;
    normal_frag = normal;
}
