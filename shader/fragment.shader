#version 410 core
in vec3 myColor;
in vec2 texCoord;
out vec4 FragColor;

uniform vec4 globalColor;
uniform sampler2D ourTexture;
uniform sampler2D tex1;
void main(){
    FragColor = mix(texture(tex1, texCoord), texture(ourTexture, texCoord),0.1);
}