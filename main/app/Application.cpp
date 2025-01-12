#include "Application.hpp"

GLFWwindow* Application::window = nullptr;
Scene* Application::scene = nullptr;

GLFWwindow* Application::initGlfwWindow(const char* windowName) {
    if (!glfwInit())
        throw "Glfw initialization error!\n";

    GLFWwindow* window;
    window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw "Glfw window creation error!\n";
    }
    glfwMakeContextCurrent(window);
    return window;
}

void Application::initOpenGl() {
    GLenum err = glewInit();
    std::cout << "Glew message: " << glewGetErrorString(err) << std::endl << std::endl;
    Debug::displayOpenGlInfo();
    Debug::initializeDebugContext();

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    glDebugMessageCallback(&Debug::outputDebugMessage, (const void*)NULL);
}

void Application::initialize() {
    window = initGlfwWindow("Simulation");
    initOpenGl();

    scene = new Scene();
    scene->create();

    Gui::initialize(window, scene);
    DebugDrawer::initProgram();
}

void Application::render() {
    while (!glfwWindowShouldClose(window)) {
        Time::calculateCurrentTime();

        if (Time::currentTime - Time::previousTime < Time::frameTime)
            continue;

        Time::calculateDeltaTime();
        Time::previousTime = Time::currentTime;

        glfwPollEvents();
        glViewport(0, 0, sceneWidth, sceneHeight);
        glClearColor(0.5f, 0.5f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        scene->update(window);
        scene->draw();

        Gui::render(sceneWidth, windowWidth, windowHeight);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

void Application::close() {
    delete scene;
    DebugDrawer::deleteProgram();
    glfwTerminate();
}