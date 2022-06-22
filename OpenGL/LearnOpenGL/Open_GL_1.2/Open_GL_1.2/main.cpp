#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

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
  
    Shader ourShader("/Users/jberegon/Desktop/CPP_Repository/OpenGL/LearnOpenGL/Open_GL_1.2/Open_GL_1.2/3.3vertex_shader.txt", "/Users/jberegon/Desktop/CPP_Repository/OpenGL/LearnOpenGL/Open_GL_1.2/Open_GL_1.2/3.3fragment_shader.txt");
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // positions            // colors
        0.5f, -0.5f, 0.0f,      1.0f,   0.0f,   0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f,   1.0f,   0.0f,   // bottom left
        0.0f, 0.5f, 0.0f,       0.0f,   0.0f,   1.0f    // top
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0); // 3. then set the vertex attributes pointers
    glEnableVertexAttribArray(0);
    
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    
    
    // render loop
    float yOffset = 0.0f;
    float moveValue = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
//        if (xOffset >= 0.5f || xOffset <= -0.5f)
//            moveValue = -moveValue;
       if (yOffset >= 1.0f || yOffset <= -1.0f)
            moveValue = -moveValue;
        // input
        processInput(window);
        
        // renderging commands here
        // ...

        //render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // be sure to activate the shader before any calls to glUniform
        //glUseProgram(shaderProgram);
        ourShader.use();
        
        double timeValue = glfwGetTime();
        float greenValue = static_cast<float>(sin(timeValue) / 1.0 + 0.5);
        ourShader.set4f("ourColor", greenValue);
        
        yOffset += moveValue;
        ourShader.setFLoat("yOffset", yOffset);
        glBindVertexArray(VAO);
    
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
    //glDeleteProgram(shaderProgram);
    
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
