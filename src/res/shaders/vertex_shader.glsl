#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in  vec3 color;
out vec3 our_color;
uniform float shift_amount;

void main(){
    gl_Position = vec4(shift_amount + pos.x, -pos.y, pos.z, 1.0);
    our_color = vec3(gl_Position.x, gl_Position.y, gl_Position.z);
}