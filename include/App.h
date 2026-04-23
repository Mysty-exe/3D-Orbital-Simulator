#include <iostream>
#include <fstream>
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <vector>
#include "EventManager.h"
#include "UI/SimulationUI.h"

class App
{
private:
    SDL_Window *window;
    SDL_GLContext glContext;
    int WIDTH, HEIGHT;
    bool running;

    Timer deltaTimer;
    Uint64 renderingNS;
    double deltaTime, fps;

    EventManager *eventManager;
    Simulation *sim;
    SimulationUI *simUI;
    Shader shaderBlur, shaderBloom, skyboxShader;

    SDL_Cursor *defaultCursor, *pointerCursor;

public:
    App(int W, int H) : WIDTH(W), HEIGHT(H)
    {
        srand(static_cast<unsigned int>(time(0)));

        SDL_Init(SDL_INIT_VIDEO);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

        window = SDL_CreateWindow("Orbital Simulator", WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

        int w, h, c;
        unsigned char *data = stbi_load("assets/Icons/simIcon.png", &w, &h, &c, 4);
        SDL_Surface *surface = SDL_CreateSurfaceFrom(
            w,
            h,
            SDL_PIXELFORMAT_RGBA32,
            data,
            w * 4);

        SDL_SetWindowIcon(window, surface);
        SDL_DestroySurface(surface);
        stbi_image_free(data);

        glContext = SDL_GL_CreateContext(window);
        gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
        SDL_GetWindowSizeInPixels(window, &WIDTH, &HEIGHT);
        glViewport(0, 0, WIDTH, HEIGHT);
        SDL_StartTextInput(window);

        glEnable(GL_MULTISAMPLE);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        stbi_set_flip_vertically_on_load(false);

        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        defaultCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
        pointerCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);

        eventManager = new EventManager();
        sim = new Simulation(WIDTH, HEIGHT);
        simUI = new SimulationUI(WIDTH, HEIGHT);
        running = true;
    }

    void run()
    {
        float quadVertices[] = {
            -1.0f,
            1.0f,
            0.0f,
            1.0f,
            -1.0f,
            -1.0f,
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            1.0f,
            -1.0f,
            1.0f,
            0.0f,
        };

        float skyboxVertices[] = {
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f};

        unsigned int quadVAO, quadVBO;
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

        unsigned int skyboxVAO, skyboxVBO;
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

        std::vector<std::string> faces{
            "assets/Skybox/right.png",
            "assets/Skybox/left.png",
            "assets/Skybox/top.png",
            "assets/Skybox/bottom.png",
            "assets/Skybox/front.png",
            "assets/Skybox/back.png"};
        unsigned int cubemapTexture = loadCubemap(faces);

        shaderBlur = Shader("shaders/Bloom/blur.vert", "shaders/Bloom/blur.frag", "");
        shaderBloom = Shader("shaders/Bloom/screen.vert", "shaders/Bloom/screen.frag", "");
        skyboxShader = Shader("shaders/SkyBox/skybox.vert", "shaders/SkyBox/skybox.frag", "");

        shaderBlur.use();
        shaderBlur.setInt("image", 0);
        shaderBloom.use();
        shaderBloom.setInt("scene", 0);
        shaderBloom.setInt("bloom", 1);
        shaderBloom.setFloat("exposure", 1.2);
        shaderBloom.setInt("bloomBlur", 1);
        skyboxShader.use();
        skyboxShader.setInt("skybox", 0);

        unsigned int hdrFBO;
        glGenFramebuffers(1, &hdrFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

        unsigned int colorBuffers[2];
        glGenTextures(2, colorBuffers);
        for (unsigned int i = 0; i < 2; i++)
        {
            glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
        }

        unsigned int rboDepth;
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

        unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2, attachments);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)

            std::cout << "Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        unsigned int pingpongFBO[2];
        unsigned int pingpongColorbuffers[2];
        glGenFramebuffers(2, pingpongFBO);
        glGenTextures(2, pingpongColorbuffers);
        for (unsigned int i = 0; i < 2; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
            glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "Framebuffer not complete!" << std::endl;
        }

        while (running)
        {
            deltaTimer.start();
            deltaTime = renderingNS / 1'000'000'000.0f;

            eventManager->getEvents();

            if (eventManager->userQuit())
            {
                running = false;
            }
            else if (eventManager->changedWindowSize())
            {
                // sim->resize(eventManager->getWidth(), eventManager->getHeight());
            }

            if (simUI->gettingModels())
            {
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                simUI->renderLoadingScreen();
                SDL_GL_SwapWindow(window);
                capFrameRate();
                continue;
            }

            if (simUI->canLoadModels())
                simUI->loadModels();

            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
            glDepthMask(GL_TRUE);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);

            if (!simUI->isHelp())
                sim->run(window, eventManager, simUI, deltaTime);

            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glDepthMask(GL_FALSE);
            glDepthFunc(GL_LEQUAL);
            skyboxShader.use();
            glm::mat4 view = glm::mat4(glm::mat3(sim->getCamera()->getViewMatrix()));
            skyboxShader.setMat4("view", view);
            skyboxShader.setMat4("projection", sim->getProjection());
            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glDisable(GL_DEPTH_TEST);

            bool horizontal = true, first_iteration = true;
            unsigned int amount = 10;
            shaderBlur.use();
            for (unsigned int i = 0; i < amount; i++)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
                shaderBlur.setInt("horizontal", horizontal);
                glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);

                glBindVertexArray(quadVAO);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glBindVertexArray(0);

                horizontal = !horizontal;
                if (first_iteration)
                    first_iteration = false;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            shaderBloom.use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);

            glDisable(GL_DEPTH_TEST);
            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glDisable(GL_DEPTH_TEST);
            simUI->renderSimulationOverlay(sim, eventManager, deltaTime);

            sim->setEditing(simUI->currentState() == EDIT);

            if (simUI->isPointing() || sim->isHovering())
                SDL_SetCursor(pointerCursor);
            else
                SDL_SetCursor(defaultCursor);

            SDL_GL_SwapWindow(window);

            capFrameRate();

            eventManager->clear();
        }
    }

    unsigned int loadCubemap(std::vector<std::string> faces)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrComponents;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                GLenum format = (nrComponents == 4) ? GL_RGBA : GL_RGB;
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            }
            else
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

    void capFrameRate()
    {
        if (renderingNS != 0)
        {
            fps = 1000000000.0 / renderingNS;
        }

        renderingNS = deltaTimer.getTicksNS();

        constexpr Uint64 nsPerFrame = 1000000000.0 / 60;
        if (renderingNS < nsPerFrame)
        {
            Uint64 sleepTime = nsPerFrame - renderingNS;
            SDL_DelayNS(nsPerFrame - renderingNS);

            renderingNS = deltaTimer.getTicksNS();
        }
    }

    void destroy()
    {
        delete eventManager;
        delete sim;
        delete simUI;

        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);

        SDL_Quit();
    }

    SDL_Window *getWindow() const { return window; }
    int getWidth() const { return WIDTH; }
    int getHeight() const { return HEIGHT; }
    bool isRunning() const { return running; }
    void stop() { running = false; }
};