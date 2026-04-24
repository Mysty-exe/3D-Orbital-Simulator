#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Rendering/TextRenderer.h"
#include "Rendering/stb_image.h"

struct UIIcon;
struct UITextField;

enum Location
{
    TopRight,
    BottomRight,
    TopLeft,
    BottomLeft
};

struct UINode
{
protected:
    float width, height, x, y;

public:
    UINode();
    UINode(glm::vec2 size, glm::vec2 pos);
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    glm::vec2 getSize() const { return glm::vec2(width, height); }
    void setSize(glm::vec2 size) { width = size.x, height = size.y; }
    void setWidth(float width) { this->width = width; }
    void setHeight(float height) { this->height = height; }
    float getX() const { return x; }
    float getY() const { return y; }
    glm::vec2 getPos() const { return glm::vec2(x, y); }
    float getEndX(float scale = 1) const { return x + width * scale; }
    float getEndY(float scale = 1) const { return y + height * scale; }
    void setX(float x) { this->x = x; }
    void setY(float y) { this->y = y; }
    void setPos(glm::vec2 pos) { this->x = pos.x, this->y = pos.y; }
    bool isHovering(glm::vec2 mousePos) { return (mousePos.x >= x && mousePos.y >= y && mousePos.x <= x + width && mousePos.y <= y + height); }
    virtual void hover(glm::vec2 mousePos) {}
};

struct UIText : public UINode
{
private:
    TextRenderer font;
    std::string text;
    float scroll;
    glm::vec2 scale;
    glm::vec4 color;

public:
    UIText();
    UIText(const std::string &text, TextRenderer font, glm::vec2 scale = glm::vec2(1, 1), float x = 0, float y = 0, glm::vec4 color = glm::vec4(1.0f));
    void getTextDimensions();

    UIText &setText(const std::string &newText)
    {
        this->text = newText;
        getTextDimensions();
        return *this;
    }
    UIText &setPos(float x, float y, float scroll = 0)
    {
        this->x = x;
        this->y = y;
        this->scroll = scroll;
        return *this;
    }
    std::string getText() { return text; }
    void renderText();
    void setScale(glm::vec2 scale) { this->scale = scale; }
};

struct UIRect : public UINode
{
protected:
    float cornerRadius, contentWidth, contentHeight, scroll;
    glm::vec4 color, regColor, hoverColor;
    bool wrapWidth, wrapHeight;
    std::unique_ptr<UIIcon> icon;

    std::vector<UIText> title;
    std::vector<UIText> texts;
    std::vector<std::unique_ptr<UIIcon>> icons;
    std::vector<std::unique_ptr<UITextField>> textFields;

public:
    UIRect();
    UIRect(glm::vec4 color);
    UIRect(glm::vec2 size, glm::vec2 pos, glm::vec4 color);
    glm::vec4 getColor() const { return color; }
    void setColor(glm::vec4 color) { regColor = color; }
    void setCornerRadius(float radius) { cornerRadius = radius; }
    float getCornerRadius() const { return cornerRadius; }
    void forceHover() { color = hoverColor; }
    virtual UIRect &setHoverColor(glm::vec4 color)
    {
        hoverColor = color;
        return *this;
    }
    bool isHovering(glm::vec2 mousePos) { return (mousePos.x >= x && mousePos.y >= y - scroll && mousePos.x <= x + width && mousePos.y <= y + height - scroll); }
    void hover(glm::vec2 mousePos) { color = (isHovering(mousePos)) ? hoverColor : regColor; }
    void setWrap(bool width, bool height) { wrapWidth = width, wrapHeight = height; }
    void setPos(glm::vec2 pos);
    void setIcon(std::unique_ptr<UIIcon> icon);
    void addIcon(std::string path, glm::vec2 size);
    void addBodyIcon(std::unique_ptr<UIIcon> i);
    UIIcon *getIcon() const { return icon.get(); }
    void setTitle(UIText &text);
    void addText(UIText &text);
    void addTextField(std::unique_ptr<UITextField> textField);
    void setPositions(glm::vec2 scale);
    std::vector<UIText> &getTitle() { return title; }
    std::vector<UIText> &getTexts() { return texts; }
    std::vector<std::unique_ptr<UITextField>> &getTextFields() { return textFields; }
    std::vector<std::unique_ptr<UIIcon>> &getIcons() { return icons; }
    float getScroll() const { return scroll; }
    float getContentHeight() const { return contentHeight; }
    void setScroll(float scroll) { this->scroll = scroll; }
    void clearText();
};

struct TextFieldHelper
{
public:
    UIText title, text, error;
    bool activated, cursorOn;
    TextFieldHelper(UIText title, UIText text, UIText error, bool activated)
    {
        this->title = title;
        this->text = text;
        this->error = error;
        this->activated = activated;
    }
};

struct UITextField : public UINode
{
private:
    float scroll;
    UIText title, text, error, units;
    std::vector<UIText> texts;
    glm::vec4 outlineColor, activatedOutlineColor;
    UIRect rect;
    bool activated, cursorOn;

public:
    UITextField();
    UITextField(TextFieldHelper helper, glm::vec2 size, glm::vec2 pos, UIText units, glm::vec4 color);
    void setPositions();
    void setX(float x)
    {
        this->x = x;
        rect.setX(x);
    }
    void setY(float y)
    {
        this->y = y;
        rect.setY(title.getEndY() + 20);
    }
    void setPos(glm::vec2 pos)
    {
        setX(pos.x);
        setY(pos.y);
    }
    float getFieldX() { return rect.getX(); }
    float getFieldY() { return rect.getY(); }
    float getFieldWidth() { return rect.getWidth(); }
    float getFieldHeight() { return rect.getHeight(); }
    void setFieldWidth(float width) { rect.setWidth(width); }
    void setFieldHeight(float height) { rect.setHeight(height); }
    bool isHovering(glm::vec2 mousePos) { return (mousePos.x >= getFieldX() && mousePos.y >= getFieldY() && mousePos.x <= getFieldX() + rect.getWidth() && mousePos.y <= getFieldY() + rect.getHeight()); }
    float getScroll() { return scroll; }
    void setScroll(float scrollValue) { scroll = scrollValue; }
    float getCornerRadius() { return rect.getCornerRadius(); }
    glm::vec4 getOutlineColor() { return outlineColor; }
    glm::vec4 getActivatedOutlineColor() { return activatedOutlineColor; }
    void activate() { activated = !activated; }
    std::vector<UIText> &getTexts() { return texts; }
    UIText &getTitle() { return title; }
    UIText &getError() { return error; }
    UIText &getUnits() { return units; }
    bool getActivated() { return activated; }
    bool getCursorOn() { return cursorOn; }
};

struct UIImage : public UINode
{
protected:
    unsigned int texture;
    std::string path;
    int imageWidth, imageHeight;
    float scroll;

public:
    UIImage();
    UIImage(const std::string &path, glm::vec2 size, glm::vec2 pos);
    UIImage(const UIImage &) = delete;
    UIImage &operator=(const UIImage &) = delete;
    ~UIImage();
    unsigned int getTexture() const { return texture; };
    const std::string &getPath() { return path; }
    float getScroll() const { return scroll; }
    void setScroll(float scroll) { this->scroll = scroll; }
};

struct UIIcon : public UINode
{
private:
    UIRect rect;
    std::unique_ptr<UIImage> regImage, toggleImage;
    UIText text;
    bool regImageFlag;

public:
    UIIcon();
    UIIcon(const std::string &path, glm::vec2 size, glm::vec2 pos, glm::vec4 color);
    UIIcon &setToggleImage(const std::string &path);
    UIIcon &setHoverColor(glm::vec4 color);
    const UIText &getText() const { return text; }
    void setText(const std::string &text, TextRenderer font, glm::vec2 scale);
    void setSize(glm::vec2 size);
    const UIRect &getRect() const { return rect; }
    const UIImage &getImage() const;
    glm::vec4 getColor() const { return rect.getColor(); }
    UIText getText() { return text; }
    void hover(glm::vec2 mousePos) { rect.hover(mousePos); }
    bool isHovering(glm::vec2 mousePos) { return (mousePos.x >= rect.getX() && mousePos.y >= rect.getY() - rect.getScroll() && mousePos.x <= rect.getX() + width && mousePos.y <= rect.getY() - rect.getScroll() + rect.getHeight()); }
    void toggleIcon(bool toggle) { regImageFlag = toggle; }
    void setPos(glm::vec2 pos);
    const float getScroll() const { return rect.getScroll(); };
    void setScroll(float scroll);
};

struct UIPanel : public UIRect
{
private:
    float finalX, finalY;
    float gameWidth, gameHeight;
    float panelScroll;
    Location panelLocation, buttonLocation;
    std::unique_ptr<UIIcon> button;
    UIText buttonText;
    bool lines;

    std::vector<std::unique_ptr<UIRect>> rects;
    std::vector<std::unique_ptr<UIRect>> imageRects;

public:
    UIPanel();
    UIPanel(Location location, glm::vec2 gameSize, glm::vec2 panelSize, glm::vec4 color);
    void setDimensions(float WIDTH, float HEIGHT);
    void wrap();
    void enter(float deltaTime);
    void exit(float deltaTime);
    void scrollPanel(float deltaTime);
    bool canScroll() { return contentHeight > height; }
    float getPanelScroll() const { return panelScroll; }
    void setPanelScroll(float panelScroll) { this->panelScroll = panelScroll; }
    void setLines() { lines = true; }
    bool isLines() { return lines; }
    UIIcon *getButton() { return button.get(); }
    UIText getButtonText() { return buttonText; }
    Location getButtonLocation() { return buttonLocation; }
    Location getPanelLocation() { return panelLocation; }
    std::vector<std::unique_ptr<UIRect>> &getRects() { return rects; }
    std::vector<std::unique_ptr<UIRect>> &getImageRects() { return imageRects; }
    void addButton(std::unique_ptr<UIIcon> icon, UIText text, Location location);
    void addImageRect(std::unique_ptr<UIRect> imageRect);
    void addRect(std::unique_ptr<UIRect> rect, glm::vec2 scale);
    void clearRect();
};
