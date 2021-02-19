#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <stdlib.h>
#include <time.h>

#include "Skybox.hpp"
#include "shader.h"
#include "Camera.h"
#include "Terrain.h"
#include "Flyer.h"
#include "Target.h"

#include "WaterTile.hpp"
#include "WaterRenderer.hpp"
#include "WaterShader.hpp"

#include "WaterFrameBuffers.hpp"

#include "GuiTexture.hpp"
#include "GuiRenderer.hpp"

#include "ParticleEffect.hpp"
#include "CubeEmitter.hpp"

#include "stb_image.h"


class Window
{
private:
    //extracted the render scene stuff from displaycallback
    static void renderScene(glm::vec4 plane);
    static void renderSkybox(unsigned int cubemapTexture, glm::vec4 plane);

public:
    static int width;
    static int height;
    static const char* windowTitle;

    // particle
    static CubeEmitter* emitter;
    static ParticleEffect * particleEffect;
    static bool showParticles;

    // skybox
    static Skybox* skybox;
    static std::vector<unsigned int> cubemaps;
    static int currentCubemapsIndex;
    static unsigned int cubemapTexture;
    
    // frame buffer
    static WaterFrameBuffers * fbos;

    // Water
    static std::vector<WaterTile *> waters;
    static WaterRenderer * waterRenderer;
    static WaterShader * waterShader;

    // gui
    static std::vector<GuiTexture *> guis;
    static GuiTexture * refraction;
    static GuiTexture * reflection;
    static GuiRenderer * guiRenderer;
    static bool showWaterGUI;
    static Terrain* terrain;

    // Camera stuff
    static glm::mat4 initProjection;
    static glm::mat4 projection;
    static glm::mat4 view;
    static Camera camera;
	static double fovy;
    static float currX, currY, lastX, lastY;
    static bool moveCam;
    static float lastFrame;
    static float deltaTime;

    //light
    static glm::vec3 lightColor;
    static glm::vec3 lightPosition;

    // shader programs
    static GLuint skyboxProgram, terrainProgram, ufoProgram;

    // skybox shader uniforms
    static GLuint SB_projectionLoc, SB_viewLoc, SB_skyboxLoc, SB_planeLoc;

	// ufo
	static Flyer* ufo;

	static Target* target;


    static bool initializeProgram();
    static bool initializeObjects();
    static void cleanUp();
    static GLFWwindow* createWindow(int width, int height);
    static void resizeCallback(GLFWwindow* window, int width, int height);
    static void idleCallback();
    static void displayCallback(GLFWwindow* window);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    static unsigned int loadCubemap(std::vector<std::string> faces);

};

#endif
