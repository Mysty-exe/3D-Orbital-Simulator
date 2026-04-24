#include <UI/SimulationUI.h>

SimulationUI::SimulationUI(float WIDTH, float HEIGHT) : WIDTH(WIDTH), HEIGHT(HEIGHT), scale(Vector(1).getGLM()), uiState(SIM), menu(false), typing(false), moreInfo(false), error(false), helpMenu(false)
{
    font = TextRenderer(WIDTH, HEIGHT), bigFont = TextRenderer(WIDTH, HEIGHT), titleFont = TextRenderer(WIDTH, HEIGHT), biggestFont = TextRenderer(WIDTH, HEIGHT);
    font.Load("../assets/Fonts/DejaVuSans.ttf", 30);
    bigFont.Load("../assets/Fonts/DejaVuSans.ttf", 42);
    titleFont.Load("../assets/Fonts/DejaVuSans.ttf", 50);
    biggestFont.Load("../assets/Fonts/DejaVuSans.ttf", 80);

    UiRenderer = new UIRenderer(WIDTH, HEIGHT);

    addPanel = UIPanel(TopLeft, glm::vec2(WIDTH, HEIGHT), glm::vec2(WIDTH / 5, HEIGHT), glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
    editPanel = UIPanel(TopLeft, glm::vec2(WIDTH, HEIGHT), glm::vec2(WIDTH / 5, HEIGHT * 0.9), glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
    viewPanel = UIPanel(TopLeft, glm::vec2(WIDTH, HEIGHT), glm::vec2(WIDTH / 5, HEIGHT * 0.9), glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
    infoPanel = UIPanel(TopRight, glm::vec2(WIDTH, HEIGHT), glm::vec2(WIDTH * 0.3, HEIGHT), glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
    focusPanel = UIPanel(BottomRight, glm::vec2(WIDTH, HEIGHT), glm::vec2(WIDTH / 5, HEIGHT / 3), glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
    addPanel.setDimensions(WIDTH, HEIGHT);
    editPanel.setDimensions(WIDTH, HEIGHT);
    viewPanel.setDimensions(WIDTH, HEIGHT);
    infoPanel.setDimensions(WIDTH, HEIGHT);
    focusPanel.setDimensions(WIDTH, HEIGHT);
    addPanel.setWrap(false, true);
    editPanel.setWrap(false, false);
    viewPanel.setWrap(false, false);
    infoPanel.setWrap(false, false);
    focusPanel.setWrap(false, true);

    editScreen = UIRect(glm::vec2(WIDTH, HEIGHT), glm::vec2(0, 0), glm::vec4(1.0, 1.0, 1.0, 0.2));
    editScreen.setCornerRadius(0.0f);

    auto menuIcon = std::make_unique<UIIcon>("../assets/Icons/menu.png", glm::vec2(50), glm::vec2(std::max({viewPanel.getEndX(), editPanel.getEndX(), addPanel.getEndX()}) + 15, 10), glm::vec4(1.0, 1.0, 1.0, 0.3f));
    menuIcon->setHoverColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
    icons.push_back(std::move(menuIcon));

    auto slowDownIcon = std::make_unique<UIIcon>("../assets/Icons/slowDown.png", glm::vec2(50), glm::vec2(icons.back()->getEndX() + 30, 10), glm::vec4(1.0, 1.0, 1.0, 0.3f));
    slowDownIcon->setHoverColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
    icons.push_back(std::move(slowDownIcon));

    auto pauseIcon = std::make_unique<UIIcon>("../assets/Icons/pause.png", glm::vec2(50), glm::vec2(icons.back()->getEndX() + 10, 10), glm::vec4(1.0, 1.0, 1.0, 0.3f));
    pauseIcon->setToggleImage("../assets/Icons/unpause.png");
    pauseIcon->setHoverColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
    icons.push_back(std::move(pauseIcon));

    auto speedUpIcon = std::make_unique<UIIcon>("../assets/Icons/speedUp.png", glm::vec2(50), glm::vec2(icons.back()->getEndX() + 10, 10), glm::vec4(1.0, 1.0, 1.0, 0.3f));
    speedUpIcon->setHoverColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
    icons.push_back(std::move(speedUpIcon));

    addIcon = std::make_unique<UIIcon>("../assets/Icons/plus.png", glm::vec2(50), glm::vec2(15, HEIGHT - 10), glm::vec4(1.0, 1.0, 1.0, 0.3f));
    addIcon->setHoverColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));

    editIcon = std::make_unique<UIIcon>("../assets/Icons/edit.png", glm::vec2(50), glm::vec2(15, HEIGHT - 10), glm::vec4(1.0, 1.0, 1.0, 0.3f));
    editIcon->setHoverColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));

    cameraIcon = std::make_unique<UIIcon>("../assets/Icons/zoom.png", glm::vec2(40), glm::vec2(15, HEIGHT - 10), glm::vec4(1.0, 1.0, 1.0, 0.3f));
    cameraIcon->setHoverColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));

    cameraSpeedText = UIText("", font, Vector(1).getGLM());
    nameText = UIText("", bigFont, Vector(1).getGLM());
    typeText = UIText(" ", font, Vector(1).getGLM());
    radiusText = UIText(" ", font, Vector(1).getGLM());
    massText = UIText(" ", font, Vector(1).getGLM());
    velocityText = UIText(" ", font, Vector(1).getGLM());

    angularVelText = UIText("", font, Vector(1).getGLM());
    rotationalPeriodText = UIText("", font, Vector(1).getGLM());
    circularVelText = UIText("", font, Vector(1).getGLM());
    surfaceEscapeVelText = UIText(" ", font, Vector(1).getGLM());
    escapeVelText = UIText(" ", font, Vector(1).getGLM());
    accelerationText = UIText(" ", font, Vector(1).getGLM());
    surfaceGravityText = UIText(" ", font, Vector(1).getGLM());
    netForceText = UIText(" ", font, Vector(1).getGLM());

    centralBodyText = UIText("", font, Vector(1).getGLM());
    eccentricityText = UIText("", font, Vector(1).getGLM());
    semiMajorAxisText = UIText("", font, Vector(1).getGLM());
    apoapsisText = UIText("", font, Vector(1).getGLM());
    periapsisText = UIText("", font, Vector(1).getGLM());
    angularMomentumText = UIText("", font, Vector(1).getGLM());
    inclinationText = UIText("", font, Vector(1).getGLM());
    periodText = UIText("", font, Vector(1).getGLM());

    potentialText = UIText("", font, Vector(1).getGLM());
    kineticText = UIText("", font, Vector(1).getGLM());
    totalEnergyText = UIText("", font, Vector(1).getGLM());

    editPanelFields.push_back(TextFieldHelper(UIText("Name", font, Vector(1).getGLM(), 0, 0), UIText("", font, Vector(1).getGLM(), 0, 0), UIText("", font, Vector(1).getGLM(), 0, 0), false));
    editPanelFields.push_back(TextFieldHelper(UIText("Radius", font, Vector(1).getGLM(), 0, 0), UIText("", font, Vector(1).getGLM(), 0, 0), UIText("", font, Vector(1).getGLM(), 0, 0), false));
    editPanelFields.push_back(TextFieldHelper(UIText("Mass", font, Vector(1).getGLM(), 0, 0), UIText("", font, Vector(1).getGLM(), 0, 0), UIText("", font, Vector(1).getGLM(), 0, 0), false));
    editPanelFields.push_back(TextFieldHelper(UIText("Orbital Velocity", font, Vector(1).getGLM(), 0, 0), UIText("", font, Vector(1).getGLM(), 0, 0), UIText("", font, Vector(1).getGLM(), 0, 0), false));
    editPanelFields.push_back(TextFieldHelper(UIText("Tilt", font, Vector(1).getGLM(), 0, 0), UIText("", font, Vector(1).getGLM(), 0, 0), UIText("", font, Vector(1).getGLM(), 0, 0), false));
    editPanelFields.push_back(TextFieldHelper(UIText("Rotational Period", font, Vector(1).getGLM(), 0, 0), UIText("", font, Vector(1).getGLM(), 0, 0), UIText("", font, Vector(1).getGLM(), 0, 0), false));
    editPanelFields.push_back(TextFieldHelper(UIText("Star Intensity", font, Vector(1).getGLM(), 0, 0), UIText("", font, Vector(1).getGLM(), 0, 0), UIText("", font, Vector(1).getGLM(), 0, 0), false));

    modelShader = Shader("../shaders/UI/obj.vert", "../shaders/UI/obj.frag", "");
    celestialTypes = {"Stars", "Planets", "Moons"};

    cursorTimer.start();
    loadingTimer.start();
    loadString = "Loading Simulation";
    loading = 1;

    std::thread loadingThread(&SimulationUI::getModels, this);
    loadingThread.detach();
}

SimulationUI::~SimulationUI()
{
    delete UiRenderer;
}

void SimulationUI::resize(int WIDTH, int HEIGHT)
{
    scale *= Vector(WIDTH / this->WIDTH, HEIGHT / this->HEIGHT);

    this->WIDTH = WIDTH;
    this->HEIGHT = HEIGHT;

    addPanel.setSize(glm::vec2(WIDTH / 5, HEIGHT));
    editPanel.setSize(glm::vec2(WIDTH / 5, HEIGHT * 0.9));
    viewPanel.setSize(glm::vec2(WIDTH / 5, HEIGHT * 0.9));
    infoPanel.setSize(glm::vec2(WIDTH * 0.3, HEIGHT));
    focusPanel.setSize(glm::vec2(WIDTH / 5, HEIGHT / 3));

    addPanel.setDimensions(WIDTH, HEIGHT);
    editPanel.setDimensions(WIDTH, HEIGHT);
    viewPanel.setDimensions(WIDTH, HEIGHT);
    infoPanel.setDimensions(WIDTH, HEIGHT);
    focusPanel.setDimensions(WIDTH, HEIGHT);

    UiRenderer->resize(WIDTH, HEIGHT);
    editScreen.setSize(glm::vec2(WIDTH, HEIGHT));

    icons.clear();
    auto menuIcon = std::make_unique<UIIcon>("../assets/Icons/menu.png", glm::vec2(50) * glm::vec2(scale.getGLM()), glm::vec2(std::max({viewPanel.getEndX(), editPanel.getEndX(), addPanel.getEndX()}) + 15, 10), glm::vec4(1.0, 1.0, 1.0, 0.3f));
    menuIcon->setHoverColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
    icons.push_back(std::move(menuIcon));

    auto slowDownIcon = std::make_unique<UIIcon>("../assets/Icons/slowDown.png", glm::vec2(50) * glm::vec2(scale.getGLM()), glm::vec2(icons.back()->getEndX() + 30, 10), glm::vec4(1.0, 1.0, 1.0, 0.3f));
    slowDownIcon->setHoverColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
    icons.push_back(std::move(slowDownIcon));

    auto pauseIcon = std::make_unique<UIIcon>("../assets/Icons/pause.png", glm::vec2(50) * glm::vec2(scale.getGLM()), glm::vec2(icons.back()->getEndX() + 10, 10), glm::vec4(1.0, 1.0, 1.0, 0.3f));
    pauseIcon->setToggleImage("../assets/Icons/unpause.png");
    pauseIcon->setHoverColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
    icons.push_back(std::move(pauseIcon));

    auto speedUpIcon = std::make_unique<UIIcon>("../assets/Icons/speedUp.png", glm::vec2(50) * glm::vec2(scale.getGLM()), glm::vec2(icons.back()->getEndX() + 10, 10), glm::vec4(1.0, 1.0, 1.0, 0.3f));
    speedUpIcon->setHoverColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
    icons.push_back(std::move(speedUpIcon));

    addIcon->setSize(glm::vec2(50) * glm::vec2(scale.getGLM()));
    editIcon->setSize(glm::vec2(50) * glm::vec2(scale.getGLM()));
    cameraIcon->setSize(glm::vec2(50) * glm::vec2(scale.getGLM()));

    font.resetSize(WIDTH, HEIGHT);
    bigFont.resetSize(WIDTH, HEIGHT);
    titleFont.resetSize(WIDTH, HEIGHT);
    biggestFont.resetSize(WIDTH, HEIGHT);

    cameraSpeedText.setScale(glm::vec2(scale.getGLM()));
    nameText.setScale(glm::vec2(scale.getGLM()));
    typeText.setScale(glm::vec2(scale.getGLM()));
    radiusText.setScale(glm::vec2(scale.getGLM()));
    massText.setScale(glm::vec2(scale.getGLM()));
    velocityText.setScale(glm::vec2(scale.getGLM()));
    angularVelText.setScale(glm::vec2(scale.getGLM()));
    rotationalPeriodText.setScale(glm::vec2(scale.getGLM()));
    circularVelText.setScale(glm::vec2(scale.getGLM()));
    surfaceEscapeVelText.setScale(glm::vec2(scale.getGLM()));
    escapeVelText.setScale(glm::vec2(scale.getGLM()));
    accelerationText.setScale(glm::vec2(scale.getGLM()));
    surfaceGravityText.setScale(glm::vec2(scale.getGLM()));
    netForceText.setScale(glm::vec2(scale.getGLM()));
    centralBodyText.setScale(glm::vec2(scale.getGLM()));
    eccentricityText.setScale(glm::vec2(scale.getGLM()));
    semiMajorAxisText.setScale(glm::vec2(scale.getGLM()));
    apoapsisText.setScale(glm::vec2(scale.getGLM()));
    periapsisText.setScale(glm::vec2(scale.getGLM()));
    angularMomentumText.setScale(glm::vec2(scale.getGLM()));
    inclinationText.setScale(glm::vec2(scale.getGLM()));
    periodText.setScale(glm::vec2(scale.getGLM()));

    potentialText.setScale(glm::vec2(scale.getGLM()));
    kineticText.setScale(glm::vec2(scale.getGLM()));
    totalEnergyText.setScale(glm::vec2(scale.getGLM()));

    for (auto &fieldHelpers : editPanelFields)
    {
        fieldHelpers.title.setScale(glm::vec2(scale.getGLM()));
        fieldHelpers.text.setScale(glm::vec2(scale.getGLM()));
        fieldHelpers.error.setScale(glm::vec2(scale.getGLM()));
    }
}

void SimulationUI::getModels()
{
    for (const auto &entry : std::filesystem::directory_iterator("../assets/Models/Stars"))
    {
        Model m = Model(entry.path().generic_string() + "/obj.obj");
        starPaths.push_back(m);
    }

    for (const auto &entry : std::filesystem::directory_iterator("../assets/Models/Planets"))
    {
        Model m = Model(entry.path().generic_string() + "/obj.obj");
        planetPaths.push_back(m);
    }

    for (const auto &entry : std::filesystem::directory_iterator("../assets/Models/Moons"))
    {
        Model m = Model(entry.path().generic_string() + "/obj.obj");
        moonPaths.push_back(m);
    }

    loading = 0;
}

void SimulationUI::loadModels()
{
    for (auto &model : starPaths)
    {
        model.upload();
    }
    for (auto &model : planetPaths)
    {
        model.upload();
    }
    for (auto &model : moonPaths)
    {
        model.upload();
    }

    loading = -1;
}

std::string SimulationUI::removeTrailingZeroes(std::string number)
{
    if (number == "0")
        return number;

    auto removeZeroes = [](std::string number)
    {
        std::string newStr = "";
        for (int i = number.length() - 1; i > -1; i--)
        {
            if (number[i] == '.' || (number[i] != '0' && number.find('.') != std::string::npos))
            {
                (number[i] == '.') ? newStr = number.substr(0, i) : newStr = number.substr(0, i + 1);
                break;
            }
            if (number[i] != '0' && number.find('.') == std::string::npos)
            {
                return number;
            }
        }
        return newStr;
    };

    int index = 0;
    std::string dividor = "";
    if (number.find("e+") != std::string::npos)
    {
        index = number.find("e+");
        dividor = "e+";
    }
    else if (number.find("e-") != std::string::npos)
    {
        index = number.find("e-");
        dividor = "e-";
    }

    if (dividor != "")
    {
        return removeZeroes(number.substr(0, index)) +
               dividor + removeZeroes(number.substr(index + 2, number.size()));
    }

    return removeZeroes(number);
}

std::string SimulationUI::addCommas(std::string number)
{
    int originalSize = number.size();
    int start = number.size() - 3;
    if (number.size() % 3 == 0)
        originalSize--;

    for (int x = 1; x <= floor(originalSize / 3); x++)
    {
        number.insert(start, ",");
        start -= 3;
    }

    return number;
}

bool SimulationUI::validateNumber(std::string num, bool canBeNeg)
{
    if (num == "" || num == "nan" || num == "-nan" || num == "inf" || num == "-inf")
        return false;

    try
    {
        long double n = std::stold(num);
        if (!canBeNeg && n < 0)
            return false;
    }
    catch (const std::exception &e)
    {
        return false;
    }
    return true;
}

void SimulationUI::resetScroll()
{
    viewPanel.setPanelScroll(0);
    infoPanel.setPanelScroll(0);
    addPanel.setPanelScroll(0);
    editPanel.setPanelScroll(0);
    focusPanel.setPanelScroll(0);
}

void SimulationUI::handleEvents(Simulation *sim, EventManager *eventManager, float deltaTime)
{
    if (eventManager->checkPressKeyEvent(ESCAPE))
    {
        helpMenu = !helpMenu;
        return;
    }

    if (!eventManager->checkHoldKeyEvent(CTRL))
    {
        if (eventManager->checkHoldKeyEvent(UP) || eventManager->checkHoldKeyEvent(RIGHT) || eventManager->checkHoldKeyEvent(DOWN) || eventManager->checkHoldKeyEvent(LEFT))
            cameraSpeedText.setText("Camera Speed: " + addCommas(std::to_string((int)sim->getCameraSpeed(deltaTime))) + " km/s");
        else
            cameraSpeedText.setText("");
    }

    if (viewPanel.isHovering(eventManager->getMousePos()) && viewPanel.canScroll())
    {
        scrollingPointer = true;
        if (eventManager->getMouseWheel().y > 0)
        {
            viewPanel.scrollPanel(-deltaTime);
        }
        if (eventManager->getMouseWheel().y < 0)
        {
            viewPanel.scrollPanel(deltaTime);
        }
    }

    if ((addPanel.isHovering(eventManager->getMousePos()) && addPanel.canScroll()) || (editPanel.canScroll() && sim->isMovingObject()))
    {
        scrollingPointer = true;
        if (eventManager->getMouseWheel().y > 0)
        {
            addPanel.scrollPanel(-deltaTime);
        }
        if (eventManager->getMouseWheel().y < 0)
        {
            addPanel.scrollPanel(deltaTime);
        }
    }

    if (editPanel.isHovering(eventManager->getMousePos()) && editPanel.canScroll())
    {
        scrollingPointer = true;
        if (eventManager->getMouseWheel().y > 0)
        {
            editPanel.scrollPanel(-deltaTime);
        }
        if (eventManager->getMouseWheel().y < 0)
        {
            editPanel.scrollPanel(deltaTime);
        }
    }

    if (infoPanel.isHovering(eventManager->getMousePos()) && infoPanel.canScroll())
    {
        scrollingPointer = true;
        if (eventManager->getMouseWheel().y > 0)
        {
            infoPanel.scrollPanel(-deltaTime);
        }
        if (eventManager->getMouseWheel().y < 0)
        {
            infoPanel.scrollPanel(deltaTime);
        }
    }

    icons[2]->toggleIcon(!sim->isPaused());

    for (int i = 0; i < icons.size(); i++)
    {
        icons[i]->hover(eventManager->getMousePos());
        if (icons[i]->isHovering(eventManager->getMousePos()))
        {
            cursorPointer = true;

            if (eventManager->isLeftClick())
            {
                if (i == 0 && !error)
                {
                    resetScroll();
                    sim->pause(false);
                    uiState = SIM;
                    menu = !menu;
                }
                if (uiState == SIM)
                {
                    if (i == 1)
                        sim->speedDown();
                    if (i == 2)
                        sim->pause(!(sim->isPaused()));
                    if (i == 3)
                        sim->speedUp();
                }
            }
        }
    }

    addIcon->hover(eventManager->getMousePos());
    if (addIcon->isHovering(eventManager->getMousePos()))
    {
        cursorPointer = true;

        if (eventManager->isLeftClick() && !error)
        {
            resetScroll();
            uiState = (uiState != ADD1) ? ADD1 : SIM;
            sim->pause(false);
            if (uiState == ADD1)
            {
                sim->setFocusedObject(nullptr);
                sim->pause(true);
            }

            menu = false;
        }
    }

    editIcon->hover(eventManager->getMousePos());
    if (editIcon->isHovering(eventManager->getMousePos()))
    {
        cursorPointer = true;

        if (eventManager->isLeftClick() && !error)
        {
            resetScroll();
            sim->pause(true);
            uiState = (uiState != EDIT) ? EDIT : SIM;
            if (uiState == SIM)
            {
                sim->setFocusedObject(nullptr);
                sim->pause(false);
            }
            menu = false;
        }
    }

    if (sim->getSelectedObject() != nullptr)
    {
        UIIcon *i = editPanel.getIcon();
        i->setHoverColor(glm::vec4(1.0, 1.0, 1.0, 0.5f));
        i->hover(eventManager->getMousePos());
        if (i->isHovering(eventManager->getMousePos()))
        {
            cursorPointer = true;

            if (eventManager->isLeftClick() && !error)
            {
                resetScroll();
                sim->setSelectedObject(nullptr);
            }
        }
    }

    if (sim->isFollowCam() || sim->getSelectedObject() != nullptr)
    {
        cameraIcon->hover(eventManager->getMousePos());
        if (cameraIcon->isHovering(eventManager->getMousePos()))
        {
            cursorPointer = true;

            if (eventManager->isLeftClick())
            {
                sim->setCameraFocusedPos();
            }
        }
    }

    if (focusPanel.getButton() != nullptr)
    {
        focusPanel.getButton()->hover(eventManager->getMousePos());
        if (focusPanel.getButton()->isHovering(eventManager->getMousePos()))
        {
            cursorPointer = true;

            if (eventManager->isLeftClick())
                moreInfo = true;
        }
    }

    if (infoPanel.getButton() != nullptr)
    {
        infoPanel.getButton()->hover(eventManager->getMousePos());
        if (infoPanel.getButton()->isHovering(eventManager->getMousePos()))
        {
            cursorPointer = true;

            if (eventManager->isLeftClick())
                moreInfo = false;
        }
    }

    std::vector<std::unique_ptr<UIRect>> &viewRects = viewPanel.getRects();

    for (int i = 0; i < viewRects.size(); i++)
    {
        viewRects[i]->hover(eventManager->getMousePos());
        if (viewRects[i]->isHovering(eventManager->getMousePos()))
        {
            cursorPointer = true;
            if (eventManager->isLeftClick())
            {
                infoPanel.setPanelScroll(0);
                if (sim->isFollowCam() && i == 0)
                    sim->setFocusedObject(nullptr);
                else
                    sim->setFocusedObject(sim->getObjectsByDistance((sim->isFollowCam()) ? sim->getFocusedObject() : nullptr)[i]);
            }
        }
    }

    std::vector<std::unique_ptr<UIRect>> &addRects = addPanel.getRects();
    std::vector<std::unique_ptr<UIRect>> &addImages = addPanel.getImageRects();

    if (uiState == ADD1)
    {
        for (int i = 0; i < addRects.size(); i++)
        {
            addRects[i]->hover(eventManager->getMousePos());
            if (addRects[i]->isHovering(eventManager->getMousePos()))
            {
                cursorPointer = true;
                if (eventManager->isLeftClick())
                {
                    resetScroll();
                    uiState = ADD2;
                    addType = celestialTypes[i];
                    addPanel.setPanelScroll(0);
                }
            }
        }
    }

    if (uiState == ADD2)
    {
        for (int i = 0; i < addImages.size(); i++)
        {
            addImages[i]->hover(eventManager->getMousePos());
            if (addImages[i]->isHovering(eventManager->getMousePos()))
            {
                cursorPointer = true;
                if (eventManager->isLeftClick())
                {
                    if (addType == "Stars")
                        sim->addObject(starPaths[i], addType);
                    if (addType == "Planets")
                        sim->addObject(planetPaths[i], addType);
                    if (addType == "Moons")
                        sim->addObject(moonPaths[i], addType);

                    resetScroll();
                    uiState = EDIT;
                    sim->setSelectedObject(sim->getCelestialObjects()[sim->getCelestialObjects().size() - 1]);
                    setupTextFields(sim->getSelectedObject());
                }
            }
        }
    }

    if (sim->isFollowCam())
        viewRects[0]->forceHover();

    typing = false;
    if (sim->getSelectedObject() == nullptr)
    {
        std::vector<std::unique_ptr<UIRect>> &editRects = editPanel.getRects();

        for (int i = 0; i < editRects.size(); i++)
        {
            editRects[i]->hover(eventManager->getMousePos());
            if (editRects[i]->isHovering(eventManager->getMousePos()))
            {
                cursorPointer = true;
                if (eventManager->isLeftClick())
                {
                    resetScroll();
                    setupTextFields(sim->getObjectsByDistance((sim->isFollowCam()) ? sim->getFocusedObject() : nullptr)[i]);
                    sim->setSelectedObject(sim->getObjectsByDistance((sim->isFollowCam()) ? sim->getFocusedObject() : nullptr)[i]);
                    sim->setCameraFocusedPos();
                }
            }
        }
    }
    else
    {
        if (eventManager->isLeftClick())
        {
            for (int i = 0; i < editPanelFields.size(); i++)
                editPanelFields[i].activated = false;
        }

        std::vector<std::unique_ptr<UIRect>> &editRects = editPanel.getRects();

        for (int i = 0; i < editRects.size(); i++)
        {
            std::vector<std::unique_ptr<UITextField>> &textFields = editRects[i]->getTextFields();

            bool pressedTab = false;
            for (int j = 0; j < textFields.size(); j++)
            {
                if (j == 0 && (editPanelFields[j].text.getText().size() < 2 || editPanelFields[j].text.getText().size() > 32))
                    editPanelFields[j].error.setText("Invalid Name");
                else if (j == 0)
                    editPanelFields[j].error.setText("");

                for (CelestialObject *obj : sim->getCelestialObjects())
                {
                    if (obj != sim->getSelectedObject())
                    {
                        if (obj->getName() == editPanelFields[j].text.getText())
                        {
                            editPanelFields[j].error.setText("Invalid Name");
                            break;
                        }
                    }
                }

                if (j == 1 && !(validateNumber(editPanelFields[j].text.getText(), false)))
                    editPanelFields[j].error.setText("Invalid Radius");
                else if (j == 1)
                    editPanelFields[j].error.setText("");

                if (j == 2 && !(validateNumber(editPanelFields[j].text.getText(), false)))
                    editPanelFields[j].error.setText("Invalid Mass");
                else if (j == 2)
                    editPanelFields[j].error.setText("");

                if (j == 3 && !(validateNumber(editPanelFields[j].text.getText())))
                    editPanelFields[j].error.setText("Invalid Orbital Velocity");
                else if (j == 3)
                    editPanelFields[j].error.setText("");

                if (j == 4 && !(validateNumber(editPanelFields[j].text.getText())))
                    editPanelFields[j].error.setText("Invalid Tilt");
                else if (j == 4)
                    editPanelFields[j].error.setText("");

                if (j == 5 && !(validateNumber(editPanelFields[j].text.getText(), false)))
                    editPanelFields[j].error.setText("Invalid Rotational Period");
                else if (j == 5)
                    editPanelFields[j].error.setText("");

                if (sim->getSelectedObject()->getObjType() == STAR)
                {
                    if (j == 6 && !(validateNumber(editPanelFields[j].text.getText(), false)))
                        editPanelFields[j].error.setText("Invalid Star Intensity");
                    else if (j == 6)
                        editPanelFields[j].error.setText("");
                }

                if (editPanelFields[j].activated)
                {
                    typing = true;
                    if (eventManager->checkPressKeyEvent(BACKSPACE) && editPanelFields[j].text.getText().size() > 0)
                    {
                        editPanelFields[j].text.setText(editPanelFields[j].text.getText().substr(0, editPanelFields[j].text.getText().size() - 1));
                    }

                    if (eventManager->getBuffer().size() > 0)
                        editPanelFields[j].text.setText(editPanelFields[j].text.getText() + eventManager->getBuffer());

                    if (eventManager->checkPressKeyEvent(TAB) && !pressedTab)
                    {
                        editPanelFields[j].activated = false;
                        pressedTab = true;

                        if (j != 5 || sim->getSelectedObject()->getObjType() == STAR)
                            editPanelFields[j + 1].activated = true;
                    }
                }

                if (j != 6 || sim->getSelectedObject()->getObjType() == STAR)
                {
                    textFields[j]->hover(eventManager->getMousePos());
                    if (textFields[j]->isHovering(eventManager->getMousePos()))
                    {
                        cursorPointer = true;
                        if (eventManager->isLeftClick())
                        {
                            editPanelFields[j].activated = !editPanelFields[j].activated;
                            for (int k = 0; k < editPanelFields.size(); k++)
                            {
                                if (j != k && editPanelFields[k].activated)
                                    editPanelFields[k].activated = false;
                            }
                        }
                    }
                }
            }

            error = false;
            for (int j = 0; j < textFields.size(); j++)
            {
                if (editPanelFields[j].error.getText().size() > 0)
                    error = true;
            }

            if (!error)
            {
                long double radius = (editPanelFields[1].text.getText() != "") ? std::stold(editPanelFields[1].text.getText()) : 1;
                long double mass = (editPanelFields[2].text.getText() != "") ? std::stold(editPanelFields[2].text.getText()) : 1;
                long double velocity = (editPanelFields[3].text.getText() != "") ? std::stold(editPanelFields[3].text.getText()) : 0;
                float tilt = (editPanelFields[4].text.getText() != "") ? std::stof(editPanelFields[4].text.getText()) : 0;
                float rotPeriod = (editPanelFields[5].text.getText() != "") ? std::stof(editPanelFields[5].text.getText()) : 0;
                float intensity = (editPanelFields[6].text.getText() != "") ? std::stof(editPanelFields[6].text.getText()) : 1;
                sim->getSelectedObject()->updateObject(editPanelFields[0].text.getText(),
                                                       radius, mass, velocity, tilt, rotPeriod, intensity);
            }

            if (eventManager->checkPressKeyEvent(DELETE))
            {
                resetScroll();
                error = false;
                sim->deleteObject(sim->getSelectedObject());
                sim->setSelectedObject(nullptr);
            }

            if (eventManager->checkHoldKeyEvent(CTRL) && !error)
            {
                if (eventManager->checkPressKeyEvent(D))
                {
                    resetScroll();
                    sim->duplicateObject(sim->getSelectedObject());
                    sim->setSelectedObject(sim->getCelestialObjects()[sim->getCelestialObjects().size() - 1]);
                }
            }
        }
    }

    for (auto &rect : infoPanel.getRects())
    {
        for (int i = 0; i < rect->getIcons().size(); i++)
        {
            if (rect->getIcons()[i]->isHovering(eventManager->getMousePos()))
            {
                cursorPointer = true;
                if (eventManager->isLeftClick())
                {
                    if (i == 0)
                        sim->getFocusedObject()->toggleNetForceVector();
                    else
                        sim->getFocusedObject()->toggleTrajectoryVector();
                }
            }
        }
    }
}

std::string SimulationUI::getTimePassed(largeFloat timePassed, largeFloat timeSpeed)
{
    largeFloat tempSecs = timePassed;
    std::string year = std::to_string((int)floor(tempSecs / 31536000));
    tempSecs = fmod(tempSecs, 31536000);
    std::string days = std::to_string((int)floor(tempSecs / 86400));
    tempSecs = fmod(tempSecs, 86400);
    std::string hours = std::to_string((int)floor(tempSecs / 3600));
    tempSecs = fmod(tempSecs, 3600);
    std::string mins = std::to_string((int)floor(tempSecs / 60));
    tempSecs = fmod(tempSecs, 60);
    std::string secs = std::to_string((int)floor(tempSecs));

    hours = (hours.length() == 1) ? "0" + hours : hours;
    mins = (mins.length() == 1) ? "0" + mins : mins;
    secs = (secs.length() == 1) ? "0" + secs : secs;

    std::string timePassedText = "Year " + year + ", Day " + days + ", " + hours + ":" + mins + ":" + secs + "   -   " + addCommas(std::to_string(static_cast<int>(timeSpeed))) + "x Speed";

    return timePassedText;
}

void SimulationUI::setupTextFields(const CelestialObject *obj)
{
    std::stringstream radiusSS, massSS, tiltSS, orbVelSS, rotPeriodSS, starIntensitySS;
    radiusSS << obj->getRadius() / 1000;
    massSS << obj->getMass();
    tiltSS << obj->getTilt();
    orbVelSS << obj->getVelocity().magnitude;
    rotPeriodSS << obj->getRotationalPeriod();
    starIntensitySS << obj->getIntensity();
    std::string radiusStr = radiusSS.str();
    std::string massStr = massSS.str();
    std::string tiltStr = tiltSS.str();
    std::string orbVelStr = orbVelSS.str();
    std::string rotPeriodStr = rotPeriodSS.str();
    std::string starIntensityStr = starIntensitySS.str();

    editPanelFields[0] = TextFieldHelper(UIText("Name", font, glm::vec2(scale.getGLM()), 0, 0), UIText(obj->getName(), font, glm::vec2(scale.getGLM()), 0, 0), UIText("", font, glm::vec2(scale.getGLM()), 0, 0, glm::vec4(0.4, 0, 0, 1)), false);
    editPanelFields[1] = TextFieldHelper(UIText("Radius", font, glm::vec2(scale.getGLM()), 0, 0), UIText(radiusStr, font, glm::vec2(scale.getGLM()), 0, 0), UIText("", font, glm::vec2(scale.getGLM()), 0, 0, glm::vec4(0.4, 0, 0, 1)), false);
    editPanelFields[2] = TextFieldHelper(UIText("Mass", font, glm::vec2(scale.getGLM()), 0, 0), UIText(massStr, font, glm::vec2(scale.getGLM()), 0, 0), UIText("", font, glm::vec2(scale.getGLM()), 0, 0, glm::vec4(0.4, 0, 0, 1)), false);
    editPanelFields[3] = TextFieldHelper(UIText("Orbital Velocity", font, glm::vec2(scale.getGLM()), 0, 0), UIText(orbVelStr, font, glm::vec2(scale.getGLM()), 0, 0), UIText("", font, glm::vec2(scale.getGLM()), 0, 0, glm::vec4(0.4, 0, 0, 1)), false);
    editPanelFields[4] = TextFieldHelper(UIText("Tilt", font, glm::vec2(scale.getGLM()), 0, 0), UIText(tiltStr, font, glm::vec2(scale.getGLM()), 0, 0), UIText("", font, glm::vec2(scale.getGLM()), 0, 0, glm::vec4(0.4, 0, 0, 1)), false);
    editPanelFields[5] = TextFieldHelper(UIText("Rotiational Period", font, glm::vec2(scale.getGLM()), 0, 0), UIText(rotPeriodStr, font, glm::vec2(scale.getGLM()), 0, 0), UIText("", font, glm::vec2(scale.getGLM()), 0, 0, glm::vec4(0.4, 0, 0, 1)), false);
    editPanelFields[6] = TextFieldHelper(UIText("Star Intensity", font, glm::vec2(scale.getGLM()), 0, 0), UIText(starIntensityStr, font, glm::vec2(scale.getGLM()), 0, 0), UIText("", font, glm::vec2(scale.getGLM()), 0, 0, glm::vec4(0.4, 0, 0, 1)), false);
}

void SimulationUI::setObjectDetails(const CelestialObject *obj)
{
    std::stringstream radiusSS, massSS, velSS;
    radiusSS << obj->getRadius() / 1000;
    massSS << obj->getMass();
    velSS << obj->getVelocity().magnitude;
    std::string radiusStr = radiusSS.str();
    std::string massStr = massSS.str();
    std::string velStr = velSS.str();

    focusPanel.addIcon("../assets/Icons/" + obj->getObjTypeStr() + "s.png", glm::vec2(45) * glm::vec2(scale.getGLM()));
    velocityText.setText("Velocity: " + removeTrailingZeroes(velStr) + " m/s");
    massText.setText("Mass: " + removeTrailingZeroes(massStr) + " kg");
    radiusText.setText("Radius: " + removeTrailingZeroes(radiusStr) + " km");
    typeText.setText("Type: " + obj->getObjTypeStr());
    nameText.setText(obj->getName());
}

void SimulationUI::setSpeedDetails(const CelestialObject *obj)
{
    std::stringstream angularVelSS, rotationalPeriodSS, circularVelSS, surfaceEscapeVelSS, accelerationSS, surfaceGravitySS, netForceSS;
    angularVelSS << obj->getAngularVelocity();
    rotationalPeriodSS << obj->getRotationalPeriod();
    circularVelSS << obj->getCircularVelocity();
    surfaceEscapeVelSS << obj->getSurfaceEscapeVelocity();
    accelerationSS << obj->getAcceleration().magnitude;
    surfaceGravitySS << obj->getSurfaceGravity();
    netForceSS << obj->getNetForces().magnitude;
    std::string angularVelStr = angularVelSS.str();
    std::string rotationalPeriodStr = rotationalPeriodSS.str();
    std::string circularVelStr = circularVelSS.str();
    std::string surfaceEscapeVelStr = surfaceEscapeVelSS.str();
    std::string accelerationStr = accelerationSS.str();
    std::string surfaceGravityStr = surfaceGravitySS.str();
    std::string netForceStr = netForceSS.str();

    angularVelText.setText("Angular Velocity: " + angularVelStr + "°/s");
    rotationalPeriodText.setText("Orbital Rotational Period: " + rotationalPeriodStr + " seconds");
    circularVelText.setText("Circular Velocity: " + circularVelStr + " m/s");
    surfaceEscapeVelText.setText("Surface Escape Velocity: " + surfaceEscapeVelStr + " m/s");
    accelerationText.setText("Acceleration: " + accelerationStr + " m/s²");
    surfaceGravityText.setText("Surface Gravity: " + surfaceGravityStr + " m/s²");
    netForceText.setText("Net Force: " + netForceStr + " N");
}

void SimulationUI::setOrbitalDetails(Simulation *sim)
{
    CelestialObject *obj = sim->getFocusedObject();
    std::stringstream eccentricitySS, semiMajorAxisSS, apoapsisSS, periapsisSS, angularMomentumSS, inclinationSS, periodSS, escapeVelSS;
    eccentricitySS << obj->getEccentricity();
    semiMajorAxisSS << obj->getSemiMajorAxis();
    apoapsisSS << obj->getApoapsis();
    periapsisSS << obj->getPeriapsis();
    angularMomentumSS << obj->getAngularMomentum().magnitude;
    inclinationSS << obj->getInclination();
    periodSS << obj->getOrbitalPeriod() / 3600 / 24;
    escapeVelSS << obj->getEscapeVelocity();
    std::string eccentricityStr = eccentricitySS.str();
    std::string semiMajorAxisStr = semiMajorAxisSS.str();
    std::string apoapsisStr = apoapsisSS.str();
    std::string periapsisStr = periapsisSS.str();
    std::string angularMomentumStr = angularMomentumSS.str();
    std::string inclinationStr = inclinationSS.str();
    std::string periodStr = periodSS.str();
    std::string escapeVelStr = escapeVelSS.str();

    centralBodyText.setText("Central Body: " + sim->getCentralBody(obj)->getName());
    eccentricityText.setText("Eccentricity: " + eccentricityStr);
    semiMajorAxisText.setText("Semi-Major-Axis: " + semiMajorAxisStr + " m");
    apoapsisText.setText("Apoapsis:  " + apoapsisStr + " m");
    periapsisText.setText("Periapsis: " + periapsisStr + " m");
    angularMomentumText.setText("Angular Momentum: " + angularMomentumStr + " J × s");
    inclinationText.setText("Inclination: " + inclinationStr + "°");
    periodText.setText("Orbital Period: " + periodStr + " days");
    escapeVelText.setText("Orbit Escape Velocity: " + escapeVelStr + " m/s");
}

void SimulationUI::setEnergyDetails(const CelestialObject *obj)
{
    std::stringstream potentialSS, kineticSS, totalEnergySS;
    potentialSS << obj->getPotentialEnergy();
    kineticSS << std::scientific << std::setprecision(3) << obj->getKineticEnergy();
    totalEnergySS << std::scientific << std::setprecision(3) << obj->getTotalEnergy();
    std::string potentialStr = potentialSS.str();
    std::string kineticStr = kineticSS.str();
    std::string totalEnergyStr = totalEnergySS.str();

    potentialText.setText("Gravitational Potential Energy:  " + potentialStr + " J");
    kineticText.setText("Kinetic Energy: " + kineticStr + " J");
    totalEnergyText.setText("Total Mechanical Energy:  " + totalEnergyStr + " J");
}

void SimulationUI::renderUIText(Simulation *sim)
{
    if (sim->isFollowCam())
    {
        setObjectDetails(sim->getFocusedObject());
        setSpeedDetails(sim->getFocusedObject());
        setEnergyDetails(sim->getFocusedObject());
        if (sim->getFocusedObject()->isOrbiting())
            setOrbitalDetails(sim);
    }

    focusPanel.setTitle(nameText, glm::vec2(scale.getGLM()));

    auto focusIcon = std::make_unique<UIIcon>("../assets/Icons/plus.png", glm::vec2(48) * glm::vec2(scale.getGLM()), glm::vec2(0, 0), glm::vec4(0.0f));
    focusIcon->setHoverColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.2f));
    focusPanel.addButton(std::move(focusIcon), UIText("", font, glm::vec2(scale.getGLM())), TopRight);

    focusPanel.addText(typeText, glm::vec2(scale.getGLM()));
    focusPanel.addText(radiusText, glm::vec2(scale.getGLM()));
    focusPanel.addText(massText, glm::vec2(scale.getGLM()));
    focusPanel.addText(velocityText, glm::vec2(scale.getGLM()));
    focusPanel.wrap();

    if (uiState == SIM)
    {
        std::string timePassedText = getTimePassed(sim->getTimePassed(), sim->getTimeSpeed());
        UIText timePassed = UIText(timePassedText, font, glm::vec2(scale.getGLM()));
        timePassed.setPos(WIDTH / 2 - timePassed.getWidth() / 2, 25);
        timePassed.renderText();
    }

    cameraSpeedText.setPos(std::min(infoPanel.getX(), focusPanel.getX()) - cameraSpeedText.getWidth() - 10, HEIGHT - cameraSpeedText.getHeight() - 10);
    cameraSpeedText.renderText();

    if (uiState == EDIT && sim->getSelectedObject() != nullptr)
    {
        std::stringstream dirSS;
        UIText movingObjTxt = UIText("", font, glm::vec2(scale.getGLM()));

        if (sim->getMoveType() == 0)
            movingObjTxt.setText("Moving on XZ Plane");
        if (sim->getMoveType() == 1)
            movingObjTxt.setText("Moving along Y Axis");
        if (sim->getMoveType() == 2 || sim->getMoveType() == 3)
        {
            dirSS << "Direction: " << sim->getSelectedObject()->getVelocity().normalized();
            movingObjTxt.setText(dirSS.str());
        }

        if (cameraSpeedText.getText() != "")
            movingObjTxt.setPos(WIDTH - movingObjTxt.getWidth() - 20, cameraSpeedText.getY() - movingObjTxt.getHeight() - 10);
        else
            movingObjTxt.setPos(WIDTH - movingObjTxt.getWidth() - 20, HEIGHT - movingObjTxt.getHeight() - 10);

        movingObjTxt.renderText();
    }

    std::string preText = "";
    if (sim->isPaused())
        preText = "Simulation Paused  -  ";

    UIText cameraText = UIText("Camera: ", font, glm::vec2(scale.getGLM()));
    if (!sim->isFollowCam())
    {
        if (sim->getSelectedObject() != nullptr)
            cameraText.setText("Editing: " + sim->getSelectedObject()->getName() + "  -  Camera: Free Cam").setPos(cameraIcon->getEndX() + 25, HEIGHT - cameraText.getHeight() - 20).renderText();
        else
            cameraText.setText(preText + "Camera: Free Cam").setPos(10, HEIGHT - cameraText.getHeight() - 20).renderText();
        return;
    }

    cameraText.setText(preText + "Camera: Follow Cam").setPos(cameraIcon->getEndX() + 25, HEIGHT - cameraText.getHeight() - 20).renderText();
}

void SimulationUI::renderInfoText(Simulation *sim)
{
    std::unique_ptr<UIRect> viewRect;
    if (sim->isFollowCam())
    {
        infoPanel.addIcon("../assets/Icons/" + sim->getFocusedObject()->getObjTypeStr() + "s.png", glm::vec2(56) * glm::vec2(scale.getGLM()));
        auto infoIcon = std::make_unique<UIIcon>("../assets/Icons/minus.png", glm::vec2(48) * glm::vec2(scale.getGLM()), glm::vec2(0, 0), glm::vec4(0.0f));
        infoIcon->setHoverColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.2f));
        infoPanel.addButton(std::move(infoIcon), UIText("", font, glm::vec2(scale.getGLM())), TopRight);
        infoPanel.setTitle((UIText(sim->getFocusedObject()->getName(), titleFont, glm::vec2(scale.getGLM()))), glm::vec2(scale.getGLM()));

        viewRect = std::make_unique<UIRect>(glm::vec4(0.0, 0.0, 0.0, 0.0));
        viewRect->setSize(glm::vec2(infoPanel.getWidth() - 20, infoPanel.getHeight() / 15));
        viewRect->setWrap(false, true);
        viewRect->setScroll(infoPanel.getPanelScroll());
        viewRect->setTitle(UIText("Object Details", bigFont, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));
        viewRect->addIcon("../assets/Icons/object.png", glm::vec2(50) * glm::vec2(scale.getGLM()));
        viewRect->addText(typeText, glm::vec2(scale.getGLM()));
        viewRect->addText(radiusText, glm::vec2(scale.getGLM()));
        viewRect->addText(massText, glm::vec2(scale.getGLM()));
        viewRect->addText(velocityText, glm::vec2(scale.getGLM()));
        infoPanel.addRect(std::move(viewRect), glm::vec2(scale.getGLM()));

        std::unique_ptr<UIIcon> netForceIcon, trajectoryIcon;
        if (sim->getFocusedObject()->isNetForceVector())
            netForceIcon = std::make_unique<UIIcon>("../assets/Icons/check.png", glm::vec2(50) * glm::vec2(scale.getGLM()), glm::vec2(0.0), glm::vec4(0.0f));
        else
            netForceIcon = std::make_unique<UIIcon>("../assets/Icons/x.png", glm::vec2(50) * glm::vec2(scale.getGLM()), glm::vec2(0.0), glm::vec4(0.0f));
        if (sim->getFocusedObject()->isTrajectoryVector())
            trajectoryIcon = std::make_unique<UIIcon>("../assets/Icons/check.png", glm::vec2(50) * glm::vec2(scale.getGLM()), glm::vec2(0.0), glm::vec4(0.0f));
        else
            trajectoryIcon = std::make_unique<UIIcon>("../assets/Icons/x.png", glm::vec2(50) * glm::vec2(scale.getGLM()), glm::vec2(0.0), glm::vec4(0.0f));

        netForceIcon->setText("Net Force Vector: ", font, glm::vec2(scale.getGLM()));
        trajectoryIcon->setText("Trajectory Vector: ", font, glm::vec2(scale.getGLM()));
        viewRect = std::make_unique<UIRect>(glm::vec4(0.0, 0.0, 0.0, 0.0));
        viewRect->setSize(glm::vec2(infoPanel.getWidth() - 20, infoPanel.getHeight() / 15));
        viewRect->setWrap(false, true);
        viewRect->setScroll(infoPanel.getPanelScroll());
        viewRect->setTitle(UIText("Velocity & Forces", bigFont, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));
        viewRect->addIcon("../assets/Icons/velocity.png", glm::vec2(50) * glm::vec2(scale.getGLM()));
        viewRect->addText(angularVelText, glm::vec2(scale.getGLM()));
        viewRect->addText(rotationalPeriodText, glm::vec2(scale.getGLM()));
        viewRect->addText(circularVelText, glm::vec2(scale.getGLM()));
        viewRect->addText(surfaceEscapeVelText, glm::vec2(scale.getGLM()));
        viewRect->addText(accelerationText, glm::vec2(scale.getGLM()));
        viewRect->addText(surfaceGravityText, glm::vec2(scale.getGLM()));
        viewRect->addText(netForceText, glm::vec2(scale.getGLM()));
        viewRect->addBodyIcon(std::move(netForceIcon));
        viewRect->addBodyIcon(std::move(trajectoryIcon));
        infoPanel.addRect(std::move(viewRect), glm::vec2(scale.getGLM()));

        if (sim->getFocusedObject()->isOrbiting())
        {
            viewRect = std::make_unique<UIRect>(glm::vec4(0.0, 0.0, 0.0, 0.0));
            viewRect->setSize(glm::vec2(infoPanel.getWidth() - 20, infoPanel.getHeight() / 15));
            viewRect->setWrap(false, true);
            viewRect->setScroll(infoPanel.getPanelScroll());
            viewRect->setTitle(UIText("Orbital Details", bigFont, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));
            viewRect->addIcon("../assets/Icons/orbit.png", glm::vec2(50) * glm::vec2(scale.getGLM()));
            viewRect->addText(centralBodyText, glm::vec2(scale.getGLM()));
            viewRect->addText(eccentricityText, glm::vec2(scale.getGLM()));
            viewRect->addText(semiMajorAxisText, glm::vec2(scale.getGLM()));
            viewRect->addText(apoapsisText, glm::vec2(scale.getGLM()));
            viewRect->addText(periapsisText, glm::vec2(scale.getGLM()));
            viewRect->addText(angularMomentumText, glm::vec2(scale.getGLM()));
            viewRect->addText(inclinationText, glm::vec2(scale.getGLM()));
            viewRect->addText(periodText, glm::vec2(scale.getGLM()));
            viewRect->addText(escapeVelText, glm::vec2(scale.getGLM()));
            infoPanel.addRect(std::move(viewRect), glm::vec2(scale.getGLM()));
        }

        viewRect = std::make_unique<UIRect>(glm::vec4(0.0, 0.0, 0.0, 0.0));
        viewRect->setSize(glm::vec2(infoPanel.getWidth() - 20, infoPanel.getHeight() / 15));
        viewRect->setWrap(false, true);
        viewRect->setScroll(infoPanel.getPanelScroll());
        viewRect->setTitle(UIText("Energy", bigFont, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));
        viewRect->addIcon("../assets/Icons/energy.png", glm::vec2(50) * glm::vec2(scale.getGLM()));
        viewRect->addText(potentialText, glm::vec2(scale.getGLM()));
        viewRect->addText(kineticText, glm::vec2(scale.getGLM()));
        viewRect->addText(totalEnergyText, glm::vec2(scale.getGLM()));
        infoPanel.addRect(std::move(viewRect), glm::vec2(scale.getGLM()));

        infoPanel.setLines();
    }
}

void SimulationUI::renderLoadingScreen()
{
    if (round(loadingTimer.getTicksNS() / 1'000'000'000.0f) >= 0.75)
    {
        if (loadString == "Loading Simulation...")
            loadString = "Loading Simulation.";
        else if (loadString == "Loading Simulation..")
            loadString = "Loading Simulation...";
        else if (loadString == "Loading Simulation.")
            loadString = "Loading Simulation..";
        else if (loadString == "Loading Simulation")
            loadString = "Loading Simulation.";

        loadingTimer.stop();
        loadingTimer.start();
    }

    UIText titleText = UIText(loadString, biggestFont, glm::vec2(scale.getGLM()));
    titleText.setPos(WIDTH / 2 - titleText.getWidth() / 2, HEIGHT / 2 - titleText.getHeight() / 2);
    titleText.renderText();
}

void SimulationUI::renderHelpMenu()
{
    UIText titleText = UIText("Help", bigFont, glm::vec2(scale.getGLM()));
    titleText.setPos(15, titleText.getHeight());
    UIText helpControls = UIText("ESCAPE: Controls", font, glm::vec2(scale.getGLM())).setPos(25, titleText.getEndY() + 15);
    UIText cameraControls = UIText("WASD, Space, Shift: Move Camera Around", font, glm::vec2(scale.getGLM())).setPos(25, helpControls.getEndY() + 10);
    UIText speedSimControls = UIText("CTRL Plus / Minus: Speed up/Slow down Simulation", font, glm::vec2(scale.getGLM())).setPos(25, cameraControls.getEndY() + 10);
    UIText pauseControls = UIText("CTRL P: Pause the Simulation", font, glm::vec2(scale.getGLM())).setPos(25, speedSimControls.getEndY() + 10);
    UIText followControls = UIText("CTRL + Left Arrow / Right Arrow: Follow Object", font, glm::vec2(scale.getGLM())).setPos(25, pauseControls.getEndY() + 10);
    UIText stopFollowControls = UIText("CTRL Backspace: Stop Following Object", font, glm::vec2(scale.getGLM())).setPos(25, followControls.getEndY() + 10);
    UIText speedCamControls = UIText("Left Arrow / Right Arrow: Speed up / Slow down camera", font, glm::vec2(scale.getGLM())).setPos(25, stopFollowControls.getEndY() + 10);
    UIText dupControls = UIText("CTRL D: (While editing an object) Duplicate the object", font, glm::vec2(scale.getGLM())).setPos(25, speedCamControls.getEndY() + 10);
    UIText delControls = UIText("DEL: (While editing an object) Delete the object", font, glm::vec2(scale.getGLM())).setPos(25, dupControls.getEndY() + 10);

    titleText.renderText();
    helpControls.renderText();
    cameraControls.renderText();
    speedSimControls.renderText();
    pauseControls.renderText();
    followControls.renderText();
    stopFollowControls.renderText();
    speedCamControls.renderText();
    dupControls.renderText();
    delControls.renderText();
}

void SimulationUI::renderSimulationOverlay(Simulation *sim, EventManager *eventManager, float deltaTime)
{
    cursorPointer = false;
    scrollingPointer = false;

    if (helpMenu)
        renderHelpMenu();
    else
    {
        if (uiState == EDIT || uiState == ADD1 || uiState == ADD2)
            UiRenderer->drawRect(editScreen);

        viewPanel.addIcon("../assets/Icons/view.png", glm::vec2(56) * glm::vec2(scale.getGLM()));
        viewPanel.setTitle(UIText("View Objects", titleFont, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));

        if (uiState == ADD1)
        {
            addPanel.setTitle(UIText("Add Object", titleFont, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));
            addPanel.addIcon("../assets/Icons/planets.png", glm::vec2(56) * glm::vec2(scale.getGLM()));

            for (int i = 0; i < celestialTypes.size(); i++)
            {
                std::unique_ptr<UIRect> addRect = std::make_unique<UIRect>(glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
                addRect->setHoverColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
                addRect->setSize(glm::vec2(addPanel.getWidth() - 20, 200));
                addRect->setCornerRadius(0.3f);
                addRect->setScroll(addPanel.getPanelScroll());
                addRect->setWrap(false, true);

                addRect->setTitle(UIText(celestialTypes[i], titleFont, glm::vec2(scale.getGLM()) * glm::vec2(0.8)), glm::vec2(scale.getGLM()));
                addRect->addIcon("../assets/Icons/" + celestialTypes[i] + ".png", glm::vec2(50) * glm::vec2(scale.getGLM()));
                if (i == 0)
                    addRect->addText(UIText(std::to_string(starPaths.size()) + " " + celestialTypes[i] + " Available", font, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));
                if (i == 1)
                    addRect->addText(UIText(std::to_string(planetPaths.size()) + " " + celestialTypes[i] + " Available", font, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));
                if (i == 2)
                    addRect->addText(UIText(std::to_string(moonPaths.size()) + " " + celestialTypes[i] + " Available", font, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));

                addPanel.addRect(std::move(addRect), glm::vec2(scale.getGLM()));
            }
        }
        else if (uiState == ADD2)
        {
            addPanel.setTitle(UIText("Add " + addType, titleFont, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));
            addPanel.addIcon("../assets/Icons/" + addType + ".png", glm::vec2(56) * glm::vec2(scale.getGLM()));

            int x;
            if (addType == "Stars")
                x = starPaths.size();
            else if (addType == "Planets")
                x = planetPaths.size();
            else
                x = moonPaths.size();

            for (int i = 0; i < x; i++)
            {
                std::unique_ptr<UIRect> typeRect = std::make_unique<UIRect>(glm::vec2(0), glm::vec2(10, 0), glm::vec4(1.0, 1.0, 1.0, 0.0f));
                typeRect->setHoverColor(glm::vec4(1.0, 1.0, 1.0, 0.3));
                addPanel.addImageRect(std::move(typeRect));
            }
        }

        if (sim->getSelectedObject() == nullptr)
        {
            editPanel.setTitle(UIText("Edit Object", titleFont, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));
            editPanel.addIcon("../assets/Icons/edit.png", glm::vec2(56) * glm::vec2(scale.getGLM()));
        }
        else
        {
            if (sim->isMovingObject() && (sim->getMoveType() != 2 && sim->getMoveType() != 3))
            {
                editPanel.setTitle(UIText("Editing Objects", titleFont, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));
                editPanel.addIcon("../assets/Icons/edit.png", glm::vec2(56) * glm::vec2(scale.getGLM()));

                std::unique_ptr<UIRect> viewRect;
                std::vector<CelestialObject *> &objects = sim->getObjectsByDistance(sim->getSelectedObject());
                for (int i = 0; i < objects.size(); i++)
                {
                    CelestialObject *obj = objects[i];
                    if (obj == sim->getSelectedObject())
                        continue;

                    viewRect = std::make_unique<UIRect>(glm::vec4(1.0, 1.0, 1.0, 0.4));
                    viewRect->setHoverColor(glm::vec4(1.0, 1.0, 1.0, 0.6));
                    viewRect->setSize(glm::vec2(editPanel.getWidth() - 20, editPanel.getHeight() / 15));
                    viewRect->setWrap(false, true);
                    viewRect->setScroll(editPanel.getPanelScroll());
                    viewRect->addIcon("../assets/Icons/" + obj->getObjTypeStr() + "s.png", glm::vec2(48));
                    viewRect->setTitle(UIText(objects[i]->getName(), bigFont, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));
                    viewRect->addText(UIText(addCommas(removeTrailingZeroes(std::to_string(sim->getDistance(obj, sim->getSelectedObject()) / 1000))) + " km from " + sim->getSelectedObject()->getName(), font, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));

                    editPanel.addRect(std::move(viewRect), glm::vec2(scale.getGLM()));
                }
            }
            else
            {
                for (int i = 0; i < editPanelFields.size(); i++)
                    editPanelFields[i].cursorOn = (int)(cursorTimer.getTicksNS() / 1'000'000'000.0f) % 2 == 0;

                editPanel.addIcon("../assets/Icons/back.png", glm::vec2(50) * glm::vec2(scale.getGLM()));
                editPanel.setTitle(UIText("Editing " + sim->getSelectedObject()->getName(), titleFont, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));

                std::unique_ptr<UIRect> editRect = std::make_unique<UIRect>(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
                editRect->setSize(glm::vec2(editPanel.getWidth() - 20, editPanel.getHeight() / 15));
                editRect->setScroll(editPanel.getPanelScroll());
                editRect->setWrap(false, true);

                nameTextField = std::make_unique<UITextField>(editPanelFields[0], glm::vec2(0.0f), glm::vec2(0.0f), UIText("", font, glm::vec2(scale.getGLM()), 0, 0), glm::vec4(1.0f));
                radiusTextField = std::make_unique<UITextField>(editPanelFields[1], glm::vec2(0.0f), glm::vec2(0.0f), UIText("km", font, glm::vec2(scale.getGLM()), 0, 0), glm::vec4(1.0f));
                massTextField = std::make_unique<UITextField>(editPanelFields[2], glm::vec2(0.0f), glm::vec2(0.0f), UIText("kg", font, glm::vec2(scale.getGLM()), 0, 0), glm::vec4(1.0f));
                orbVelocityTextField = std::make_unique<UITextField>(editPanelFields[3], glm::vec2(0.0f), glm::vec2(0.0f), UIText("m/s", font, glm::vec2(scale.getGLM()), 0, 0), glm::vec4(1.0f));
                tiltTextField = std::make_unique<UITextField>(editPanelFields[4], glm::vec2(0.0f), glm::vec2(0.0f), UIText("degrees", font, glm::vec2(scale.getGLM()), 0, 0), glm::vec4(1.0f));
                rotPeriodTextField = std::make_unique<UITextField>(editPanelFields[5], glm::vec2(0.0f), glm::vec2(0.0f), UIText("s", font, glm::vec2(scale.getGLM()), 0, 0), glm::vec4(1.0f));
                starIntensityTextField = std::make_unique<UITextField>(editPanelFields[6], glm::vec2(0.0f), glm::vec2(0.0f), UIText("", font, glm::vec2(scale.getGLM()), 0, 0), glm::vec4(1.0f));

                editRect->addTextField(std::move(nameTextField), glm::vec2(scale.getGLM()));
                editRect->addTextField(std::move(radiusTextField), glm::vec2(scale.getGLM()));
                editRect->addTextField(std::move(massTextField), glm::vec2(scale.getGLM()));
                editRect->addTextField(std::move(orbVelocityTextField), glm::vec2(scale.getGLM()));
                editRect->addTextField(std::move(tiltTextField), glm::vec2(scale.getGLM()));
                editRect->addTextField(std::move(rotPeriodTextField), glm::vec2(scale.getGLM()));
                if (sim->getSelectedObject()->getObjType() == STAR)
                    editRect->addTextField(std::move(starIntensityTextField), glm::vec2(scale.getGLM()));

                editPanel.addRect(std::move(editRect), glm::vec2(scale.getGLM()));

                // if (!error)
                // {
                //     bool updated = false;
                //     try
                //     {
                //         if (editPanelFields[0].text.getText() != sim->getSelectedObject()->getName() || std::stold(editPanelFields[1].text.getText()) * 1000 != sim->getSelectedObject()->getRadius())
                //             updated = true;
                //         if (std::stold(editPanelFields[2].text.getText()) != sim->getSelectedObject()->getMass() || std::stold(editPanelFields[4].text.getText()) != static_cast<long double>(sim->getSelectedObject()->getVelocity().magnitude))
                //             updated = true;
                //         if (std::stof(editPanelFields[3].text.getText()) != sim->getSelectedObject()->getTilt() || std::stof(editPanelFields[5].text.getText()) != sim->getSelectedObject()->getRotationalPeriod())
                //             updated = true;
                //     }
                //     catch (const std::exception &e)
                //     {
                //     }
                // }
            }
        }

        for (int i = 0; i < icons.size(); i++)
        {
            if (i == 0)
                icons[i]->setPos(glm::vec2(std::max({viewPanel.getEndX(), editPanel.getEndX(), addPanel.getEndX()}) + 15, 10));
            if (i == 1)
                icons[i]->setPos(glm::vec2(icons[i - 1]->getEndX() + 30, 10));
            else if (i > 1)
                icons[i]->setPos(glm::vec2(icons[i - 1]->getEndX() + 10, 10));

            UiRenderer->drawIcon(*(icons[i]));
        }

        addIcon->setPos(glm::vec2(std::max({viewPanel.getEndX(), editPanel.getEndX(), addPanel.getEndX()}) + 15, icons[0]->getEndY() + 20));
        UiRenderer->drawIcon(*addIcon);

        editIcon->setPos(glm::vec2(std::max({viewPanel.getEndX(), editPanel.getEndX(), addPanel.getEndX()}) + 15, addIcon->getEndY() + 20));
        UiRenderer->drawIcon(*editIcon);

        if (sim->isFollowCam() || sim->getSelectedObject() != nullptr)
        {
            cameraIcon->setPos(glm::vec2(15, HEIGHT - cameraIcon->getHeight() - 15));
            UiRenderer->drawIcon(*cameraIcon);
        }

        if (uiState == ADD1 || uiState == ADD2)
            addPanel.enter(deltaTime);
        else
            addPanel.exit(deltaTime);

        if (uiState == EDIT)
            editPanel.enter(deltaTime);
        else
            editPanel.exit(deltaTime);

        if (menu)
            viewPanel.enter(deltaTime);
        else
            viewPanel.exit(deltaTime);

        renderUIText(sim);
        renderInfoText(sim);

        std::unique_ptr<UIRect> viewRect, editRect;
        std::vector<CelestialObject *> &objects = sim->getObjectsByDistance((sim->isFollowCam()) ? sim->getFocusedObject() : nullptr);
        for (int i = 0; i < objects.size(); i++)
        {
            CelestialObject *obj = objects[i];
            viewRect = std::make_unique<UIRect>(glm::vec4(1.0, 1.0, 1.0, 0.4));
            viewRect->setHoverColor(glm::vec4(1.0, 1.0, 1.0, 0.6));
            viewRect->setSize(glm::vec2(viewPanel.getWidth() - 20, viewPanel.getHeight() / 15));
            viewRect->setWrap(false, true);
            viewRect->setScroll(viewPanel.getPanelScroll());
            viewRect->addIcon("../assets/Icons/" + obj->getObjTypeStr() + "s.png", glm::vec2(48) * glm::vec2(scale.getGLM()));
            viewRect->setTitle(UIText(objects[i]->getName(), bigFont, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));

            if (sim->isFollowCam())
                viewRect->addText(UIText(addCommas(removeTrailingZeroes(std::to_string(sim->getDistance(obj, sim->getFocusedObject()) / 1000))) + " km from " + sim->getFocusedObject()->getName(), font, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));
            else
                viewRect->addText(UIText(addCommas(removeTrailingZeroes(std::to_string(sim->getDistance(obj, sim->getFocusedObject()) / 1000))) + " km from Camera", font, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));

            viewPanel.addRect(std::move(viewRect), glm::vec2(scale.getGLM()));

            if (sim->getSelectedObject() == nullptr)
            {
                setObjectDetails(objects[i]);
                editRect = std::make_unique<UIRect>(glm::vec4(1.0, 1.0, 1.0, 0.4));
                editRect->setHoverColor(glm::vec4(1.0, 1.0, 1.0, 0.6));
                editRect->setSize(glm::vec2(viewPanel.getWidth() - 20, viewPanel.getHeight() / 15));
                editRect->setWrap(false, true);
                editRect->setScroll(editPanel.getPanelScroll());
                editRect->addIcon("../assets/Icons/" + obj->getObjTypeStr() + "s.png", glm::vec2(48));
                editRect->setTitle(UIText(objects[i]->getName(), bigFont, glm::vec2(scale.getGLM())), glm::vec2(scale.getGLM()));
                editRect->addText(radiusText, glm::vec2(scale.getGLM()));
                editRect->addText(massText, glm::vec2(scale.getGLM()));
                editRect->addText(velocityText, glm::vec2(scale.getGLM()));

                editPanel.addRect(std::move(editRect), glm::vec2(scale.getGLM()));
            }
        }

        viewPanel.setPositions(glm::vec2(scale.getGLM()));
        addPanel.setPositions(glm::vec2(scale.getGLM()));
        editPanel.setPositions(glm::vec2(scale.getGLM()));
        focusPanel.setPositions(glm::vec2(scale.getGLM()));
        infoPanel.setPositions(glm::vec2(scale.getGLM()));

        if (sim->isFollowCam())
        {
            if (moreInfo)
            {
                infoPanel.enter(deltaTime);
                focusPanel.exit(deltaTime);
            }
            else
            {
                focusPanel.enter(deltaTime);
                infoPanel.exit(deltaTime);
            }
        }
        else
        {
            infoPanel.exit(deltaTime);
            focusPanel.exit(deltaTime);
        }
    }

    handleEvents(sim, eventManager, deltaTime);

    if (!helpMenu)
    {
        UiRenderer->drawPanel(viewPanel);
        UiRenderer->drawPanel(addPanel);
        UiRenderer->drawPanel(editPanel);
        UiRenderer->drawPanel(focusPanel);
        UiRenderer->drawPanel(infoPanel);

        sim->setRenderMatrices(modelShader, glm::mat4(1.0f), glm::ortho(0.0f, WIDTH, HEIGHT, 0.0f, -1.0f, 1.0f));
        for (int i = 0; i < addPanel.getImageRects().size(); i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            glm::vec2 pos = addPanel.getImageRects()[i]->getPos();
            float radius = (addPanel.getImageRects()[i]->getWidth() - 10) / 2;
            model = glm::translate(model, glm::vec3(pos.x + addPanel.getImageRects()[i]->getWidth() / 2, pos.y + addPanel.getImageRects()[i]->getWidth() / 2, 0.0));

            if (addType == "Stars")
            {
                model = glm::scale(model, glm::vec3(radius / starPaths[i].getModelRadius()));
                modelShader.setMat4("model", model);
                starPaths[i].draw(modelShader);
            }
            else if (addType == "Planets")
            {
                model = glm::scale(model, glm::vec3(radius / planetPaths[i].getModelRadius()));
                modelShader.setMat4("model", model);
                planetPaths[i].draw(modelShader);
            }
            else if (addType == "Moons")
            {
                model = glm::scale(model, glm::vec3(radius / moonPaths[i].getModelRadius()));
                modelShader.setMat4("model", model);
                moonPaths[i].draw(modelShader);
            }
        }

        viewPanel.clearRect();
        addPanel.clearRect();
        editPanel.clearRect();
        focusPanel.clearText();
        infoPanel.clearRect();
    }
}
