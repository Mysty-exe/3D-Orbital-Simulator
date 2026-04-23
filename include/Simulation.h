#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <vector>
#include "EventManager.h"
#include "Utils/Timer.h"
#include "UI/SimulationUI.h"
#include "Rendering/Shader.h"
#include "Rendering/TextRenderer.h"
#include "Rendering/Camera.h"
#include "Objects/CelestialObject.h"

class SimulationUI;

class Simulation
{
private:
    static float scaledRadiusFactor, scaledDistanceFactor;

    int WIDTH, HEIGHT;
    Camera *camera;

    float alpha, timeSpeed, accumulator, avgDistance, dragPlaneY;

    largeFloat timePassed;

    Shader lightShader, blurShader, objShader, trajShader, highlightShader;

    std::vector<CelestialObject *> celestialObjects;
    CelestialObject *focusedObject, *hoveringObject, *selectedObject;
    int focusedObjectInt, moveType;

    glm::mat4 projection;

    TextRenderer textRenderer;
    bool paused, editing, movingObject;

    Vector grabOffsetY, grabOffsetXZ, dragPlaneNormal, dragPlanePoint, dirPlaneNormal, dirPlanePoint;

public:
    Simulation(int WIDTH, int HEIGHT);
    ~Simulation();

    Camera *getCamera() { return camera; };
    float getCameraSpeed(float deltaTime) { return camera->getSpeed() * scaledDistanceFactor / 1000; };

    void resize(int WIDTH, int HEIGHT);

    void setRenderMatrices(Shader &shader, glm::mat4 view, glm::mat4 projection);
    glm::vec2 projectToScreenSpace(const glm::vec3 worldPos);

    Vector getMouseRayDirection(float mouseX, float mouseY);
    bool mouseOverObject(glm::vec2 mousePos, CelestialObject *obj);
    void drawObj(CelestialObject *obj);
    void drawHighlightedObj(CelestialObject *obj);
    void run(SDL_Window *window, EventManager *eventManager, SimulationUI *simUI, float deltaTime);

    void addObject(Model model, std::string celestialType);
    void duplicateObject(CelestialObject *obj);
    void deleteObject(CelestialObject *obj);
    void updateCelestialObjects(float deltaTime, float dt);
    void updatePositions(float dt);
    void updateForces(float deltaTime);
    void updateVelocities(float dt);
    void updateOrbitValues();
    CelestialObject *getCentralBody(CelestialObject *obj);

    void handleEvents(SDL_Window *window, EventManager *eventManager, SimulationUI *simUI);

    void handleCamera(SDL_Window *window, EventManager *eventManager, float deltaTime);
    void moveCamera(EventManager *eventManager, Camera *camera, float deltaTime);
    void rotateCamera(SDL_Window *window, Camera &camera, Vector relative);
    void setCameraFocusedPos();

    float getDistance(CelestialObject *obj1, CelestialObject *obj2);

    float getTimeSpeed();
    largeFloat getTimePassed();
    void speedDown();
    void pause(bool pauseBool);
    void setEditing(bool editing) { this->editing = editing; }
    void speedUp();
    bool isPaused();
    bool isFollowCam();
    std::vector<CelestialObject *> getCelestialObjects();
    std::vector<CelestialObject *> getObjectsByDistance(CelestialObject *obj);
    CelestialObject *getFocusedObject();
    void setFocusedObject(CelestialObject *obj);

    glm::mat4 getProjection() { return projection; };
    int getMoveType() { return moveType; };
    bool isMovingObject() { return movingObject; };
    bool isHovering() { return hoveringObject != nullptr; };
    CelestialObject *getSelectedObject() { return selectedObject; };
    void setSelectedObject(CelestialObject *obj);
};
