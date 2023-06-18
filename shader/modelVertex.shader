#version 410 core
layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;


out vec3 _Normal;
out vec2 _TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model* vec4(Pos,1.0);
    _Normal = Normal;
    _TexCoords = TexCoords;
}
