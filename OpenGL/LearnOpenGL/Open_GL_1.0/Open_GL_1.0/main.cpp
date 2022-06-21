//
//  main.cpp
//  Open_GL_1.0
//
//  Created by Josephine Beregond on 6/20/22.
//

#include <iostream>

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
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShader1Source = "#version 330 core\n" // shape's color
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
const char *fragmentShader2Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";


enum SHADER_TYPE
{
    VERTEX = 0, // GL_VERTEX_SHADER
    FRAGMENT_YELLOW = 1, // GL_FRAGMENT_SHADER
    FRAGMENT_ORANGE = 2, // GL_FRAGMENT_SHADER,
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

    if (TYPE != 0 && TYPE != 1 && TYPE != 2)
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
        glShaderSource(shader_ref, 1, &fragmentShader2Source, NULL);
    }
    else if (TYPE == 2)
    {
        std::cout << "fragmentShaderOrange" << infoLog << std::endl;
        shader_ref = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shader_ref, 1, &fragmentShader1Source, NULL);
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
    unsigned int fragmentShaderOrange = 0;
    unsigned int fragmentShaderYellow = 0;
    if (create_shader(VERTEX, vertexShader) == false)
        return (-1);
    if (create_shader(FRAGMENT_ORANGE, fragmentShaderOrange) == false)
        return (-1);
    if (create_shader(FRAGMENT_YELLOW, fragmentShaderYellow) == false)
        return (-1);

    //link shaders
    int success;
    char infoLog[512];
    // link the first program object
    unsigned int shaderProgramOrange = glCreateProgram();
    glAttachShader(shaderProgramOrange, vertexShader);
    glAttachShader(shaderProgramOrange, fragmentShaderOrange);
    glLinkProgram(shaderProgramOrange);

    //then link the second program object using a different fragment shader (but same vertex shader)
    //this is perfectly allowed since the inputs and outputs of both the vertex and fragment shaders
    //are equally matched.
    unsigned int shaderProgramYellow = glCreateProgram();
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);

    // check for linking errors
    glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    float firstTriangle[] = {
        -0.9f, -0.5f, 0.0f,  // left
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top
    };
    float secondTriangle[] = {
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top
    };
    
    // VBO - vertex buffer object
    // VAO - vertex array object
    // EBO - element buffer object
    unsigned int VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    // first triangle setup
    // --------------------
    glBindVertexArray(VAOs[0]); // 1. bind Vertex Array object
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]); // 2. copy our vertices array in a buffer for OpenGL to use
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0); // 3. then set the vertex attributes pointers
    glEnableVertexAttribArray(0);
    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);
    
    
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
        
        //draw triangles
        glUseProgram(shaderProgramOrange);
        glBindVertexArray(VAOs[0]); // 4. use our shader program when we want to render an object
        glDrawArrays(GL_TRIANGLES, 0, 3); // 5. now draw the object
        
        glUseProgram(shaderProgramYellow);
        glBindVertexArray(VAOs[1]); // 4. use our shader program when we want to render an object
        glDrawArrays(GL_TRIANGLES, 0, 3); // 5. now draw the object
        
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    
    glDeleteShader(vertexShader);
    glDeleteShader(shaderProgramOrange);
    glDeleteShader(shaderProgramYellow);
    
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgramOrange);
    glDeleteProgram(shaderProgramYellow);
    
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
