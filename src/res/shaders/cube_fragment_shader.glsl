#version 330 core

out vec4 color;
in vec3 vertex_color;
in vec2 texture_coordinates;

uniform sampler2D texture_1;
uniform vec3 light_color;


void main()
{
    // color = vec4(vertex_color, 1.0f);
    color = texture(texture_1, texture_coordinates);
}