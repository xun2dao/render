#include "Camera.h"
#include <iostream>

void Camera::MoveFront() {
  m_direction += m_front * 0.5f;
  UpdateView();
}

void Camera::MoveLeft() {
  glm::vec3 leftVec = glm::normalize(glm::cross(m_up, m_front));
  m_direction += 0.1f * leftVec;
  UpdateView();
}

void Camera::MoveRight() {
  glm::vec3 rightVec = glm::normalize(glm::cross(m_front, m_up));
  m_direction += 0.1f * rightVec;
  UpdateView();
}

void Camera::MoveBack() {
  m_direction -= m_front * 0.5f;
  UpdateView();
}

void Camera::LookUp(float pitch) {
  float dy = glm::sin(pitch);
  float dz = -glm::cos(pitch);
  m_front += glm::vec3(0.0f, dy, dz);
  UpdateView();
}

void Camera::LookDown(float pitch) { LookUp(-pitch); }
void Camera::LookLeft(float yaw) {
  float dz = -glm::cos(yaw);
  float dx = glm::sin(yaw);
  m_front += glm::vec3(dx, 0.0f, dz);
  UpdateView();
}

void Camera::LookRight(float yaw) { LookLeft(-yaw); }

void Camera::OriginLookLeft(float yaw) {

  auto target = glm::vec3(3.0f, 3.0f, 3.0f) * m_front + m_direction;
  float n_x = m_front.x * glm::cos(yaw) - m_front.z * glm::sin(yaw);
  float n_z = m_front.x * glm::sin(yaw) + m_front.z * glm::cos(yaw);
  m_front = glm::vec3(n_x, m_front.y, n_z);
  m_direction = target - m_front;
  UpdateView();
}
void Camera::OriginLookRight(float yaw) { OriginLookLeft(-yaw); }
