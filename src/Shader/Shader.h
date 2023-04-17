#pragma once
#include<string>
#include<fstream>
#include<sstream>


class Shader{
std::string ReadFromFile(){
    std::ifstream fos(m_file);
    std::stringstream ss;
    std::string line;
    while(getline(fos, line)){
        ss << line;
    }
    return ss.str();
}

public:
Shader(std::string file):m_file(file){
    std::string shader_src = ReadFromFile();
}
private:
unsigned int m_shader_id;
std::string m_file; 
};

