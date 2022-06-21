//
//  main.cpp
//  Open_GL_1.0
//
//  Created by Josephine Beregond on 6/20/22.
//

#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n" // shape's vertexes
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n" // shape's color
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";

enum SHADER_TYPE
{
    VERTEX = 0, // GL_VERTEX_SHADER
    FRAGMENT = 1, // GL_FRAGMENT_SHADER
};

bool create_window(GLFWwindow **window)
{
    (*window) = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if ((*window) == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return (false);
    }
    glfwMakeContextCurrent((*window));
    
    // !!! called a function framebuff... every time the window is resized
    glfwSetFramebufferSizeCallback((*window), framebuffer_size_callback);
    
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return (false);
    }
    return (true);
}

bool create_shader(SHADER_TYPE TYPE, unsigned int &shader_ref)
{
    int success;
    char infoLog[512];

    if (TYPE != 0 && TYPE != 1)
    {
        std::cout << "INCORRECT SHADER_TYPE!\n" << infoLog << std::endl;
        return (false);
    }
    
    if (TYPE == 0)
    {
        std::cout << "VERTEX" << infoLog << std::endl;
        shader_ref = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shader_ref, 1, &vertexShaderSource, NULL);
    }
    else if (TYPE == 1)
    {
        std::cout << "fragmentShaderYellow" << infoLog << std::endl;
        shader_ref = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shader_ref, 1, &fragmentShaderSource, NULL);
    }
    
    glCompileShader(shader_ref);
    glGetShaderiv(shader_ref, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_ref, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return (false);
    }
    return (true);
}

int main(int argc, const char **argv)
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for macOS
    #endif
  
    // glfw window creation
    GLFWwindow *window = nullptr;
    if (create_window(&window) == false)
    {
        return (-1);
    }
    
    unsigned int vertexShader = 0;
    unsigned int fragmentShader = 0;
    if (create_shader(VERTEX, vertexShader) == false)
        return (-1);
    if (create_shader(FRAGMENT, fragmentShader) == false)
        return (-1);
    
    //link shaders
    int success;
    char infoLog[512];
    // link the first program object
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    float triangle[] = {
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    
    // VBO - vertex buffer object
    // VAO - vertex array object
    // EBO - element buffer object
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO); // 1. bind Vertex Array object
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 2. copy our vertices array in a buffer for OpenGL to use
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0); // 3. then set the vertex attributes pointers
    glEnableVertexAttribArray(0);
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);
    
    // bind the VAO (it was already bound, but just to demonstrate): seeing as we only have a single VAO we can
    // just bind it beforehand before rendering the respective triangle; this is another approach.
    glBindVertexArray(VAO);
    
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        
        // renderging commands here
        // ...

        //render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // be sure to activate the shader before any calls to glUniform
        glUseProgram(shaderProgram);
        
        //update shader uniform
        double timeValue = glfwGetTime();
        float greenValue = static_cast<float>(sin(timeValue) / 2.0 + 0.5);
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        
        // render the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    
    // glfw: terminate, clearing all previously GLFW resources.
    glfwTerminate();
    return (0);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be signigicantly larger than specified on retina displays
    glViewport(0, 0, width, height);
}
