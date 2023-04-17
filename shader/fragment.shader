#version 410 core
in vec3 myColor;
in vec2 texCoord;
out vec4 FragColor;

uniform vec4 globalColor;
uniform sampler2D ourTexture;
void main(){
    FragColor = texture(ourTexture, texCoord);
}