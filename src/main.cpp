#include<iostream>
#include<string>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Shader/Shader.h"
#include"configure/configure.h"
#include"VertexArray/VertexArray.h"
#include"Texture/Texture.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"EventSystem/Event/MouseEvent.h"
#include"EventSystem/Event/KeyEvent.h"
#include"EventSystem/EventDispatcher.h"
#include"Camera/Camera.h"


Camera camera;

unsigned int CreateShader(const char* source){
    unsigned int shader;
    glCreateShader(shader);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    char errorInfo[256];
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shader, 256, nullptr, errorInfo);
        std::cout<< errorInfo<<std::endl;
    }
    return shader;
}

unsigned int CreateProgram(const char* vertexSource, const char* fragmentSource){
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
    if(!success){
        glGetProgramInfoLog(program, 256, nullptr, errorInfo);
        std::cout<< errorInfo<<std::endl;
    }
    return program;
}

void MouseButtonCallbackFunc(GLFWwindow* window, int button, int action, int mods){
    EventDispatcher* dispatcher = (EventDispatcher*)glfwGetWindowUserPointer(window);
    Event* e = new MouseButtonEvent(button);
    dispatcher->PushEvent(e);
    
}

void KeyPressCallbackFunc(GLFWwindow* window, int key, int scancode, int action, int mods){
    EventDispatcher* dispatcher = (EventDispatcher*)glfwGetWindowUserPointer(window);
    Event* e = new KeyPressEvent(key, 1);
    dispatcher->PushEvent(e);
    KeyPressEvent* event = dynamic_cast<KeyPressEvent*>(const_cast<Event*>(e));
    switch(event->GetKey()){
        case 65:
            camera.MoveLeft();
        break;
        case 68:
            camera.MoveRight();
        break;
        case 83:
            camera.MoveFront();
        break;
        case 87:
            camera.MoveBack();
        break;
    }
}

void ScrollCallbackFunc(GLFWwindow* window, double x, double y){
    EventDispatcher* dispatcher = (EventDispatcher*)glfwGetWindowUserPointer(window);
    Event* e = new MouseScrollEvent(x, y);
    dispatcher->PushEvent(e);
    // 自由移动

}

void CursorPosCallback(GLFWwindow* window, double x, double y){
    EventDispatcher* dispatcher = (EventDispatcher*)glfwGetWindowUserPointer(window);
    Event* e = new MouseCursorPos(x, y);
    dispatcher->PushEvent(e);
    // 缩放
}


void HandleMouseButton(const Event* e){ 
    //MouseButtonEvent * e = dynamic_cast<MouseButtonEvent*>(const_cast<Event*>(e));
    std::cout<< e->ToString()<<std::endl;
}

void HandleKeyPress(const Event* e){
    std::cout<<e->ToString()<<std::endl;
    KeyPressEvent* event = dynamic_cast<KeyPressEvent*>(const_cast<Event*>(e));
}

void HandleMouseScroll(const Event* e){
    std::cout<< e->ToString()<<std::endl;
    MouseScrollEvent* event = dynamic_cast<MouseScrollEvent*>(const_cast<Event*>(e));
}

void HandleMouseCursorPos(const Event* e){
    std::cout<< e->ToString()<<std::endl;
    MouseCursorPos* event = dynamic_cast<MouseCursorPos*>(const_cast<Event*>(e));
}



int main(){


    if(!glfwInit()){
        std::cout<< "Hello, World"<<std::endl;
        return -1;

    }


    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"GLAD load failed!"<<std::endl;
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
        0.5, 0, 0, 1.0, 0, 0, 1, 1,
        0, 0.5, 0, 0, 1.0, 0, 0, 0.5,
        -0.5, 0, 0, 0, 0, 1.0, 0, 0,
        0, -0.5, 0, 0, 1.0, 0, 0, 0.5
    };

    unsigned int indexes[] = {
        0, 1, 2,
        0, 2, 3
    };

    // unsigned int Triangle;
    // glGenVertexArrays(1, &Triangle);
    // glBindVertexArray(Triangle);

    // unsigned int VBO; // 顶点缓冲对象
    // glGenBuffers(1, &VBO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);// 绑定缓冲对象
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW); // 把数据传送给ARRAY_BUFFER

    // unsigned int VEO;
    // glGenBuffers(1, & VEO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0); // 告诉GPU如何理解输入的数据
    // glEnableVertexAttribArray(0);

    

    VertexArray vao(sizeof(vertexes)/sizeof(float), vertexes, 6, indexes);
    vao.AddAttrib({GL_FLOAT, 3});
    vao.AddAttrib({GL_FLOAT, 3});
    vao.AddAttrib({GL_FLOAT, 2});
    vao.ParseData();


    //ConfigureManager configure_manager;

    Program program("../shader/vertex.shader", "../shader/fragment.shader");

    camera.AddProgram(program);

    // int width, height, channel;
    // stbi_set_flip_vertically_on_load(true);
    // unsigned char* data = stbi_load("../assert/wall.jpg", &width, &height, &channel, 0);
    // if(!data) std::cout<<"data doesn't exist!"<<std::endl;
    // unsigned int tex;
    // glGenTextures(1, &tex);
    // glBindTexture(GL_TEXTURE_2D, tex);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D);
    // stbi_image_free(data);
    Texture tex("../assert/wall.jpg");
    Texture tex1("../assert/awesomeface.png", 1);
    program.SetUniform1i("tex1", tex1.GetUnit());


    glm::mat4 model, projection;
    model = glm::rotate(model, -55.0f, glm::vec3(1.0f, 0, 0));
    projection = glm::perspective(45.0f, 800/600.0f, 0.1f, 100.0f);
    
    program.SetUniformMat4f("model", glm::value_ptr(model));
    program.SetUniformMat4f("projection", glm::value_ptr(projection));


    while(!glfwWindowShouldClose(window)){
        glClearColor(0.0,0.3,0.1,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        vao.Use();
        //glBindVertexArray(Triangle);
        //program.SetUniform4f("globalColor", red, 2.0, 1.0, 1.0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        //std::cout<<"Tick"<<std::endl;
        glfwPollEvents();
        glfwSwapBuffers(window);
        
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}