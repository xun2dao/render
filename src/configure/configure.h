#pragma once

#include<string>
enum Configuration{
    VERTEX_SHADER_PATH,
    FRAGMENT_SHADER_PATH
};
class ConfigureManager{
public:
    ConfigureManager(std::string filepath){

    }
    const std::string VertexShaderPath() const{
        return m_vertex_shader_path;
    }

    const std::string FragmentShaderPath() const{
       return m_fragment_shader_path;
    }
private:
    std::string m_vertex_shader_path;
    std::string m_fragment_shader_path;
};