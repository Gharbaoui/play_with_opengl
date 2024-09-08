#version 330 core

out vec4 color;
in vec3 vertex_color;
in vec2 texture_coordinates;

uniform sampler2D our_interface_to_texture_unit;


void main()
{
    // color = vec4(vertex_color, 1.0f);
    color = texture(our_interface_to_texture_unit, texture_coordinates);
}