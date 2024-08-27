#version 330 core

out vec4 color;
in vec3 vertex_color;
// in vec2 texture_coords;

// uniform sampler2D texture1;
// uniform sampler2D texture2;



void main()
{
    // color = mix(texture(texture1, texture_coords), texture(texture2, texture_coords), 0.5);
    // color = texture(texture2, texture_coords);
    color = vec4(vertex_color, 1.0);
}