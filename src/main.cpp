#include<iostream>
#include<string>
#include<glad/glad.h>
#include<GLFW/glfw3.h>


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
        0.5, 0, 0, 
        0, 0.5, 0,
        -0.5, 0, 0,
        0, -0.5, 0
    };

    unsigned int indexes[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int Triangle;
    glGenVertexArrays(1, &Triangle);
    glBindVertexArray(Triangle);
    unsigned int VBO; // 顶点缓冲对象
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);// 绑定缓冲对象
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW); // 把数据传送给ARRAY_BUFFER

    unsigned int VEO;
    glGenBuffers(1, & VEO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0); // 告诉GPU如何理解输入的数据
    glEnableVertexAttribArray(0);
    

    const char* vertexSource = "#version 420 core\n\
                                layout(location = 0) in vec3 aPos;\n\
                                void main(){\n\
                                gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n\
                                }\0";

    const char* fragmentSource = "#version 420 core\n\
                                 out vec4 FragColor;\n\
                                 void main(){\n\
                                    FragColor = vec4(1.0, 0.5, 0.1, 1.0);\n\
                                 }\0";
    
    

    unsigned int program = CreateProgram(vertexSource, fragmentSource);
    glUseProgram(program);

    


    while(!glfwWindowShouldClose(window)){
        glClearColor(0.0,0.3,0.1,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(Triangle);
        glUseProgram(program);
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