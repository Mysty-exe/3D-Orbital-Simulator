#include "Simulation.h"

float Simulation::scaledDistanceFactor = 100000000.0;
float Simulation::scaledRadiusFactor = 1.0 / 1000000.0;

Simulation::Simulation(int WIDTH, int HEIGHT)
    : WIDTH(WIDTH), HEIGHT(HEIGHT), timeSpeed(1), accumulator(0.0f), timePassed(0), editing(false), paused(false), movingObject(false), avgDistance(0), focusedObjectInt(-1), moveType(0), focusedObject(nullptr), hoveringObject(nullptr), selectedObject(nullptr)
{
    lightShader = Shader("../shaders/Star/star.vert", "../shaders/Star/star.frag", "");
    objShader = Shader("../shaders/Object/obj.vert", "../shaders/Object/obj.frag", "");
    trajShader = Shader("../shaders/Trajectory/traj.vert", "../shaders/Trajectory/traj.frag", "");
    highlightShader = Shader("../shaders/Highlight/highlight.vert", "../shaders/Highlight/highlight.frag", "");

    camera = new Camera(glm::vec3(0.0f, 0.0f, 2000.0f));
    camera->setSpeed(1000.0f);

    // celestialObjects.push_back(new CelestialObject("../assets/Models/Stars/StarOne/obj.obj", "Sun", STAR, Vector(0.0f, 0.0f, 0.0f), 1.989 * pow(10, 30), 695700.0f, scaledRadiusFactor, scaledDistanceFactor, Vector(0), 7.25, 0));
    // celestialObjects.push_back(new CelestialObject("../assets/Models/Planets/Earth/obj.obj", "Earth",
    //                                                PLANET, Vector(150000000.0f, 0.0f, 0.0f), 5.97 * pow(10, 24), 6371.0f, scaledRadiusFactor, scaledDistanceFactor, Vector(0, -2, 29.722), 23.5, glm::two_pi<float>() / 86400.0));
    // celestialObjects.push_back(new CelestialObject("../assets/Models/Planets/Mars/obj.obj", "Mars",
    //                                                PLANET, Vector(225000000.0f, -2134210.0f, 0.0f), 8.97 * pow(10, 24), 9371.0f, scaledRadiusFactor, scaledDistanceFactor, Vector(0, 3, -37.722), 23.5, glm::two_pi<float>() / 60000.0));

    dragPlaneY = 0;
}

Simulation::~Simulation()
{
    delete camera;

    for (CelestialObject *&obj : celestialObjects)
    {
        delete obj;
    }
    celestialObjects.clear();
}

void Simulation::resize(int WIDTH, int HEIGHT)
{
    this->WIDTH = WIDTH;
    this->HEIGHT = HEIGHT;
}

void Simulation::setRenderMatrices(Shader &shader, glm::mat4 view, glm::mat4 projection)
{
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
}

glm::vec2 Simulation::projectToScreenSpace(const glm::vec3 worldPos)
{
    glm::vec4 clip = projection * camera->getViewMatrix() * glm::vec4(worldPos, 1.0f);

    glm::vec3 ndc = glm::vec3(clip) / clip.w;

    glm::vec2 screen;
    screen.x = (ndc.x * 0.5f + 0.5f) * WIDTH;
    screen.y = (1.0f - (ndc.y * 0.5f + 0.5f)) * HEIGHT;

    return screen;
}

Vector Simulation::getMouseRayDirection(float mouseX, float mouseY)
{
    float x = (2.0f * mouseX) / WIDTH - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / HEIGHT;

    glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

    glm::vec4 rayEye = glm::inverse(projection) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    glm::vec3 rayWorld = glm::vec3(glm::inverse(camera->getViewMatrix()) * rayEye);

    return Vector(glm::normalize(rayWorld));
}

bool Simulation::mouseOverObject(glm::vec2 mousePos, CelestialObject *obj)
{
    glm::vec3 rayWorld =
        getMouseRayDirection(mousePos.x, mousePos.y).getGLM();
    glm::vec3 rayOrigin = camera->getCameraPos();

    glm::vec3 center = obj->getRenderPos(alpha).getGLM();
    float radius = (float)obj->getRadius() * scaledRadiusFactor;

    glm::vec3 oc = rayOrigin - center;

    float a = glm::dot(rayWorld, rayWorld);
    float b = 2.0f * glm::dot(oc, rayWorld);
    float c = glm::dot(oc, oc) - radius * radius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return false;

    float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
    float t2 = (-b + sqrt(discriminant)) / (2.0f * a);

    return t1 > 0 || t2 > 0;
}

void Simulation::drawObj(CelestialObject *obj)
{
    if (!paused)
        obj->startTrajectoryThread(celestialObjects, alpha, 0.01f * timeSpeed);

    trajShader.use();
    if (!editing)
    {
        trajShader.setVec3("color", glm::vec3(1.0f));
        obj->drawTrajectory(trajShader);

        trajShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
        obj->drawNetForceVector(trajShader);
    }

    trajShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
    obj->drawDirectionVector(trajShader);

    if (obj->getObjType() == STAR)
    {
        lightShader.use();
        lightShader.setFloat("intensity", obj->getIntensity());
        lightShader.setVec3("color", obj->getColor());
        obj->drawModel(lightShader, alpha);
    }
    else
        obj->drawModel(objShader, alpha);
}

void Simulation::drawHighlightedObj(CelestialObject *obj)
{
    if (!paused)
        obj->startTrajectoryThread(celestialObjects, alpha, 0.01f * timeSpeed);

    glStencilMask(0x00);
    trajShader.use();
    trajShader.setVec3("color", glm::vec3(1.0f));

    if (!editing)
    {
        obj->drawTrajectory(trajShader);
        trajShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
        obj->drawNetForceVector(trajShader);
    }
    trajShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
    obj->drawDirectionVector(trajShader);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);

    if (obj->getObjType() == STAR)
    {
        lightShader.use();
        lightShader.setFloat("intensity", obj->getIntensity());
        lightShader.setVec3("color", obj->getColor());
        obj->drawModel(lightShader, alpha);
    }
    else
        obj->drawModel(objShader, alpha);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);

    glm::vec2 firstPos = projectToScreenSpace((obj->getPosition() / scaledDistanceFactor).getGLM());
    glm::vec2 secondPos = projectToScreenSpace((obj->getPosition() / scaledDistanceFactor).getGLM() + camera->getCameraRight() * static_cast<float>(obj->getRadius() * scaledRadiusFactor));

    float thickness = (1 / glm::distance(firstPos, secondPos)) * 30;
    thickness = glm::clamp(thickness, 1.02f, 1.1f);

    obj->drawModel(highlightShader, alpha, true, thickness);

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glDepthFunc(GL_LESS);
}

void Simulation::run(SDL_Window *window, EventManager *eventManager, SimulationUI *simUI, float deltaTime)
{
    handleEvents(window, eventManager, simUI);
    if (!simUI->isTyping())
        handleCamera(window, eventManager, deltaTime);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    projection = glm::perspective(glm::radians(camera->getZoom()), (float)WIDTH / (float)HEIGHT, 100.0f, 1000000.0f);

    setRenderMatrices(lightShader, camera->getViewMatrix(), projection);
    setRenderMatrices(blurShader, camera->getViewMatrix(), projection);
    setRenderMatrices(objShader, camera->getViewMatrix(), projection);
    setRenderMatrices(trajShader, camera->getViewMatrix(), projection);
    setRenderMatrices(highlightShader, camera->getViewMatrix(), projection);

    int x = 0;
    objShader.use();
    for (int i = 0; i < celestialObjects.size(); i++)
    {
        if (celestialObjects[i]->getObjType() == STAR)
        {
            objShader.setVec3("lightPositions[" + std::to_string(x) + "]", celestialObjects[i]->getRenderPos(alpha).getGLM());
            objShader.setVec3("lightColors[" + std::to_string(x) + "]", celestialObjects[i]->getColor());
            x++;
        }
    }

    objShader.setInt("lightCount", x);
    objShader.setVec3("viewPos", camera->getCameraPos());

    if (!paused)
    {
        accumulator += deltaTime;

        while (accumulator >= 0.01f)
        {
            updateCelestialObjects(deltaTime, 0.01f * timeSpeed);
            accumulator -= 0.01f;
        }

        alpha = accumulator / 0.01f;
        timePassed += deltaTime * timeSpeed;
    }

    hoveringObject = nullptr;
    if (!editing)
    {
        if (selectedObject != nullptr)
            selectedObject->toggleDirectionVector(false);

        selectedObject = nullptr;
    }
    else
        setFocusedObject(nullptr);

    for (CelestialObject *obj : getObjectsByDistance((isFollowCam()) ? getFocusedObject() : nullptr))
    {
        glStencilMask(0x00);

        // if (obj == getFocusedObject())
        //     continue;

        if (!simUI->isPointing() && !simUI->isTyping() && editing && hoveringObject == nullptr && !camera->isRotating() && mouseOverObject(eventManager->getMousePos(), obj))
            hoveringObject = obj;

        if (hoveringObject == obj || selectedObject == obj)
            continue;

        drawObj(obj);
    }

    // if (isFollowCam())
    //     drawHighlightedObj(getFocusedObject());

    for (CelestialObject *obj : getObjectsByDistance((isFollowCam()) ? getFocusedObject() : nullptr))
    {
        if (obj == hoveringObject || obj == selectedObject)
            drawHighlightedObj(obj);

        if (obj == selectedObject)
            selectedObject->setDirectionVector();
    }
}

void Simulation::addObject(Model model, std::string celestialType)
{
    Vector pos = Vector(camera->getCameraPos().x, camera->getCameraPos().y, camera->getCameraPos().z) * scaledDistanceFactor;
    Vector dir = Vector(camera->getCameraFront().x, camera->getCameraFront().y, camera->getCameraFront().z);

    if (celestialType == "Stars")
        celestialObjects.push_back(new CelestialObject(model, STAR, pos, dir, scaledRadiusFactor, scaledDistanceFactor));
    if (celestialType == "Planets")
        celestialObjects.push_back(new CelestialObject(model, PLANET, pos, dir, scaledRadiusFactor, scaledDistanceFactor));
    if (celestialType == "Moons")
        celestialObjects.push_back(new CelestialObject(model, MOON, pos, dir, scaledRadiusFactor, scaledDistanceFactor));
}

void Simulation::duplicateObject(CelestialObject *obj)
{
    Vector pos = Vector(camera->getCameraPos().x, camera->getCameraPos().y, camera->getCameraPos().z) * scaledDistanceFactor;
    Vector dir = Vector(camera->getCameraFront().x, camera->getCameraFront().y, camera->getCameraFront().z);
    celestialObjects.push_back(new CelestialObject(obj, pos, dir, scaledRadiusFactor, scaledDistanceFactor));
}

void Simulation::deleteObject(CelestialObject *obj)
{
    for (int i = 0; i < celestialObjects.size(); i++)
    {
        if (celestialObjects[i] == obj)
        {
            celestialObjects[i] = nullptr;
            delete celestialObjects[i];
            celestialObjects.erase(celestialObjects.begin() + i);
        }
    }
}

void Simulation::updateCelestialObjects(float deltaTime, float dt)
{
    updatePositions(dt);
    updateOrbitValues();
    updateForces(deltaTime);
    updateVelocities(dt);
}

void Simulation::updatePositions(float dt)
{
    for (CelestialObject *obj : celestialObjects)
    {
        obj->updatePosition(dt);
        obj->setOrbitalValues(nullptr);
    }
}

void Simulation::updateForces(float deltaTime)
{
    int pairCount = 0;

    for (CelestialObject *obj1 : celestialObjects)
    {
        for (CelestialObject *obj2 : celestialObjects)
        {
            if (obj1 != obj2)
            {
                obj1->updateForce(obj2);
                pairCount++;
            }
        }
    }
}

void Simulation::updateVelocities(float dt)
{
    for (CelestialObject *obj : celestialObjects)
    {
        obj->updateVelocity(dt);
    }
}

void Simulation::updateOrbitValues()
{
    for (CelestialObject *obj : celestialObjects)
    {
        obj->setOrbitalValues(getCentralBody(obj));
        obj->setEnergyValues(celestialObjects);
    }
}

CelestialObject *Simulation::getCentralBody(CelestialObject *obj)
{
    CelestialObject *result = nullptr;
    Vector force;
    for (CelestialObject *body : celestialObjects)
    {
        if (body == obj)
            continue;
        if (body->getMass() <= obj->getMass())
            continue;

        largeFloat distance = (body->getPosition() - obj->getPosition()).magnitude;
        Vector direction = (body->getPosition() - obj->getPosition()).normalized();
        Vector f = direction * ((CelestialObject::gConstant * obj->getMass() * body->getMass()) / (distance * distance));
        if (f.magnitude > force.magnitude)
        {
            result = body;
            force = f;
        }
    }

    return result;
}

void Simulation::handleEvents(SDL_Window *window, EventManager *eventManager, SimulationUI *simUI)
{
    if (!simUI->isScrolling() && !movingObject)
        camera->zoom(static_cast<float>(eventManager->getMouseWheel().y));

    if (eventManager->isHoldingClick() && !simUI->isScrolling() && eventManager->getMouseOffset().magnitude != 0)
    {
        camera->setRotating(true);
        SDL_SetWindowRelativeMouseMode(window, true);
        SDL_GetRelativeMouseState(0, 0);
    }
    else if (!eventManager->isHoldingClick())
    {
        camera->setRotating(false);
        SDL_SetWindowRelativeMouseMode(window, false);
        if (movingObject)
            simUI->resetScroll();
        movingObject = false;
    }

    if (hoveringObject != nullptr)
    {
        if (eventManager->isLeftClick() && !simUI->isError())
        {
            movingObject = true;
            setSelectedObject(hoveringObject);
            if (selectedObject != nullptr)
                simUI->setupTextFields(selectedObject);

            Vector rayOrigin = camera->getCameraPos();
            Vector rayDir = getMouseRayDirection(eventManager->getMousePos().x, eventManager->getMousePos().y);

            if (abs(rayDir.y) > 0.0001f && moveType == 0)
            {
                dragPlaneY = (float)selectedObject->getPosition().y / scaledDistanceFactor;

                float t = (dragPlaneY - (float)rayOrigin.y) / (float)rayDir.y;
                if (t > 0.0f)
                {
                    Vector hit = rayOrigin + rayDir * t;
                    grabOffsetXZ = (selectedObject->getPosition() / scaledDistanceFactor) - hit;
                }
            }

            dragPlaneNormal = camera->getCameraFront();
            dragPlanePoint = selectedObject->getPosition() / scaledDistanceFactor;

            float denom = rayDir.dot(dragPlaneNormal);
            if (abs(denom) > 0.0001f && moveType == 1)
            {
                float t = (dragPlanePoint - rayOrigin).dot(dragPlaneNormal) / denom;
                if (t > 0.0f)
                {
                    Vector hit = rayOrigin + rayDir * t;
                    grabOffsetY = (selectedObject->getPosition() / scaledDistanceFactor) - hit;
                }
            }
        }
    }

    if (movingObject)
    {
        if (moveType == 0)
        {
            Vector rayOrigin = camera->getCameraPos();
            Vector rayDir = getMouseRayDirection(eventManager->getMousePos().x, eventManager->getMousePos().y);

            if (abs(rayDir.y) > 0.0001f)
            {
                float t = (dragPlaneY - (float)rayOrigin.y) / (float)rayDir.y;

                if (t > 0.0f)
                {
                    Vector hit = rayOrigin + rayDir * t;
                    hit.y = dragPlaneY;
                    Vector newPos = hit + grabOffsetXZ;

                    float grid = 0.5;
                    if (eventManager->checkHoldKeyEvent(CTRL))
                        newPos = Vector(round(newPos.x / grid), round(newPos.y / grid), round(newPos.z / grid)) * grid;

                    selectedObject->setPosition(newPos * scaledDistanceFactor);
                }
            }
        }
        else if (moveType == 1)
        {
            Vector rayOrigin = camera->getCameraPos();
            Vector rayDir = getMouseRayDirection(eventManager->getMousePos().x, eventManager->getMousePos().y);

            float denom = rayDir.dot(dragPlaneNormal);
            if (abs(denom) > 0.0001f)
            {
                float t = (dragPlanePoint - rayOrigin).dot(dragPlaneNormal) / denom;
                if (t > 0.0f)
                {
                    Vector hit = rayOrigin + rayDir * t;
                    Vector newPos = hit + grabOffsetY;

                    float grid = 0.5;
                    if (eventManager->checkHoldKeyEvent(CTRL))
                        newPos = Vector(round(newPos.x / grid), round(newPos.y / grid), round(newPos.z / grid)) * grid;

                    Vector current = selectedObject->getPosition() / scaledDistanceFactor;
                    selectedObject->setPosition(Vector(current.x, newPos.y, current.z) * scaledDistanceFactor);
                }
            }
        }
        else if ((moveType == 2 || moveType == 3) && selectedObject->getVelocity().magnitude != 0)
        {
            float dx = (float)eventManager->getMouseOffset().x;
            float dy = (float)eventManager->getMouseOffset().y;

            float sensitivity = 0.005f;

            Vector dir = selectedObject->getVelocity().normalized();

            glm::vec3 dirGLM = selectedObject->getVelocity().normalized().getGLM();

            glm::vec3 up = camera->getCameraUp();
            glm::vec3 right = glm::normalize(glm::cross(up, dirGLM));

            if (glm::length(right) < 0.0001f)
                right = glm::vec3(1, 0, 0);

            glm::mat4 yawRot = glm::rotate(glm::mat4(1.0f), dx * sensitivity, up);
            glm::mat4 pitchRot = glm::rotate(glm::mat4(1.0f), dy * sensitivity, right);

            glm::vec3 newDir = glm::normalize(glm::vec3(pitchRot * yawRot * glm::vec4(dirGLM, 0.0f)));

            if (moveType == 2)
                newDir.y = (float)dir.y;
            else if (moveType == 3)
            {
                newDir = glm::vec3((float)dir.x, newDir.y, (float)dir.z);
            }

            selectedObject->setDirection(Vector(newDir));
        }
    }

    if (eventManager->checkHoldKeyEvent(CTRL) && simUI->currentState() == SIM)
    {
        if (eventManager->checkPressKeyEvent(P))
            pause(!paused);

        if (eventManager->checkPressKeyEvent(PLUS))
        {
            speedUp();
        }
        else if (eventManager->checkPressKeyEvent(MINUS))
        {
            speedDown();
        }

        if (celestialObjects.size() > 0)
        {
            if (eventManager->checkPressKeyEvent(BACKSPACE))
                setFocusedObject(nullptr);
            if (eventManager->checkPressKeyEvent(LEFT))
            {
                simUI->resetScroll();
                focusedObjectInt = (focusedObjectInt <= 0) ? celestialObjects.size() - 1 : focusedObjectInt - 1;
                camera->setCameraDirection(glm::normalize(celestialObjects[focusedObjectInt]->getRenderPos(alpha).getGLM() - camera->getCameraPos()));
                focusedObject = celestialObjects[focusedObjectInt];
            }
            else if (eventManager->checkPressKeyEvent(RIGHT))
            {
                simUI->resetScroll();
                focusedObjectInt = (focusedObjectInt >= celestialObjects.size() - 1) ? 0 : focusedObjectInt + 1;
                camera->setCameraDirection(glm::normalize(celestialObjects[focusedObjectInt]->getRenderPos(alpha).getGLM() - camera->getCameraPos()));
                focusedObject = celestialObjects[focusedObjectInt];
            }
        }
    }

    if (editing)
    {
        if ((eventManager->checkPressKeyEvent(ONE) || eventManager->checkPressKeyEvent(TWO) || eventManager->checkPressKeyEvent(THREE) || eventManager->checkPressKeyEvent(FOUR)) && !simUI->isTyping())
        {
            if (eventManager->checkPressKeyEvent(ONE))
                moveType = 0;
            else if (eventManager->checkPressKeyEvent(TWO))
                moveType = 1;
            else if (eventManager->checkPressKeyEvent(THREE) || eventManager->checkPressKeyEvent(FOUR))
            {
                simUI->resetScroll();
                if (eventManager->checkPressKeyEvent(FOUR))
                    moveType = 2;
                else if (eventManager->checkPressKeyEvent(FOUR))
                    moveType = 3;
            }

            if (selectedObject != nullptr)
                if (moveType == 2 || moveType == 3)
                    selectedObject->toggleDirectionVector(true);
                else
                    selectedObject->toggleDirectionVector(false);
        }
    }

    if (!eventManager->checkHoldKeyEvent(CTRL))
    {
        if (eventManager->checkHoldKeyEvent(UP) || eventManager->checkHoldKeyEvent(RIGHT))
            camera->setSpeed(camera->getSpeed() * 1.05);
        if (eventManager->checkHoldKeyEvent(DOWN) || eventManager->checkHoldKeyEvent(LEFT))
            camera->setSpeed(camera->getSpeed() * 0.95);
    }

    if (camera->getSpeed() > 10000)
        camera->setSpeed(10000.0f);
    if (camera->getSpeed() < 5.0f)
        camera->setSpeed(5.0f);
}

void Simulation::handleCamera(SDL_Window *window, EventManager *eventManager, float deltaTime)
{
    moveCamera(eventManager, camera, deltaTime);

    if (focusedObject != nullptr)
    {
        if (eventManager->isLetClickGo())
            camera->setStepValues(glm::normalize(focusedObject->getRenderPos(alpha).getGLM() - camera->getCameraPos()));

        if (!camera->isRotating())
            camera->setCameraDirection(glm::normalize(focusedObject->getRenderPos(alpha).getGLM() - camera->getCameraPos()));
    }

    if (selectedObject != nullptr && camera->isReorienting())
    {
        if (!camera->isRotating())
            camera->setCameraDirection(glm::normalize(selectedObject->getRenderPos(alpha).getGLM() - camera->getCameraPos()));
    }

    if (camera->isRotating() && !movingObject)
        rotateCamera(window, *camera, eventManager->getMouseOffset());
}

void Simulation::moveCamera(EventManager *eventManager, Camera *camera, float deltaTime)
{
    glm::vec3 cameraMovement(0.0f);
    if (eventManager->checkHoldKeyEvent(W))
    {
        cameraMovement += camera->getCameraFront();
    }
    else if (eventManager->checkHoldKeyEvent(S))
    {
        cameraMovement += -camera->getCameraFront();
    }
    if (eventManager->checkHoldKeyEvent(A))
    {
        cameraMovement += -camera->getCameraRight();
    }
    else if (eventManager->checkHoldKeyEvent(D))
    {
        cameraMovement += camera->getCameraRight();
    }
    if (eventManager->checkHoldKeyEvent(SPACE))
    {
        cameraMovement += camera->getCameraUp();
    }
    else if (eventManager->checkHoldKeyEvent(SHIFT))
    {
        cameraMovement += -camera->getCameraUp();
    }

    camera->moveCamera(cameraMovement, deltaTime);
};

void Simulation::rotateCamera(SDL_Window *window, Camera &camera, Vector relative)
{
    camera.rotateCamera(static_cast<float>(relative.x), static_cast<float>(-relative.y));
}

void Simulation::setCameraFocusedPos()
{
    CelestialObject *obj;
    if (isFollowCam())
        obj = getFocusedObject();
    else if (selectedObject != nullptr)
        obj = getSelectedObject();

    camera->setCameraDirection(glm::normalize(obj->getRenderPos(alpha).getGLM() - camera->getCameraPos()));

    float scalar = static_cast<float>(obj->getRadius() * scaledRadiusFactor) + 5000;
    glm::vec3 direction = glm::normalize(camera->getCameraPos() - obj->getRenderPos(alpha).getGLM());

    glm::vec3 newCameraPos;
    newCameraPos = obj->getRenderPos(alpha).getGLM() + direction * (float)scalar;
    camera->setPosition(newCameraPos);
}

float Simulation::getDistance(CelestialObject *obj1, CelestialObject *obj2)
{
    if (obj2 == nullptr)
        return glm::distance(obj1->getPosition().getGLM(), camera->getCameraPos() * scaledDistanceFactor);

    return static_cast<float>(obj1->getPosition().distance(obj2->getPosition()));
}

float Simulation::getTimeSpeed()
{
    return timeSpeed;
}

largeFloat Simulation::getTimePassed()
{
    return timePassed;
}

void Simulation::speedDown()
{
    timeSpeed /= 10;
    timeSpeed = (timeSpeed < 1) ? 1 : timeSpeed;
}

void Simulation::pause(bool pauseBool)
{
    paused = pauseBool;
}

void Simulation::speedUp()
{
    timeSpeed *= 10;
    timeSpeed = (timeSpeed > 10000000) ? 10000000 : timeSpeed;
}

bool Simulation::isPaused()
{
    return paused;
}

bool Simulation::isFollowCam()
{
    return focusedObject != nullptr;
}

std::vector<CelestialObject *> Simulation::getCelestialObjects()
{
    return celestialObjects;
}

std::vector<CelestialObject *> Simulation::getObjectsByDistance(CelestialObject *obj)
{
    Camera *cam = camera;
    CelestialObject *followObj = obj;
    std::vector<CelestialObject *> objects = getCelestialObjects();

    std::sort(objects.begin(), objects.end(), [cam, followObj](CelestialObject *a, CelestialObject *b)
              { 
        if (followObj != nullptr)
            return followObj->getPosition().distance(a->getPosition()) < followObj->getPosition().distance(b->getPosition());

        return glm::distance(a->getPosition().getGLM(), cam->getCameraPos() * scaledDistanceFactor) < glm::distance(b->getPosition().getGLM(), cam->getCameraPos() * scaledDistanceFactor); });

    return objects;
}

CelestialObject *Simulation::getFocusedObject()
{
    return focusedObject;
}

void Simulation::setFocusedObject(CelestialObject *obj)
{
    focusedObject = obj;
    if (obj == nullptr)
    {
        focusedObjectInt = -1;
        return;
    }

    for (int i = 0; i < celestialObjects.size(); i++)
    {
        if (celestialObjects[i] == obj)
        {
            focusedObjectInt = i;
        }
    }
}

void Simulation::setSelectedObject(CelestialObject *obj)
{
    if (selectedObject != nullptr)
        selectedObject->toggleDirectionVector(false);

    selectedObject = obj;
    if (selectedObject != nullptr && (moveType == 2 || moveType == 3))
        selectedObject->toggleDirectionVector(true);
}
