#shader vertex
#version 330 core
layout (location = 0) in vec3 pos;

void main(){
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
}


#shader fragment
#version 330 core
out vec4 frag_color;

void main()
{
    frag_color = vec4(1.0f, 0.5f, 0.3f, 1.0f);
}