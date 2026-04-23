#include "Rendering/Shader.h"

Shader::Shader()
{
}

Shader::Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath)
{
    std::string vertexCode, fragmentCode, geometryCode, line;

    std::ifstream vertexShaderFile(vertexPath);
    std::ifstream fragmentShaderFile(fragmentPath);
    std::ifstream geometryShaderFile(geometryPath);

    while (getline(vertexShaderFile, line))
    {
        vertexCode += line;
        vertexCode += "\n";
    }

    vertexShaderFile.close();

    while (getline(fragmentShaderFile, line))
    {
        fragmentCode += line;
        fragmentCode += "\n";
    }

    fragmentShaderFile.close();

    while (getline(geometryShaderFile, line))
    {
        geometryCode += line;
        geometryCode += "\n";
    }

    geometryShaderFile.close();

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();

    unsigned int vertexShader, fragmentShader, geometryShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    ID = glCreateProgram();

    int success;
    char infoLog[512];

    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::" << vertexPath << "::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::" << fragmentPath << "::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    if (!geometryCode.empty())
    {
        glShaderSource(geometryShader, 1, &gShaderCode, NULL);
        glCompileShader(geometryShader);

        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::GEOMETRY::" << geometryPath << "::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }
    }

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    if (!geometryCode.empty())
    {
        glAttachShader(ID, geometryShader);
    }

    glLinkProgram(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);
}

void Shader::use()
{
    glUseProgram(ID);
}

unsigned int Shader::getID()
{
    return ID;
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, glm::vec2 value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string &name, glm::vec4 value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

float Shader::getFloat(const std::string &name) const
{
    GLfloat value;
    glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &value);
    return value;
}
