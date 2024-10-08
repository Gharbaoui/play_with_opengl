#version 330 core
out vec4 frag_color;
in vec3 our_color;
in vec2 tex_coord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // frag_color = vec4(our_color, 1.0f);
    // frag_color = texture(texture1, tex_coord) * vec4(our_color, 1.0f);
    // frag_color = texture(texture1, tex_coord);
    frag_color = mix(
        texture(texture1, tex_coord), texture(texture2, tex_coord), 0.5
    );
}