#shader vertex
#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in  vec3 color;
out vec3 our_color;

void main(){
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
    our_color = color;
}


#shader fragment
#version 330 core
out vec4 frag_color;
in vec3 our_color;

void main()
{
    frag_color = vec4(our_color, 1.0f);
}