#pragma once
#include<vector>
#include<iostream>
#include<glad/glad.h>


template<typename T>
class Buffer{
public:
    Buffer(){}
    Buffer(GLenum type, T* data, unsigned int count) :m_type(type), m_data(data),m_count(count){
        glGenBuffers(1, &m_buffer_id);
        glBindBuffer(type, m_buffer_id);
    
        glBufferData(type, sizeof(T)*count, data, GL_STATIC_DRAW);
    }
    Buffer(GLenum type) :m_type(type){
        glGenBuffers(1, &m_buffer_id);
        glBindBuffer(type, m_buffer_id);
    }
    Buffer(const Buffer& other){
        other.m_type = m_type;
        other.m_data = m_data;
        other.m_count = m_count;
        other.m_buffer_id = m_buffer_id;
        m_data = nullptr;
        m_count = 0;
        m_buffer_id = 0;
    }


    void TransferData(T* data, unsigned int count){
        m_data = data;
        m_count = count;
        glBindBuffer(m_type, m_buffer_id);
        glBufferData(type, size(T)*count, data, GL_STATIC_DRAW);

    }
    void Use(){
        glBindBuffer(m_type, m_buffer_id);
    }
    const unsigned int Size() const{return m_count * sizeof(T);}
    const unsigned int Count() const {return m_count;}
private:
    GLenum m_type;
    T * m_data;
    unsigned int m_count;
    unsigned int m_buffer_id;
};

struct VertexAttrib{
        GLenum type = GL_FLOAT;
        unsigned int count;
};

class VertexArray{

public:
    VertexArray(unsigned int count, float* data,  unsigned int point_nums = 0, unsigned int * index = nullptr){
        glGenVertexArrays(1, &m_vao_id);
        glBindVertexArray(m_vao_id);

        Buffer<float> vbo(GL_ARRAY_BUFFER, data, count);
        m_vbo = vbo;
        if(index){
            Buffer<unsigned int> veo(GL_ELEMENT_ARRAY_BUFFER, index, point_nums);
            m_veo = veo;
        }
    }


    void AddAttrib(VertexAttrib attrib){
        m_attribs.push_back(attrib);
        m_stride += attrib.count * sizeof(float);
    }

    void ParseData(){
        unsigned int offset = 0;
        for(int i = 0; i < m_attribs.size(); ++i){
            glVertexAttribPointer(i, m_attribs[i].count, m_attribs[i].type, GL_FALSE, m_stride, (void*)offset);
            offset += m_attribs[i].count * sizeof(float);
            glEnableVertexAttribArray(i);
        }
        
    }

    void Use(){
        glBindVertexArray(m_vao_id);
        
    }

    void Unuse(){
        glBindVertexArray(0);
        
    }
private:
    std::vector<VertexAttrib> m_attribs;   
    Buffer<float> m_vbo;
    Buffer<unsigned int> m_veo;  
    unsigned int m_stride = 0;
    unsigned int m_vao_id;
};