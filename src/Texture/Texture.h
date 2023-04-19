#pragma once
#include<glad/glad.h>
#include<string>
#include<stb_image.h>
#include<iostream>
/**
 * 1. 纹理坐标
 * 2. 环绕方式
 * 3. 过滤方式
 * 4. 多级纹理
 * 5. 多级纹理的过滤方式
*/

class Texture{
public:
    Texture(std::string file, int level = 0) : m_file_name(file),m_unit(level){
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(m_file_name.c_str(), &m_width, &m_height, &m_channel, 0);
        if(data){
            glGenTextures(1, &m_texture_id);
            glActiveTexture(GL_TEXTURE0+level);
            glBindTexture(GL_TEXTURE_2D, m_texture_id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
           
        }else{
            std::cout<< "Input file for texture doesn't exist."<<std::endl;
        }
         stbi_image_free(data);
    }

    void ActivateTexture(int level){
        glActiveTexture(GL_TEXTURE0+level);
        glBindTexture(GL_TEXTURE_2D, m_texture_id);
        m_unit = level;
    }

    void Use(){
        glBindTexture(GL_TEXTURE_2D, m_texture_id);
    }

    GLenum GetUnit() const {
        return m_unit;
    }

private:
    std::string m_file_name;
    int m_width;
    int m_height; 
    int m_channel;
    unsigned int m_texture_id;
    GLenum m_unit;
};

