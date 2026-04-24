
#pragma once
#include <iostream>
#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <freetype/include/ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

struct Character
{
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};

class TextRenderer
{
public:
    std::map<char, Character> Characters;
    Shader textShader;

    TextRenderer();
    TextRenderer(unsigned int width, unsigned int height);
    void resetSize(unsigned int width, unsigned int height);
    void Load(std::string font, unsigned int fontSize);
    void renderText(std::string text, float x, float y, glm::vec2 scale, glm::vec3 color = glm::vec3(1.0f));

private:
    unsigned int VAO, VBO;
};
