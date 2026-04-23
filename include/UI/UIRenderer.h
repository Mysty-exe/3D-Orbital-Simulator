#pragma once
#include <glad/glad.h>
#include "UI/UINodes.h"
#include "Rendering/Shader.h"

class UIRenderer
{
private:
    unsigned int rectVAO, rectVBO, rectEBO;
    Shader rectShader;

    float WIDTH, HEIGHT;

public:
    UIRenderer();
    ~UIRenderer();
    UIRenderer(float WIDTH, float HEIGHT);
    void drawRect(const UIRect &rect, glm::vec2 scale = glm::vec2(1, 1));
    void drawTextField(UITextField &textField, glm::vec2 scale = glm::vec2(1, 1));
    void drawLine(float width, glm::vec2 pos);
    void drawCursor(glm::vec2 pos, float height);
    void drawImage(const UIImage &image, glm::vec2 scale = glm::vec2(1, 1));
    void drawIcon(UIIcon &icon, glm::vec2 scale = glm::vec2(1, 1));
    void drawPanel(UIPanel &panel, glm::vec2 scale = glm::vec2(1, 1));
};
