#include<iostream>
#include<string>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Shader/Shader.h"
#include"configure/configure.h"
#include"VertexArray/VertexArray.h"
#include<stb_image.h>

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

    int width, height, channel;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("../assert/wall.jpg", &width, &height, &channel, 0);
    if(!data) std::cout<<"data doesn't exist!"<<std::endl;
    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    

    float red = 0;
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.0,0.3,0.1,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        vao.Use();
        //glBindVertexArray(Triangle);
        program.Use();
        red = std::sin(glfwGetTime());
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