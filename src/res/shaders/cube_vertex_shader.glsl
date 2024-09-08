#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 t_coor;

out vec3 vertex_color;
out vec2 texture_coordinates;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;


void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    vertex_color = color;
    texture_coordinates = t_coor;
    // gl_Position = vec4(pos, 1.0f);
    // texture_coords = text_coords;
}