#include "Camera/Camera.h"
#include "EventSystem/Event/KeyEvent.h"
#include "EventSystem/Event/MouseEvent.h"
#include "EventSystem/EventDispatcher.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"
#include "VertexArray/VertexArray.h"
#include "configure/configure.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include "Model/Model.h"
#include <filesystem>

Camera camera;

unsigned int CreateShader(const char *source) {
    unsigned int shader;
    glCreateShader(shader);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    char errorInfo[256];
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 256, nullptr, errorInfo);
        std::cout << errorInfo << std::endl;
    }
    return shader;
}

unsigned int CreateProgram(const char *vertexSource,
                           const char *fragmentSource) {
    unsigned int vertexShader = CreateShader(vertexSource);
    unsigned int fragmentShader = CreateShader(fragmentSource);
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    int success;
    char errorInfo[256];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 256, nullptr, errorInfo);
        std::cout << errorInfo << std::endl;
    }
    return program;
}

void MouseButtonCallbackFunc(GLFWwindow *window, int button, int action,
                             int mods) {
    EventDispatcher *dispatcher =
        (EventDispatcher *)glfwGetWindowUserPointer(window);
    Event *e = new MouseButtonEvent(button);
    dispatcher->PushEvent(e);
}

float yaw = 0;
void KeyPressCallbackFunc(GLFWwindow *window, int key, int scancode, int action,
                          int mods) {
    EventDispatcher *dispatcher =
        (EventDispatcher *)glfwGetWindowUserPointer(window);
    Event *e = new KeyPressEvent(key, 1);
    dispatcher->PushEvent(e);
    KeyPressEvent *event =
        dynamic_cast<KeyPressEvent *>(const_cast<Event *>(e));
    switch (event->GetKey()) {
    case 65:
        camera.MoveLeft();
        break;
    case 68:
        camera.MoveRight();
        break;
    case 83:
        camera.MoveBack();
        break;
    case 87:
        camera.MoveFront();
        break;
    case 81:
        camera.OriginLookRight(3.0f);
        break;
    case 69:
        camera.OriginLookLeft(3.0f);
        break;
    }
}

void ScrollCallbackFunc(GLFWwindow *window, double x, double y) {
    EventDispatcher *dispatcher =
        (EventDispatcher *)glfwGetWindowUserPointer(window);
    Event *e = new MouseScrollEvent(x, y);
    dispatcher->PushEvent(e);
    // 自由移动
}

void CursorPosCallback(GLFWwindow *window, double x, double y) {
    EventDispatcher *dispatcher =
        (EventDispatcher *)glfwGetWindowUserPointer(window);
    Event *e = new MouseCursorPos(x, y);
    dispatcher->PushEvent(e);
    // 缩放
}

void HandleMouseButton(const Event *e) {
    // MouseButtonEvent * e =
    // dynamic_cast<MouseButtonEvent*>(const_cast<Event*>(e));
    // std::cout << e->ToString() << std::endl;
}

void HandleKeyPress(const Event *e) {
    // std::cout << e->ToString() << std::endl;
    KeyPressEvent *event =
        dynamic_cast<KeyPressEvent *>(const_cast<Event *>(e));
}

void HandleMouseScroll(const Event *e) {
    // std::cout << e->ToString() << std::endl;
    MouseScrollEvent *event =
        dynamic_cast<MouseScrollEvent *>(const_cast<Event *>(e));
}

void HandleMouseCursorPos(const Event *e) {
    // std::cout << e->ToString() << std::endl;
    MouseCursorPos *event =
        dynamic_cast<MouseCursorPos *>(const_cast<Event *>(e));
}

int main() {

    if (!glfwInit()) {
        std::cout << "Hello, World" << std::endl;
        return -1;
    }

    GLFWwindow *window =
        glfwCreateWindow(1290, 900, "OpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD load failed!" << std::endl;
    }
    EventDispatcher dispatcher;
    dispatcher.Register(EventType::MouseButton, HandleMouseButton);
    dispatcher.Register(EventType::KeyPress, HandleKeyPress);
    dispatcher.Register(EventType::MouseScroll, HandleMouseScroll);
    dispatcher.Register(EventType::MouseCursorPos, HandleMouseCursorPos);
    glfwSetWindowUserPointer(window, &dispatcher);

    glfwSetMouseButtonCallback(window, MouseButtonCallbackFunc);
    glfwSetKeyCallback(window, KeyPressCallbackFunc);
    glfwSetScrollCallback(window, ScrollCallbackFunc);
    glfwSetCursorPosCallback(window, CursorPosCallback);

    float vertexes[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
        -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
        0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
        1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
        0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
        0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
        -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
        0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
        0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
        -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
        0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
        0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
        -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
        -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
        1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
        -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
        1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f};

    // unsigned int Triangle;
    // glGenVertexArrays(1, &Triangle);
    // glBindVertexArray(Triangle);

    // unsigned int VBO; // 顶点缓冲对象
    // glGenBuffers(1, &VBO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);// 绑定缓冲对象
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes,
    // GL_STATIC_DRAW);
    // // 把数据传送给ARRAY_BUFFER

    // unsigned int VEO;
    // glGenBuffers(1, & VEO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes,
    // GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float),
    // (void*)0);
    // // 告诉GPU如何理解输入的数据 glEnableVertexAttribArray(0);

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // 光的颜色
    glm::vec3 lightPos(3.0f, 0.0f, 3.0f);   // 光的位置
                                            //

    VertexArray light(sizeof(vertexes) / sizeof(float), vertexes);
    light.AddAttrib({GL_FLOAT, 3});
    light.AddAttrib({GL_FLOAT, 3});
    light.AddAttrib({GL_FLOAT, 2});
    light.ParseData();
    Program lightProgram("shader/vertex.shader", "shader/lightFragment.shader");
    lightProgram.SetUniform3f("lightColor", lightColor);
    light.Unuse();
    lightProgram.Unuse();

    VertexArray vao(sizeof(vertexes) / sizeof(float), vertexes);

    vao.AddAttrib({GL_FLOAT, 3});
    vao.AddAttrib({GL_FLOAT, 3});
    vao.AddAttrib({GL_FLOAT, 2});
    vao.ParseData();

    Texture diffMap("assert/wall.jpg", 0);
    Texture diffSpecularMap("assert/container2_specular.png", 1);
    Texture launchMap("assert/matrix.jpg", 2);
    // ConfigureManager configure_manager;

    Program program("shader/vertex.shader", "shader/fragment.shader");
    // program.SetUniform3f("material.diffuse", glm::vec3{1.0f, 0.5f, 0.31f});
    // // 漫反射 program.SetUniform3f("material.ambient", glm::vec3{1.0f, 0.5f,
    // 0.31f}); // 环境光
    program.SetUniform1I("material.diffuse", 0);
    // program.SetUniform3f("material.specular", glm::vec3{0.5f, 0.5f, 0.5f});
    // // 镜面反射光
    program.SetUniform1I("material.specualr", 1);
    program.SetUniform1I("material.launch", 2);

    program.SetUniform1f("material.shininess", 64.0f); // 反光度
    // program.SetUniform3f("lightColor", lightColor);
    program.SetUniform3f("lightPos", lightPos);
    glm::vec3 lightDir = glm::vec3(1.0f, 0.0f, 1.0f);
    program.SetUniform3f("light.lightDir", lightDir);
    // program.SetUniform3f("myColor", glm::vec3{0.5, 0.2, 0.2});
    program.SetUniform3f("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    program.SetUniform3f("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    program.SetUniform3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    program.SetUniform1f("light.constant", 1.0f);
    program.SetUniform1f("light.linear", 0.22f);
    program.SetUniform1f("light.qudratic", 0.20f);
    program.SetUniform3f("_light._lightPos", lightPos);
    program.SetUniform3f("_light._lightDir", glm::vec3(1.0f, 1.0f, 1.0f));

    float line[] = {-0.5f, -0.5f, -0.5f, 3.0f, 0.0f, 3.0f};
    VertexArray showLine(sizeof(line) / sizeof(float), line);
    showLine.AddAttrib({GL_FLOAT, 3});
    showLine.ParseData();
    Program lineProgram("shader/vertex.shader", "shader/lineFragment.shader");

    camera.AddProgram(program);
    camera.AddProgram(lightProgram);
    camera.AddProgram(lineProgram);

    // int width, height, channel;
    // stbi_set_flip_vertically_on_load(true);
    // unsigned char* data = stbi_load("../assert/wall.jpg", &width, &height,
    // &channel, 0); if(!data) std::cout<<"data doesn't exist!"<<std::endl;
    // unsigned int tex;
    // glGenTextures(1, &tex);
    // glBindTexture(GL_TEXTURE_2D, tex);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    // GL_LINEAR_MIPMAP_LINEAR); glTexParameteri(GL_TEXTURE_2D,
    // GL_TEXTURE_MAG_FILTER, GL_LINEAR); glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
    // width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D);
    // stbi_image_free(data);

    glm::mat4 projection;
    // model = glm::rotate(model, -55.0f, glm::vec3(1.0f, 0, 0));
    projection = glm::perspective(45.0f, 800 / 600.0f, 0.1f, 100.0f);

    // program.SetUniformMat4f("model", glm::value_ptr(model));
    program.SetUniformMat4f("projection", glm::value_ptr(projection));
    lightProgram.SetUniformMat4f("projection", glm::value_ptr(projection));
    lineProgram.SetUniformMat4f("projection", glm::value_ptr(projection));
    glEnable(GL_DEPTH_TEST);

    glm::vec3 box_pos[] = {glm::vec3{1.0f, 1.0, 1.0f},
                           glm::vec3{1.0f, 0.0, -1.0}, glm::vec3{0.5, 1.0, -1.0}

    };

    float x = 0.0f, y = 1.0f;

    Program modelProgram("shader/modelVertex.shader",
                         "shader/modelFragment.shader");

    char *modelPath = "assert/model/nanosuit.obj";
    Model item(modelPath);
    camera.AddProgram(modelProgram);
    glm::mat4 modelMat;
    modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f));
    modelProgram.SetUniformMat4f("model", glm::value_ptr(modelMat));
    modelProgram.SetUniformMat4f("projection", glm::value_ptr(projection));
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0.2f, 0.1f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /*light.Use();        // 数据和解释
        lightProgram.Use(); // shader : 变换

        glm::mat4 model;
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2));
        lightProgram.SetUniformMat4f("model", glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        showLine.Use();
        lineProgram.Use();
        lineProgram.SetUniformMat4f("model", glm::value_ptr(model));
        glDrawArrays(GL_LINES, 0, 2);

        x = glm::cos(glfwGetTime());
        y = glm::sqrt(1 - x * x);
        vao.Use();
        program.Use();
        program.SetUniform3f("viewPos", camera.GetPos());
        program.SetUniform3f("lightColor", lightColor);
        program.SetUniform1f("_light._cutOff", glm::cos(0.5f));
        program.SetUniform3f("_light._lightDir", glm::vec3(x, y, -1.0f));

        for (int i = 0; i < 3; ++i) {
            glm::mat4 model;
            model = glm::translate(model, box_pos[i]);
            program.SetUniformMat4f("model", glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        */

        modelProgram.Use();
        item.Draw(modelProgram);

        // std::cout<<"Tick"<<std::endl;
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
