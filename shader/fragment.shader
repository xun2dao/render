#version 410 core

in vec3 Normal;
in vec3 FragPos;
in vec2 texCoord;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 myColor; // ?


struct Material{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D launch;
    float shininess;
};


struct DirLight{
    vec3 _lightPos;
    vec3 _lightDir;
    float _cutOff;
};

struct Light{
    vec3 lightDir;
    vec3 ambient; // 环境光
    vec3 diffuse; // 散射光
    vec3 specular; // 镜面光


    float constant;
    float linear;
    float quadratic;

};

uniform Material material;
uniform Light light;
uniform DirLight _light;

void main(){


    //vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));
//
    //vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(lightPos - FragPos);
    //float diff = max(dot(norm, lightDir),0);
    //vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
//
    //vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 reflectDir = reflect(-lightDir, norm);
    //float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    //vec3 specular = light.specular * (spec * vec3(texture(material.specular, texCoord)));
//
    //vec3 launch = vec3(texture(material.launch, texCoord));
//
    //float len = length(FragPos-lightPos);
    //float delta = 1.0/(light.constant + light.linear*len + light.quadratic * len*len);

    //ambient *= delta;
    //diffuse *= delta;
    //specular *= delta;

    vec3 _lightDir = normalize(FragPos - _light._lightPos);
    float theta = dot(normalize(_light._lightDir), _lightDir);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));



    vec3 launch = vec3(texture(material.launch, texCoord));

    if(theta > _light._cutOff){
        //vec3 viewDir = normalize(viewPos - FragPos);
        //float spec = max(dot(viewDir, reflect(_lightDir, norm)), 0);
        //vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));
        vec3 norm = normalize(Normal);
        vec3 lightDir = -_lightDir;
        float diff = max(dot(lightDir, norm),0);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
        FragColor =  vec4(ambient + diffuse + launch, 1.0);

    }else{
        FragColor =  vec4(ambient , 1.0);
    }


    //FragColor = vec4(ambient + diffuse + specular + launch, 1.0);
}
