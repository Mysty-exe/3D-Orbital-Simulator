#include <UI/UIRenderer.h>

UIRenderer::UIRenderer()
{
}

UIRenderer::UIRenderer(float WIDTH, float HEIGHT) : WIDTH(WIDTH), HEIGHT(HEIGHT)
{
    float vertices[] = {
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3};

    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);
    glGenBuffers(1, &rectEBO);

    glBindVertexArray(rectVAO);

    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    rectShader = Shader("shaders/UI/rect.vert", "shaders/UI/rect.frag", "");

    rectShader.use();
    rectShader.setMat4("projection", glm::ortho(0.0f, WIDTH, HEIGHT, 0.0f));
}

UIRenderer::~UIRenderer()
{
    glDeleteVertexArrays(1, &rectVAO);
    glDeleteBuffers(1, &rectVBO);
    glDeleteBuffers(1, &rectEBO);
}

void UIRenderer::drawRect(const UIRect &rect, glm::vec2 scale)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(rect.getX(), rect.getY() - rect.getScroll(), 0.0f));
    model = glm::scale(model, glm::vec3(rect.getWidth() * scale.x, rect.getHeight() * scale.y, 1.0f));

    rectShader.use();
    rectShader.setMat4("model", model);
    rectShader.setVec4("color", rect.getColor());
    rectShader.setVec2("rectSize", glm::vec2(rect.getWidth() * scale.x, rect.getHeight() * scale.y));
    rectShader.setFloat("cornerRadius", rect.getCornerRadius());
    rectShader.setBool("image", false);

    glBindVertexArray(rectVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void UIRenderer::drawTextField(UITextField &textField, glm::vec2 scale)
{
    textField.getTitle().renderText();
    float cursorHeight = textField.getTexts()[textField.getTexts().size() - 1].getHeight() + 5;

    for (int i = 0; i < textField.getTexts().size(); i++)
    {
        if (i == 0 && textField.getTexts()[i].getText().size() == 0)
            break;

        textField.getTexts()[i].renderText();
        if (i + 1 == textField.getTexts().size() && textField.getActivated() && textField.getCursorOn())
            drawCursor(glm::vec2(textField.getTexts()[i].getEndX() + 2, textField.getTexts()[i].getEndY() - cursorHeight - textField.getScroll()), cursorHeight);
    }

    textField.getUnits().renderText();
    textField.getError().renderText();

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(textField.getFieldX(), textField.getFieldY() - textField.getScroll(), 0.0f));
    model = glm::scale(model, glm::vec3(textField.getFieldWidth() * scale.x, textField.getFieldHeight() * scale.y, 1.0f));

    rectShader.use();
    rectShader.setMat4("model", model);
    rectShader.setVec4("color", glm::vec4(0.0f));
    rectShader.setVec2("rectSize", glm::vec2(textField.getFieldWidth() * scale.x, textField.getFieldHeight() * scale.y));
    rectShader.setFloat("cornerRadius", textField.getCornerRadius());
    rectShader.setBool("image", false);

    glBindVertexArray(rectVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);

    float outline = 5.0f * scale.x;
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(textField.getFieldX() - outline, textField.getFieldY() - textField.getScroll() - outline, 0.0f));
    model = glm::scale(model, glm::vec3(textField.getFieldWidth() * scale.x + outline * 2.0f, textField.getFieldHeight() * scale.y + outline * 2.0f, 1.0f));

    rectShader.setMat4("model", model);
    if (textField.getActivated())
        rectShader.setVec4("color", textField.getActivatedOutlineColor());
    else
        rectShader.setVec4("color", textField.getOutlineColor());
    rectShader.setVec2("rectSize", glm::vec2(textField.getFieldWidth() * scale.x + outline * 2.0f, textField.getFieldHeight() * scale.y + outline * 2.0f));
    rectShader.setFloat("cornerRadius", textField.getCornerRadius());

    glBindVertexArray(rectVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
}

void UIRenderer::drawLine(float width, glm::vec2 pos)
{
    UIRect line = UIRect(glm::vec2(width, 2), pos, glm::vec4(1.0f));
    drawRect(line);
}

void UIRenderer::drawCursor(glm::vec2 pos, float height)
{
    UIRect line = UIRect(glm::vec2(3, height), pos, glm::vec4(1.0f));
    drawRect(line);
}

void UIRenderer::drawImage(const UIImage &image, glm::vec2 scale)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(image.getX(), image.getY() - image.getScroll(), 0.0f));
    model = glm::scale(model, glm::vec3(image.getWidth() * scale.x, image.getHeight() * scale.y, 1.0f));

    rectShader.use();
    rectShader.setMat4("model", model);
    rectShader.setBool("image", true);
    rectShader.setInt("tex1", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, image.getTexture());

    glBindVertexArray(rectVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void UIRenderer::drawIcon(UIIcon &icon, glm::vec2 scale)
{
    icon.getText().renderText();
    drawRect(icon.getRect());
    drawImage(icon.getImage());
}

void UIRenderer::drawPanel(UIPanel &panel, glm::vec2 scale)
{
    drawRect(panel);

    std::vector<UIText> title = panel.getTitle();

    if (panel.getIcon() != nullptr)
        drawIcon(*panel.getIcon());

    for (int i = 0; i < title.size(); i++)
        title[i].renderText();

    if (panel.getButton() != nullptr)
    {
        drawIcon(*panel.getButton());
        panel.getButtonText().renderText();
    }

    if (panel.canScroll())
    {
        int y = title[title.size() - 1].getEndY() + 20;
        glScissor(panel.getX(), HEIGHT - (y + (panel.getHeight() - y)), panel.getWidth(), panel.getHeight() - y);
        glEnable(GL_SCISSOR_TEST);
    }

    for (auto &rect : panel.getRects())
    {
        drawRect(*(rect.get()));

        if (panel.isLines())
            drawLine(panel.getWidth() - 50, glm::vec2(panel.getX() + 25, rect->getY() - 10 - rect->getScroll()));

        if (rect->getIcon() != nullptr)
            drawIcon(*rect->getIcon());

        for (auto &title : rect->getTitle())
            title.renderText();

        for (auto &text : rect->getTexts())
            text.renderText();

        glClear(GL_STENCIL_BUFFER_BIT);
        for (auto &textField : rect->getTextFields())
            drawTextField(*textField);

        for (auto &icon : rect->getIcons())
            drawIcon(*(icon.get()));
    }

    for (auto &imageRect : panel.getImageRects())
    {
        drawRect(*(imageRect));
    }

    for (UIText &text : panel.getTexts())
        text.renderText();

    glDisable(GL_SCISSOR_TEST);
}
