#pragma once
#include <algorithm>
#include "Utils/Vector.h"

enum KeyEvent
{
    W,
    A,
    S,
    D,
    P,
    SPACE,
    SHIFT,
    PLUS,
    MINUS,
    LEFT,
    RIGHT,
    BACKSPACE,
    CTRL,
    TAB,
    ENTER,
    DELETE,
    UP,
    DOWN,
    ONE,
    TWO,
    THREE,
    FOUR,
    ESCAPE
};

class EventManager
{
private:
    SDL_Event event;
    std::vector<KeyEvent> holdKeyEvents, pressKeyEvents;
    bool holdingClick, leftClick, letClickGo, quit;
    Vector mousePos, mouseOffset, mouseWheel;
    const bool *keys;
    std::string currentBuffer;

    int changedWidth, changedHeight;

public:
    EventManager() : quit(false), holdingClick(false), leftClick(false), letClickGo(false), changedWidth(0), changedHeight(0)
    {
    }

    std::vector<KeyEvent> getHoldKeyEvents()
    {
        return holdKeyEvents;
    }

    std::vector<KeyEvent> getPressKeyEvents()
    {
        return pressKeyEvents;
    }

    bool userQuit()
    {
        return quit;
    }

    void getEvents()
    {
        float relativeX, relativeY;
        SDL_GetRelativeMouseState(&relativeX, &relativeY);
        mouseOffset = Vector(relativeX, relativeY);

        float mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        mousePos = Vector(mouseX, mouseY);

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
            if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                changedWidth = event.window.data1;
                changedHeight = event.window.data2;
            }
            if (event.type == SDL_EVENT_TEXT_INPUT)
            {
                currentBuffer = event.text.text;
            }
            if (event.type == SDL_EVENT_MOUSE_WHEEL)
            {
                mouseWheel.x = event.wheel.x;
                mouseWheel.y = event.wheel.y;
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    leftClick = true;
                    holdingClick = true;
                }
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    letClickGo = true;
                    holdingClick = false;
                }
            }
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_P)
                    pressKeyEvents.push_back(P);
                if (event.key.key == SDLK_EQUALS)
                    pressKeyEvents.push_back(PLUS);
                if (event.key.key == SDLK_MINUS)
                    pressKeyEvents.push_back(MINUS);
                if (event.key.key == SDLK_LEFT)
                    pressKeyEvents.push_back(LEFT);
                if (event.key.key == SDLK_RIGHT)
                    pressKeyEvents.push_back(RIGHT);
                if (event.key.key == SDLK_BACKSPACE)
                    pressKeyEvents.push_back(BACKSPACE);
                if (event.key.key == SDLK_RETURN)
                    pressKeyEvents.push_back(ENTER);
                if (event.key.key == SDLK_TAB)
                    pressKeyEvents.push_back(TAB);
                if (event.key.key == SDLK_DELETE)
                    pressKeyEvents.push_back(DELETE);
                if (event.key.key == SDLK_D)
                    pressKeyEvents.push_back(D);
                if (event.key.key == SDLK_LCTRL)
                    pressKeyEvents.push_back(CTRL);
                if (event.key.key == SDLK_1)
                    pressKeyEvents.push_back(ONE);
                if (event.key.key == SDLK_2)
                    pressKeyEvents.push_back(TWO);
                if (event.key.key == SDLK_3)
                    pressKeyEvents.push_back(THREE);
                if (event.key.key == SDLK_4)
                    pressKeyEvents.push_back(FOUR);
                if (event.key.key == SDLK_ESCAPE)
                    pressKeyEvents.push_back(ESCAPE);
            }
        }

        keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_W])
            holdKeyEvents.push_back(W);
        else if (keys[SDL_SCANCODE_S])
            holdKeyEvents.push_back(S);

        if (keys[SDL_SCANCODE_A])
            holdKeyEvents.push_back(A);
        else if (keys[SDL_SCANCODE_D])
            holdKeyEvents.push_back(D);

        if (keys[SDL_SCANCODE_SPACE])
            holdKeyEvents.push_back(SPACE);
        else if (keys[SDL_SCANCODE_LSHIFT])
            holdKeyEvents.push_back(SHIFT);

        if (keys[SDL_SCANCODE_LCTRL])
            holdKeyEvents.push_back(CTRL);

        if (keys[SDL_SCANCODE_UP])
            holdKeyEvents.push_back(UP);
        if (keys[SDL_SCANCODE_DOWN])
            holdKeyEvents.push_back(DOWN);
        if (keys[SDL_SCANCODE_RIGHT])
            holdKeyEvents.push_back(RIGHT);
        if (keys[SDL_SCANCODE_LEFT])
            holdKeyEvents.push_back(LEFT);
    }

    Vector EventManager::getMouseOffset()
    {
        return mouseOffset;
    }

    Vector EventManager::getMouseWheel()
    {
        return mouseWheel;
    }

    glm::vec2 EventManager::getMousePos()
    {
        return mousePos.getGLM();
    }

    bool EventManager::checkHoldKeyEvent(KeyEvent event)
    {
        if (std::find(holdKeyEvents.begin(), holdKeyEvents.end(), event) != holdKeyEvents.end())
        {
            return true;
        }
        return false;
    }

    bool EventManager::checkPressKeyEvent(KeyEvent event)
    {
        if (std::find(pressKeyEvents.begin(), pressKeyEvents.end(), event) != pressKeyEvents.end())
        {
            return true;
        }
        return false;
    }

    std::string &getBuffer() { return currentBuffer; }

    bool EventManager::isLeftClick()
    {
        return leftClick;
    }

    bool EventManager::isHoldingClick()
    {
        return holdingClick;
    }

    bool EventManager::isLetClickGo()
    {
        return letClickGo;
    }

    bool EventManager::changedWindowSize()
    {
        return changedWidth != 0 && changedHeight != 0;
    }

    int EventManager::getWidth()
    {
        return changedWidth;
    }

    int EventManager::getHeight()
    {
        return changedHeight;
    }

    void EventManager::clear()
    {
        mouseWheel = Vector(0, 0);
        holdKeyEvents.clear();
        pressKeyEvents.clear();
        changedWidth = 0;
        changedHeight = 0;
        leftClick = false;
        letClickGo = false;
        currentBuffer = "";
    }
};
