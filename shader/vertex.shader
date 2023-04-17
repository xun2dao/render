#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 Coord;
out vec3 myColor;
out vec2 texCoord;
void main(){
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    myColor = color;
    texCoord = Coord;
}