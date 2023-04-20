#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include"../Shader/Shader.h"
#include<glm/gtc/type_ptr.hpp>

class Camera{
public:
    Camera(std::string view_name= "view"): m_view_name(view_name){
        m_direction = glm::vec3(0.0f, 0.0f, 3.0f);
        m_front = glm::vec3(0.0f, 0.0f, -1.0f);
        m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    Camera(Program& program, std::string viwe_name= "view"): m_program(program), m_view_name(viwe_name){
        m_direction = glm::vec3(0.0f, 0.0f, 3.0f);
        m_front = glm::vec3(0.0f, 0.0f, -1.0f);
        m_up = glm::vec3(0.0f, 1.0f, 0.0f);

        UpdateView();
    }

    void AddProgram(Program& program){
        m_program = program;
        UpdateView();
    }
    void MoveFront();
    void MoveLeft();
    void MoveRight();
    void MoveBack();
    void LookUp(float);
    void LookDown(float);
    void LookLeft(float);
    void LookRight(float);

private:
    void UpdateView(){
        glm::mat4 view = glm::lookAt(m_direction, m_front + m_direction, m_up);
        m_program.SetUniformMat4f(m_view_name, glm::value_ptr(view));
    }
private:
    glm::vec3 m_direction;
    glm::vec3 m_front;
    glm::vec3 m_up;
    std::string m_view_name;
    Program m_program;
};