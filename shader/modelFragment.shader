#version 410 core

in vec3 _Normal;
in vec2 _TexCoords;

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

};

uniform Material material;
out vec4 fragColor;

void main(){
    fragColor = texture(material.texture_specular1, _TexCoords) * texture(material.texture_diffuse1, _TexCoords);
}
