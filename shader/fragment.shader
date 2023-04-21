#version 410 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main(){
    vec3 myColor = vec3(0.5, 0.3, 0.2);
    float ambientFactor = 0.1;
    vec3 ambient = ambientFactor * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir),0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(reflectDir, viewDir), 0), 256);
    float specularStrength = 0.5;
    vec3 specular = spec * specularStrength * lightColor;

    FragColor = vec4((diffuse + ambient + specular) * myColor, 1.0);

    
}