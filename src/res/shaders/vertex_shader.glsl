#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in  vec3 color;
layout (location = 2) in vec2 texture_coordinates;
out vec3 our_color;
out vec2 tex_coord;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(){
    gl_Position = projection * view * model * vec4(pos.x, pos.y, pos.z, 1.0);
    our_color = color;
    tex_coord = texture_coordinates;
}