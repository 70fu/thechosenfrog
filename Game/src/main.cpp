/*
* Copyright 2018 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/

#include <sstream>
#include <string>
#include "Game.h"
#include "util/INIReader.h"
#include "Constants.h"
#include "IEventManager.h"

static Game game=Game();

//helper method to generate an error message from the debug callback arguments
static std::string formatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg)
{
    std::stringstream stringStream;
    std::string sourceString;
    std::string typeString;
    std::string severityString;

    switch (source) {
        case GL_DEBUG_SOURCE_API: {
            sourceString = "API";
            break;
        }
        case GL_DEBUG_SOURCE_APPLICATION: {
            sourceString = "Application";
            break;
        }
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
            sourceString = "Window System";
            break;
        }
        case GL_DEBUG_SOURCE_SHADER_COMPILER: {
            sourceString = "Shader Compiler";
            break;
        }
        case GL_DEBUG_SOURCE_THIRD_PARTY: {
            sourceString = "Third Party";
            break;
        }
        case GL_DEBUG_SOURCE_OTHER: {
            sourceString = "Other";
            break;
        }
        default: {
            sourceString = "Unknown";
            break;
        }
    }

    switch (type) {
        case GL_DEBUG_TYPE_ERROR: {
            typeString = "Error";
            break;
        }
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
            typeString = "Deprecated Behavior";
            break;
        }
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
            typeString = "Undefined Behavior";
            break;
        }
        /*case GL_DEBUG_TYPE_PORTABILITY_ARB: {
            typeString = "Portability";
            break;
        }*///TODO why is that not working
        case GL_DEBUG_TYPE_PERFORMANCE: {
            typeString = "Performance";
            break;
        }
        case GL_DEBUG_TYPE_OTHER: {
            typeString = "Other";
            break;
        }
        default: {
            typeString = "Unknown";
            break;
        }
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: {
            severityString = "High";
            break;
        }
        case GL_DEBUG_SEVERITY_MEDIUM: {
            severityString = "Medium";
            break;
        }
        case GL_DEBUG_SEVERITY_LOW: {
            severityString = "Low";
            break;
        }
        default: {
            severityString = "Unknown";
            break;
        }
    }

    stringStream << "OpenGL Error: " << msg;
    stringStream << " [Source = " << sourceString;
    stringStream << ", Type = " << typeString;
    stringStream << ", Severity = " << severityString;
    stringStream << ", ID = " << id << "]";

    return stringStream.str();
}

/* --------------------------------------------- */
// Prototypes & callbacks
/* --------------------------------------------- */
static void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam)
{
    std::string error = formatDebugOutput(source, type, id, severity, message);
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
        case GL_DEBUG_SEVERITY_MEDIUM: {
            ImGuiAl::Log::getInstance().Error(error.c_str());
            break;
        }
        case GL_DEBUG_SEVERITY_LOW:
            ImGuiAl::Log::getInstance().Debug(error.c_str());
            break;
        default:
            //ImGuiAl::Log::getInstance().Debug(error.c_str());
            break;

    }
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	game.getEventManager()->keyCallback(game, key, scancode, action, mods);
}

static void mousePosCallback(GLFWwindow* window, double x, double y)
{
	game.getEventManager()->mousePosCallback(game, x, y);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	game.getEventManager()->mouseButtonCallback(game, button, action, mods);
}

/* --------------------------------------------- */
// Main
/* --------------------------------------------- */

int main(int argc, char** argv)
{
    /* --------------------------------------------- */
    // Load settings.ini
    /* --------------------------------------------- */

    INIReader reader("assets/settings/settings.ini");

    // load values from ini file
    // first param: section [window], second param: property name, third param: default value
    int width = reader.GetInteger("window", "width", 800);
    int height = reader.GetInteger("window", "height", 800);
    float fov = reader.GetReal("camera", "fov",60);
    float nearParam = reader.GetReal("camera", "near", 0.1);
    float farParam = reader.GetReal("camera", "far", 100);
    std::string window_title = reader.Get("window", "title", "ECG");

    /* --------------------------------------------- */
    // Initialize Window and OpenGL context with GLFW
    /* --------------------------------------------- */

    if (glfwInit() != GLFW_TRUE)
    {
        std::cerr<<"GLFW Initialization failed"<<std::endl;
        return 1;
    }

    //set window hints

    //set version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //disable fixed function pipeline
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //prevent window from being resized
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#ifndef NDEBUG
    //activate debug context
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    //create window
    GLFWwindow* window = glfwCreateWindow(width, height, window_title.c_str(), nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        std::cerr<<"GLFW Window Initialization failed"<<std::endl;
        return 2;
    }

    //make context active
    glfwMakeContextCurrent(window);

    //load openGL functions
    int gladInitRes = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    if (!gladInitRes) {
        std::cerr << "Unable to initialize glad" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 3;
    }

    /* --------------------------------------------- */
    // Initialize Game
    /* --------------------------------------------- */
    game.init(window);

    /* --------------------------------------------- */
    // Register callback functions (input, opengl errors, ...)
    /* --------------------------------------------- */
#ifndef NDEBUG
    //register debug callback function
	glDebugMessageCallback(debugCallback, &game);

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

    //register key callback
    glfwSetKeyCallback(window, keyCallback);

    //setup mouse callbacks
    glfwSetCursorPosCallback(window, mousePosCallback);

    //glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window,mouseButtonCallback);

    //setup joystick callbacks
    //TODO

    //TODO do we need that?
    //activate depth testing
    //glEnable(GL_DEPTH_TEST);

    //enable back-face culling
    //glEnable(GL_CULL_FACE);

    /* --------------------------------------------- */
    // Update/Render Loop (using fixed time step), used https://medium.com/@tglaiel/how-to-make-your-game-run-at-60fps-24c61210fe75 for some design decisions
    /* --------------------------------------------- */
    //render loop
    bool anotherUpdate = true;
    double lastTime = glfwGetTime();
    double currentTime;
    double accumulator = FIXED_DELTA;
    double delta;

    while (!glfwWindowShouldClose(window) && anotherUpdate)
    {
		//calculate delta time
        currentTime = glfwGetTime();
        delta = currentTime-lastTime;
        lastTime = currentTime;
        
        //increase accumulator
        accumulator +=delta;
        //cap accumulator to a maximum to prevent "spiral of doom"
        if(accumulator>UPDATE_TIME_ACCUM_MAX)
            accumulator = UPDATE_TIME_ACCUM_MAX;

        //do update if necessary
        while(accumulator>=FIXED_DELTA)
        {
            //poll input
            glfwPollEvents();

            anotherUpdate = game.update();

            accumulator-=FIXED_DELTA;
        }

        game.render();
    }

    /* --------------------------------------------- */
    // Cleanup
    /* --------------------------------------------- */

    //cleanup game
    game.cleanup();

    //destroy window and context
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}



