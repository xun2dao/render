#pragma once
#include "../Shader/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Camera {
public:
  Camera(std::string view_name = "view") : m_view_name(view_name) {
    m_direction = glm::vec3(0.0f, 0.0f, 3.0f);
    m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
  }
  Camera(Program &program, std::string viwe_name = "view")
      : m_view_name(viwe_name) {
    m_programs.push_back(program);
    m_direction = glm::vec3(0.0f, 0.0f, 3.0f);
    m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);

    UpdateView();
  }

  void AddProgram(Program &program) {
    m_programs.push_back(program);
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
  void OriginLookLeft(float);
  void OriginLookRight(float);

  glm::vec3 GetPos() const { return m_direction; }

private:
  void UpdateView() {
    glm::mat4 view = glm::lookAt(m_direction, m_front + m_direction, m_up);
    for (auto &program : m_programs)
      program.SetUniformMat4f(m_view_name, glm::value_ptr(view));
  }

private:
  glm::vec3 m_direction;
  glm::vec3 m_front;
  glm::vec3 m_up;
  std::string m_view_name;
  std::vector<Program> m_programs;
};
