#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <glad/glad.h>
#include <glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Shader
{
private:
    unsigned int ID;

public:
    Shader();
    Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath);

    void use();
    unsigned int getID();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, glm::vec2 value) const;
    void setVec3(const std::string &name, glm::vec3 value) const;
    void setVec4(const std::string &name, glm::vec4 value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;
    float getFloat(const std::string &name) const;
};
