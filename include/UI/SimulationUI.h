#pragma once
#include "Simulation.h"
#include "Rendering/TextRenderer.h"
#include "UI/UIRenderer.h"
#include <filesystem>

class CelestialObject;
class Simulation;
class Model;

enum UIState
{
    SIM,
    ADD1,
    ADD2,
    EDIT
};

class SimulationUI
{
private:
    unsigned int rectVAO, rectVBO, rectEBO;
    TextRenderer biggestFont, titleFont, bigFont, font;
    float WIDTH, HEIGHT, uiRotate;

    UIRenderer *UiRenderer;

    UIRect editScreen;
    UIPanel addPanel, editPanel, viewPanel, focusPanel, infoPanel;
    std::vector<std::unique_ptr<UIIcon>> icons;
    std::unique_ptr<UIIcon> addIcon, editIcon, cameraIcon;

    UIText cameraSpeedText;
    UIText nameText, typeText, radiusText, massText, velocityText;
    UIText angularVelText, rotationalPeriodText, circularVelText, surfaceEscapeVelText, escapeVelText, accelerationText, surfaceGravityText, netForceText;
    UIText centralBodyText, eccentricityText, semiMajorAxisText, apoapsisText, periapsisText, angularMomentumText, inclinationText, periodText;
    UIText potentialText, kineticText, totalEnergyText;

    std::vector<TextFieldHelper> editPanelFields;
    std::unique_ptr<UITextField> nameTextField, radiusTextField, massTextField, tiltTextField, orbVelocityTextField, rotPeriodTextField, starIntensityTextField;

    Shader modelShader;
    std::vector<std::string> celestialTypes;
    std::vector<Model> starPaths, planetPaths, moonPaths;

    UIState uiState;
    std::string addType, loadString;

    bool menu, typing, cursorPointer, scrollingPointer, moreInfo, error, helpMenu;

    Timer cursorTimer, loadingTimer;
    std::atomic<int> loading;

public:
    SimulationUI(float WIDTH, float HEIGHT);
    ~SimulationUI();
    void getModels();
    void loadModels();
    std::string removeTrailingZeroes(std::string number);
    bool validateNumber(std::string num, bool canBeNeg = true);
    void handleEvents(Simulation *sim, EventManager *eventManager, float deltaTime);
    std::string getTimePassed(largeFloat timePassed, largeFloat timeSpeed);
    void setupTextFields(const CelestialObject *obj);
    void setObjectDetails(const CelestialObject *obj);
    void setSpeedDetails(const CelestialObject *obj);
    void setOrbitalDetails(Simulation *sim);
    void setEnergyDetails(const CelestialObject *obj);
    void renderUIText(Simulation *sim);
    void renderInfoText(Simulation *sim);
    void renderLoadingScreen();
    void renderHelpMenu();
    void renderSimulationOverlay(Simulation *sim, EventManager *eventManager, float deltaTime);
    bool isPointing() { return cursorPointer; };
    bool isScrolling() { return scrollingPointer; };
    UIState currentState() { return uiState; };
    bool isTyping() { return typing; };
    bool isError() { return error; };
    bool isHelp() { return helpMenu; };
    bool gettingModels() { return loading == 1; };
    bool canLoadModels() { return loading == 0; };
};
