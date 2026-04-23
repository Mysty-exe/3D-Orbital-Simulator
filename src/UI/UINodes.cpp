#include <UI/UINodes.h>

UINode::UINode() {}
UINode::UINode(glm::vec2 size, glm::vec2 pos) : width(size.x), height(size.y), x(pos.x), y(pos.y) {}

UIRect::UIRect() : UINode(glm::vec2(0, 0), glm::vec2(0, 0)), color(glm::vec4(0, 0, 0, 0)), icon(nullptr), regColor(color), cornerRadius(0.15f), contentWidth(20.0f), contentHeight(20.0f), scroll(0.0f) {}
UIRect::UIRect(glm::vec4 color) : UINode(glm::vec2(0, 0), glm::vec2(0, 0)), icon(nullptr), color(color), regColor(color), cornerRadius(0.15f), contentWidth(20.0f), contentHeight(20.0f), scroll(0.0f) {}
UIRect::UIRect(glm::vec2 size, glm::vec2 pos, glm::vec4 color) : UINode(size, pos), icon(nullptr), color(color), regColor(color), cornerRadius(0.15f), contentWidth(20.0f), contentHeight(20.0f), scroll(0.0f) {}
void UIRect::setPos(glm::vec2 pos)
{
    this->x = pos.x, this->y = pos.y;
    if (icon != nullptr)
        icon->setPos(pos + glm::vec2(10, 10));
}
void UIRect::addIcon(std::string path, glm::vec2 size)
{
    icon = nullptr;
    icon = std::make_unique<UIIcon>(path, size, glm::vec2(x + 10, y + 10), glm::vec4(0.0f));
    icon->setScroll(scroll);
}
void UIRect::setIcon(std::unique_ptr<UIIcon> icon)
{
    icon = nullptr;
    icon->setScroll(scroll);
    this->icon = std::move(icon);
}
void UIRect::addBodyIcon(std::unique_ptr<UIIcon> i)
{
    i->setScroll(scroll);
    float h = i->getHeight();
    icons.push_back(std::move(i));
    height += h + 10;
    contentHeight += h + 10;
}
void UIRect::setTitle(UIText &text)
{
    title.clear();
    if (wrapWidth)
    {
        contentWidth = (contentWidth < text.getWidth()) ? text.getWidth() : contentWidth;
        width = contentWidth + 30;
    }
    else
    {
        int x = (icon != nullptr) ? icon->getWidth() + 25 : 0;
        if (text.getWidth() + x > width * 0.9)
        {
            UIText temp = text;
            std::string currentLine;

            for (char c : text.getText())
            {
                std::string testLine = currentLine + c;
                temp.setText(testLine);

                if (temp.getWidth() + x > width * 0.9)
                {
                    temp.setText(currentLine);
                    contentHeight += temp.getHeight() + 10;
                    title.push_back(temp);

                    currentLine = std::string(1, c);
                }
                else
                    currentLine = testLine;
            }

            if (!currentLine.empty())
            {
                temp.setText(currentLine);
                contentHeight += temp.getHeight() + 10;
                title.push_back(temp);
            }

            if (wrapHeight)
                height = contentHeight + 20;

            return;
        }
    }

    contentHeight += text.getHeight() + 15;
    if (wrapHeight)
    {
        height = contentHeight;
    }

    title.push_back(text);
}
void UIRect::addText(UIText &text)
{
    if (wrapWidth)
    {
        contentWidth = (contentWidth < text.getWidth()) ? text.getWidth() : contentWidth;
        width = contentWidth + 30;
    }
    else
    {
        if (text.getWidth() > width * 0.9)
        {
            UIText temp = text;
            std::string currentLine;

            for (char c : text.getText())
            {
                std::string testLine = currentLine + c;
                temp.setText(testLine);

                if (temp.getWidth() > width * 0.9)
                {
                    temp.setText(currentLine);
                    contentHeight += temp.getHeight() + 10;
                    texts.push_back(temp);

                    currentLine = std::string(1, c);
                }
                else
                    currentLine = testLine;
            }

            if (!currentLine.empty())
            {
                temp.setText(currentLine);
                contentHeight += temp.getHeight() + 10;
                texts.push_back(temp);
            }

            if (wrapHeight)
                height = contentHeight + 20;

            return;
        }
    }

    contentHeight += text.getHeight() + 20;
    if (wrapHeight)
    {
        height = contentHeight + 20;
    }

    texts.push_back(text);
}
void UIRect::addTextField(std::unique_ptr<UITextField> textField)
{
    textField->setFieldWidth(width - 20);
    textField->setPositions();
    textField->setScroll(scroll);
    contentHeight += textField->getTitle().getHeight() + 10 + textField->getHeight() + 10;
    height = contentHeight + 20;

    textFields.push_back(std::move(textField));
}
void UIRect::setPositions()
{
    for (int i = 0; i < title.size(); i++)
    {
        if (i != 0)
            title[i].setPos(x + 20, title[i - 1].getEndY() + 10, scroll);
        else
        {
            if (icon != nullptr)
            {
                title[i].setPos(icon->getEndX() + 20, y + 20, scroll);
                icon->setY(title[i].getY() + (title[i].getHeight() / 2) - (icon->getHeight() / 2) + 5);
            }
            else
                title[i].setPos(x + 20, y + 20, scroll);
        }
    }

    for (int i = 0; i < texts.size(); i++)
    {
        if (i != 0)
            texts[i].setPos(x + 20, texts[i - 1].getEndY() + 15, scroll);
        else
        {
            if (title.size() > 0)
                texts[i].setPos(x + 20, title[title.size() - 1].getEndY() + 30, scroll);
            else
                texts[i].setPos(x + 20, y + 20, scroll);
        }
    }

    for (int i = 0; i < textFields.size(); i++)
    {
        if (i != 0)
        {
            textFields[i]->getTitle().setPos(x + 10, textFields[i - 1]->getEndY() + 30, scroll);
            textFields[i]->setPos(glm::vec2(x + 10, textFields[i - 1]->getEndY() + 30));
        }
        else
        {
            textFields[i]->getTitle().setPos(x + 10, y, scroll);
            textFields[i]->setPos(glm::vec2(x + 10, y));
        }

        for (int j = 0; j < textFields[i]->getTexts().size(); j++)
        {
            if (j == 0)
                textFields[i]->getTexts()[j].setPos(x + 20, textFields[i]->getFieldY() + 20, scroll);
            else
                textFields[i]->getTexts()[j].setPos(x + 20, textFields[i]->getTexts()[j - 1].getEndY() + 10, scroll);
        }

        textFields[i]->getUnits().setPos(textFields[i]->getFieldX() + textFields[i]->getFieldWidth() + 20, textFields[i]->getFieldY() + (textFields[i]->getFieldHeight() / 2) - textFields[i]->getUnits().getHeight() / 2, scroll);
        textFields[i]->getError().setPos(x + 10, textFields[i]->getFieldY() + textFields[i]->getFieldHeight() + 20, scroll);
    }

    for (int i = 0; i < icons.size(); i++)
    {
        if (i != 0)
            icons[i]->setPos(glm::vec2(x + 20, icons[i - 1]->getEndY() + 10));
        else
            icons[i]->setPos(glm::vec2(x + 20, texts[texts.size() - 1].getEndY() + 20));
    }
}
void UIRect::clearText()
{
    icon = nullptr;
    title.clear();
    texts.clear();
    textFields.clear();
    icons.clear();
    contentHeight = 0;
    contentWidth = 0;
}

UITextField::UITextField() {}
UITextField::UITextField(TextFieldHelper helper, glm::vec2 size, glm::vec2 pos, UIText units, glm::vec4 color) : UINode(size, pos), title(helper.title), text(helper.text), error(helper.error), units(units), outlineColor(color), activatedOutlineColor(glm::vec4(0.7, 0.7, 0.7, 1.0)), activated(helper.activated), cursorOn(helper.cursorOn)
{
    rect = UIRect(size, pos, outlineColor);
    rect.setWrap(false, true);
    rect.setCornerRadius(0.0);
}
void UITextField::setPositions()
{
    UIText temp = text;

    texts.clear();
    rect.setHeight(text.getHeight() + 30);

    if (temp.getText().empty() || temp.setText("PLACEHOLDER").getHeight() < text.getHeight())
        rect.setHeight(temp.setText("PLACEHOLDER").getHeight() + 30);

    if (!units.getText().empty())
        setFieldWidth(rect.getWidth() - units.getWidth() - 20);

    float maxWidth = rect.getWidth() * 0.95;
    std::string original = text.getText();

    if (text.getWidth() > maxWidth)
    {
        height = title.getHeight() + 10;

        std::string currentLine;

        for (char c : original)
        {
            std::string testLine = currentLine + c;
            temp.setText(testLine);

            if (temp.getWidth() > maxWidth)
            {
                temp.setText(currentLine);
                texts.push_back(temp);

                rect.setHeight(rect.getHeight() + temp.getHeight() + 10);

                currentLine = std::string(1, c);
            }
            else
            {
                currentLine = testLine;
            }
        }

        if (!currentLine.empty())
        {
            temp.setText(currentLine);
            texts.push_back(temp);

            rect.setHeight(rect.getHeight() + temp.getHeight() + 5);
        }

        height = title.getHeight() + 20 + rect.getHeight() + 20 + error.getHeight();
        return;
    }

    height = title.getHeight() + 20 + rect.getHeight() + 20 + error.getHeight();
    texts.push_back(text);
}

UIImage::UIImage() {}
UIImage::UIImage(const std::string &path, glm::vec2 size, glm::vec2 pos) : UINode(size, pos), path(path), scroll(0.0)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &imageWidth, &imageHeight, &nrComponents, 0);
    if (size.x == 0 && size.y == 0)
    {
        width = imageWidth;
        height = imageHeight;
    }

    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, imageWidth, imageHeight, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
}

UIImage::~UIImage()
{
    if (texture != 0)
        glDeleteTextures(1, &texture);
}

UIIcon::UIIcon() {}
UIIcon::UIIcon(const std::string &path, glm::vec2 size, glm::vec2 pos, glm::vec4 color) : UINode(size, pos), regImageFlag(true)
{
    rect = UIRect(size, pos, color);
    regImage = std::make_unique<UIImage>(path, size - glm::vec2(10.0f), pos + glm::vec2(5.0f));
}

UIIcon &UIIcon::setToggleImage(const std::string &path)
{
    toggleImage = std::make_unique<UIImage>(path, rect.getSize() - glm::vec2(10.0f), rect.getPos() + glm::vec2(5.0f));
    return *this;
}

const UIImage &UIIcon::getImage() const
{
    if (regImageFlag)
        return *(regImage.get());

    return *(toggleImage.get());
}

UIIcon &UIIcon::setHoverColor(glm::vec4 color)
{
    rect.setHoverColor(color);
    return *this;
}

void UIIcon::setText(const std::string &text, TextRenderer font, float scale)
{
    this->text = UIText(text, font, scale).setPos(x, y, rect.getScroll());
}

void UIIcon::setSize(glm::vec2 size)
{
    width = size.x;
    height = size.y;
    rect.setSize(size);
    regImage->setSize(size);
    if (toggleImage)
        toggleImage->setSize(size);
}

void UIIcon::setPos(glm::vec2 pos)
{
    x = pos.x;
    y = pos.y;
    text.setPos(pos.x, pos.y + rect.getHeight() / 2 - text.getHeight() / 2, rect.getScroll());
    // UINode::setPos(pos + glm::vec2(text.getWidth() + 10, 0));
    rect.setPos(pos + glm::vec2(text.getWidth() + 10, 0));
    regImage->setPos(pos + glm::vec2(5.0f) + glm::vec2(text.getWidth() + 10, 0));

    if (toggleImage)
        toggleImage->setPos(pos + glm::vec2(5.0f) + glm::vec2(text.getWidth() + 10, 0));
}

void UIIcon::setScroll(float scroll)
{
    rect.setScroll(scroll);
    regImage->setScroll(scroll);
    if (toggleImage)
        toggleImage->setScroll(scroll);
}

UIPanel::UIPanel() {}
UIPanel::UIPanel(Location location, glm::vec2 gameSize, glm::vec2 panelSize, glm::vec4 color) : UIRect(panelSize, glm::vec2(0.0f), color), panelLocation(location), lines(false), panelScroll(0.0f)
{
    gameWidth = gameSize.x;
    gameHeight = gameSize.y;
    cornerRadius = 0.01f;

    switch (location)
    {
    case TopLeft:
        x = 0 - width, y = 0;
        finalX = 0, finalY = 0;
        break;
    case BottomLeft:
        x = 0 - width, y = gameSize.y - height;
        finalX = 0, finalY = gameSize.y - height;
        break;
    case TopRight:
        x = gameSize.x, y = 0;
        finalX = gameSize.x - width, finalY = 0;
        break;
    case BottomRight:
        x = gameSize.x, y = gameSize.y - height;
        finalX = gameSize.x - width, finalY = gameSize.y - height;
        break;
    }

    button = nullptr;
}
void UIPanel::setDimensions(float WIDTH, float HEIGHT)
{
    gameWidth = WIDTH, gameHeight = HEIGHT;
    switch (panelLocation)
    {
    case TopLeft:
        y = 0, finalX = 0, finalY = 0;
        break;
    case BottomLeft:
        y = gameHeight - height, finalX = 0, finalY = gameHeight - height;
        break;
    case TopRight:
        y = 0, finalX = gameWidth - width, finalY = 0;
        break;
    case BottomRight:
        y = gameHeight - height, finalX = gameWidth - width, finalY = gameHeight - height;
        break;
    }
}
void UIPanel::enter(float deltaTime)
{
    if (panelLocation == TopLeft || panelLocation == BottomLeft)
    {
        x += 3000 * deltaTime;
        x = (x >= finalX) ? finalX : x;
    }
    else if (panelLocation == TopRight || panelLocation == BottomRight)
    {
        x -= 3000 * deltaTime;
        x = (x <= finalX) ? finalX : x;
    }

    if (button != nullptr)
    {
        if (buttonLocation == TopLeft)
        {
            button->setPos(glm::vec2(20, y + 20));
            buttonText.setPos(button->getEndX() + 20, button->getY() + (button->getHeight() / 2) - (buttonText.getHeight() / 2) + 5);
        }
        if (buttonLocation == TopRight)
        {
            button->setPos(glm::vec2(getEndX() - button->getWidth() - 20, y + 20));
            buttonText.setPos(button->getX() - buttonText.getWidth() - 20, button->getY() + (button->getHeight() / 2) - (buttonText.getHeight() / 2) + 5);
        }
        if (buttonLocation == BottomLeft)
        {
            button->setPos(glm::vec2(20, getEndY() - button->getHeight() - 20));
            buttonText.setPos(button->getEndX() + 20, button->getY() + (button->getHeight() / 2) - (buttonText.getHeight() / 2) + 5);
        }
        if (buttonLocation == BottomRight)
        {
            button->setPos(glm::vec2(getEndX() - button->getWidth() - 20, getEndY() - button->getHeight() - 20));
            buttonText.setPos(button->getX() - buttonText.getWidth() - 20, button->getY() + (button->getHeight() / 2) - (buttonText.getHeight() / 2) + 5);
        }
        button->setScroll(0.0f);
    }
}
void UIPanel::exit(float deltaTime)
{
    if (panelLocation == TopLeft || panelLocation == BottomLeft)
    {
        x -= 3000 * deltaTime;
        x = (x <= -width) ? -width : x;
    }
    else if (panelLocation == TopRight || panelLocation == BottomRight)
    {
        x += 3000 * deltaTime;
        x = (x >= gameWidth) ? gameWidth : x;
    }

    if (button != nullptr)
    {
        if (buttonLocation == TopLeft)
        {
            button->setPos(glm::vec2(20, y + 20));
            buttonText.setPos(button->getEndX() + 20, button->getY() + (button->getHeight() / 2) - (buttonText.getHeight() / 2) + 5);
        }
        if (buttonLocation == TopRight)
        {
            button->setPos(glm::vec2(getEndX() - button->getWidth() - 20, y + 20));
            buttonText.setPos(button->getX() - buttonText.getWidth() - 20, button->getY() + (button->getHeight() / 2) - (buttonText.getHeight() / 2) + 5);
        }
        if (buttonLocation == BottomLeft)
        {
            button->setPos(glm::vec2(20, getEndY() - button->getHeight() - 20));
            buttonText.setPos(button->getEndX() + 20, button->getY() + (button->getHeight() / 2) - (buttonText.getHeight() / 2) + 5);
        }
        if (buttonLocation == BottomRight)
        {
            button->setPos(glm::vec2(getEndX() - button->getWidth() - 20, getEndY() - button->getHeight() - 20));
            buttonText.setPos(button->getX() - buttonText.getWidth() - 20, button->getY() + (button->getHeight() / 2) - (buttonText.getHeight() / 2) + 5);
        }
        button->setScroll(0.0f);
    }
}
void UIPanel::scrollPanel(float deltaTime)
{
    panelScroll += 1000 * deltaTime;
    panelScroll = (panelScroll < 0) ? 0 : panelScroll;
    panelScroll = (panelScroll > (contentHeight - height)) ? contentHeight - height : panelScroll;
}
void UIPanel::addImageRect(std::unique_ptr<UIRect> imageRect)
{
    imageRect->setSize(glm::vec2(width / 2 - 30, width / 2 - 30));

    int yInc;
    if (imageRects.size() == 0)
    {
        yInc = contentHeight + 20;
        imageRect->setPos(glm::vec2(x + 10, yInc));
        contentHeight += width / 2 + 10;
    }

    if (imageRects.size() % 2 == 0 && imageRects.size() != 0)
    {
        contentHeight += width / 2 + 10;
        yInc = imageRects[imageRects.size() - 1]->getY() + (width / 2) + 10;
        imageRect->setPos(glm::vec2(x + 10, yInc));
    }
    else if (imageRects.size() != 0)
    {
        yInc = imageRects[imageRects.size() - 1]->getY();
        imageRect->setPos(glm::vec2(x + width / 2 + 5, yInc));
    }

    if (wrapHeight)
        height = std::min((double)(contentHeight + 20), gameHeight * 0.9);

    imageRect->setScroll(panelScroll);

    imageRects.push_back(std::move(imageRect));
}
void UIPanel::addRect(std::unique_ptr<UIRect> rect)
{
    int yInc = (rects.size() == 0) ? contentHeight + 20 : rects[rects.size() - 1]->getEndY() + 10;
    rect->setPos(glm::vec2(x + 10, yInc));
    rect->setCornerRadius(0.1f);
    contentHeight += rect->getHeight() + 30;

    if (wrapHeight)
        height = contentHeight + 20;

    rect->setPositions();
    rects.push_back(std::move(rect));
}
void UIPanel::addButton(std::unique_ptr<UIIcon> icon, UIText text, Location location)
{
    button = std::move(icon);
    buttonText = text;
    buttonLocation = location;
}
void UIPanel::clearRect()
{
    icon = nullptr;
    button = nullptr;
    buttonText = UIText();
    contentHeight = 20.0;
    contentWidth = 20.0;
    title.clear();

    for (auto &rect : rects)
        rect->clearText();
    imageRects.clear();
    rects.clear();
}

UIText::UIText() : UINode(glm::vec2(0), glm::vec2(0)) {}
UIText::UIText(const std::string &text, TextRenderer font, float scale, float x, float y, glm::vec4 color)
    : UINode(glm::vec2(0), glm::vec2(x, y)), text(text), font(font), scale(scale), color(color)
{
    getTextDimensions();
}

void UIText::getTextDimensions()
{
    int maxAscent = 0;
    int maxDescent = 0;
    width = 0;

    for (char c : text)
    {
        Character ch = font.Characters[c];

        int ascent = ch.Bearing.y * scale;
        int descent = (ch.Size.y - ch.Bearing.y) * scale;

        maxAscent = std::max(maxAscent, ascent);
        maxDescent = std::max(maxDescent, descent);

        width += (ch.Advance >> 6) * scale;
    }

    height = maxAscent + maxDescent;
}

void UIText::renderText()
{
    font.renderText(text, x, y - scroll, scale, color);
}
