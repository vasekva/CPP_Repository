#ifndef Shader_hpp
#define Shader_hpp

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    public:
        // the program ID
        unsigned int ID;
    
        Shader(const char *vertexPath, const char *fragmentPath);
        ~Shader();
    
        void use();
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFLoat(const std::string &name, float value) const;
        void set4f(const std::string &name, float value) const;
    private:
        void checkCompileErrors(unsigned int shader, std::string type);
};

#endif /* Shader_hpp */
