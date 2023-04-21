#pragma once
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<glad/glad.h>
#include<glm/glm.hpp>
class Shader{
std::string ReadFromFile(){
    std::ifstream fos(m_file);
    std::stringstream ss;
    std::string line;
    while(getline(fos, line)){
        ss << line<<"\n";
    }
    return ss.str();
}
public:

enum ShaderType{
    VERTEX_SHADER,
    FRAGMENT_SHADER
};

private:
void Init(){
    std::string shader_src = ReadFromFile();
    m_shader_id = (m_shader_type == ShaderType::VERTEX_SHADER)?glCreateShader(GL_VERTEX_SHADER):glCreateShader(GL_FRAGMENT_SHADER);
    const char* src = shader_src.c_str();
    glShaderSource(m_shader_id, 1, &src,nullptr);  
    glCompileShader(m_shader_id);
    int success;
    char errorInfo[256];
    glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(m_shader_id, 256, nullptr, errorInfo);
        std::cout<< ((m_shader_type == ShaderType::VERTEX_SHADER)?"VertexShader" : "FragmentShader")<<errorInfo<<std::endl;
    }
}
public:

Shader(std::string file, ShaderType type):m_file(file), m_shader_type(type){
    Init();
}
~Shader(){
    glDeleteShader(m_shader_id);

}
unsigned int Id() const {return m_shader_id;}
private:
    unsigned int m_shader_id;
    std::string m_file; 
    ShaderType m_shader_type;
};


class Program{
public:
    Program(){}
    Program(std::string vertexFile, std::string fragmentFile){
        m_program_id = glCreateProgram();
        Shader m_vertex_shader(vertexFile, Shader::ShaderType::VERTEX_SHADER);
        Shader m_fragment_shader(fragmentFile, Shader::ShaderType::FRAGMENT_SHADER);
        glAttachShader(m_program_id, m_vertex_shader.Id());
        glAttachShader(m_program_id, m_fragment_shader.Id());
        glLinkProgram(m_program_id);
    }

void Use(){
    glUseProgram(m_program_id);
}

void Unuse(){
    glUseProgram(0);

}

void SetUniform4f(std::string name, float x, float y, float z, float w){
    auto location = glGetUniformLocation(m_program_id, name.c_str());
    this->Use();
    glUniform4f(location, x, y, z, w);
}

void SetUniform3f(std::string name, glm::vec3 vec){
    auto location = glGetUniformLocation(m_program_id, name.c_str());
    this->Use();
    glUniform3f(location, vec.x, vec.y, vec.z);
}

void SetUniform1i(std::string name, int x){
    auto location = glGetUniformLocation(m_program_id, name.c_str());
    this->Use();
    glUniform1i(location, x);
}

void SetUniformMat4f(std::string name, GLfloat* trans){
    auto location = glGetUniformLocation(m_program_id, name.c_str());
    this->Use();
    glUniformMatrix4fv(location, 1, GL_FALSE, trans);
}


private:
    unsigned int m_program_id;
};