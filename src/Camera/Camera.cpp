#include"Camera.h"
#include<iostream>

void Camera::MoveFront(){
    m_direction += m_front*0.5f;
    UpdateView();
}
void Camera::MoveLeft(){
    glm::vec3 leftVec = glm::normalize(glm::cross(m_up, m_front));
    m_direction += 0.1f*leftVec;
    UpdateView();
}
void Camera::MoveRight(){
    glm::vec3 rightVec = glm::normalize(glm::cross(m_front, m_up));
    m_direction += 0.1f*rightVec;
    UpdateView();
}
void Camera::MoveBack(){
    m_direction -= m_front*0.5f;
    UpdateView();
}
void Camera::LookUp(float pitch){

}
void Camera::LookDown(float pitch){

}
void Camera::LookLeft(float yaw){

}
void Camera::LookRight(float yaw){

}