#version 330
const vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
in vec3 in_vertex
out vec3 color_out;
void main(void)
{
    gl_Position =
    gl_ProjectionMatrix*gl_ModelViewMartrix*gl_Vertex *vec4(in_vertex, 1.0);
    color_out = red;
}
