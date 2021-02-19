#include "Window.h"

int Window::width;
int Window::height;
const char* Window::windowTitle = "FLIGHT";

//Terrain
Terrain* Window::terrain;
GLuint Window::terrainProgram;

//Skybox
Skybox * Window::skybox;
std::vector<unsigned int> Window::cubemaps;
int Window::currentCubemapsIndex;
unsigned int Window::cubemapTexture;
GLuint Window::skyboxProgram;
GLuint Window::SB_projectionLoc;
GLuint Window::SB_viewLoc;
GLuint Window::SB_skyboxLoc;
GLuint Window::SB_planeLoc;

// View & Projection matrix.
glm::mat4 Window::view;
glm::mat4 Window::projection;
glm::mat4 Window::initProjection;

// Camera
Camera Window::camera(glm::vec3(2500, 300, 3000), glm::vec3(0.0f, 1.0f, 0.0f), 0.8f, -18.0f);
float Window::currX;
float Window::currY;
float Window::lastX;
float Window::lastY;
bool Window::moveCam = false;
float Window::lastFrame = 0.f;
float Window::deltaTime = 0.f;
double Window::fovy = glm::radians(60.0);

// Light
glm::vec3 Window::lightColor = glm::vec3(1,1,1);
glm::vec3 Window::lightPosition = glm::vec3(500,500,500);

// Water
WaterFrameBuffers * Window::fbos;
std::vector<WaterTile *> Window::waters;
WaterRenderer * Window::waterRenderer;
WaterShader * Window::waterShader;

// GUI
std::vector<GuiTexture *> Window::guis;
GuiTexture * Window::refraction;
GuiTexture * Window::reflection;
GuiRenderer * Window::guiRenderer;
bool Window::showWaterGUI = false;

// particle
CubeEmitter* Window::emitter;
ParticleEffect * Window::particleEffect;
bool Window::showParticles = false;

// ufo
Flyer* Window::ufo;
GLuint Window::ufoProgram;
Target* Window::target;


bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    skyboxProgram = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
    terrainProgram = LoadShaders("shaders/terrain.vert", "shaders/terrain.frag");
	ufoProgram = LoadShaders("shaders/ufo.vert", "shaders/ufo.frag");

    // Check the shader program.
    if (!skyboxProgram){
        std::cerr << "Failed to initialize skybox shader program" << std::endl;
        return false;
    }
    if (!terrainProgram){
        std::cerr << "Failed to initialize terrain shader program" << std::endl;
        return false;
    }

    
    //SKYBOX SHADER
    SB_projectionLoc = glGetUniformLocation(skyboxProgram, "projection");
    SB_viewLoc = glGetUniformLocation(skyboxProgram, "view");
    SB_skyboxLoc = glGetUniformLocation(skyboxProgram, "skybox");
    SB_planeLoc = glGetUniformLocation(skyboxProgram, "plane");
    
    view = camera.GetViewMatrix();

    lastFrame = glfwGetTime();

	srand(time(NULL));
    
    return true;
}

bool Window::initializeObjects()
{
    //skybox
    skybox = new Skybox();
    
    //skyboxes from https://opengameart.org/content/mayhems-skyboxes?page=1
    //list of skyboxes images
    std::vector<std::vector<std::string>> faces = {
        {
        "mayhem/afterrain_ft.jpg",
        "mayhem/afterrain_bk.jpg",
        "mayhem/afterrain_up.jpg",
        "mayhem/afterrain_dn.jpg",
        "mayhem/afterrain_rt.jpg",
        "mayhem/afterrain_lf.jpg"
        },
        
        {
        "mayhem/h2s_ft.jpg",
        "mayhem/h2s_bk.jpg",
        "mayhem/h2s_up.jpg",
        "mayhem/h2s_dn.jpg",
        "mayhem/h2s_rt.jpg",
        "mayhem/h2s_lf.jpg"
        },
        
        {
        "mayhem/nightball_ft.tga",
        "mayhem/nightball_bk.tga",
        "mayhem/nightball_up.tga",
        "mayhem/nightball_dn.tga",
        "mayhem/nightball_rt.tga",
        "mayhem/nightball_lf.tga"
        },
        
        {
        "mayhem/thefog_ft.tga",
        "mayhem/thefog_bk.tga",
        "mayhem/thefog_up.tga",
        "mayhem/thefog_dn.tga",
        "mayhem/thefog_rt.tga",
        "mayhem/thefog_lf.tga"
        },
        {
        "mayhem/prehistoric_ft.jpg",
        "mayhem/prehistoric_bk.jpg",
        "mayhem/prehistoric_up.jpg",
        "mayhem/prehistoric_dn.jpg",
        "mayhem/prehistoric_rt.jpg",
        "mayhem/prehistoric_lf.jpg"
        }
    };
    //load all cubeMaps
    for(int i=0; i<faces.size(); i++){
        unsigned int temp = loadCubemap(faces[i]);
        cubemaps.push_back(temp);
    }
    currentCubemapsIndex = 1;
    cubemapTexture = cubemaps[currentCubemapsIndex];
    
    //frame buffer object
    fbos = new WaterFrameBuffers(width, height);
    
    //water tile
    waterShader = new WaterShader();
    waterRenderer = new WaterRenderer(waterShader, projection, fbos);
    waters.push_back(new WaterTile(2/2, 2/2, 0.03/2));
    
    //gui display
    guiRenderer = new GuiRenderer();
    refraction = new GuiTexture(fbos->getRefractionTexture(), glm::vec2(2.5,2.5), glm::vec2(0.25,0.25));
    reflection = new GuiTexture(fbos->getReflectionTexture(), glm::vec2(-2.5,2.5), glm::vec2(0.25,0.25));
    
    guis.push_back(refraction);
    guis.push_back(reflection);
    
    // Terrain
    terrain = new Terrain(1000, terrainProgram);
    terrain->setCamera(&camera);
    terrain->generateHeights();
    terrain->genTriangles();
    terrain->init();
    

    //particles
    emitter = new CubeEmitter();
    particleEffect = new ParticleEffect(60000, emitter, &camera, projection);

	//ufo
	glm::mat4 model = glm::translate(glm::vec3(10000.0f, 700.0f, 10000.0f));
	model = glm::scale(glm::vec3(0.3f, 0.3f, 0.3f)) * model;
	ufo = new Flyer("ufo.obj", ufoProgram, model);
	ufo->x_loc = 10000 * 0.3;
	ufo->y_loc = 10000 * 0.3;

	float x_rand_loc = rand() % 10 + 25.0;
	float y_rand_loc = rand() % 10 + 25.0;
	glm::mat4 sphereModel = glm::translate(glm::vec3(x_rand_loc, 2.1f, y_rand_loc));
	sphereModel = glm::scale(glm::vec3(100.0f, 100.0f, 100.0f)) * sphereModel;
	target = new Target("sphere.obj", ufoProgram, sphereModel);
	target->x_loc = x_rand_loc * 100;
	target->y_loc = y_rand_loc * 100;
    
    return true;
}

void Window::cleanUp()
{
    // Deallcoate the objects.
    delete skybox;
    delete fbos;
    delete waterShader;
    delete waterRenderer;
    for(WaterTile * w : waters)
        delete w;
    delete guiRenderer;
    delete reflection;
    delete refraction;
    delete terrain;
    delete emitter;
    delete particleEffect;
    // Delete the shader program.
    glDeleteProgram(skyboxProgram);
    glDeleteProgram(terrainProgram);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    Window::width = width;
    Window::height = height;
    
    fbos->updateWindowHeight(width, height);
    
    // Set the viewport size.
    glViewport(0, 0, width, height);

    // Set the projection matrix.
    Window::projection = glm::perspective(fovy, double(width) / (double)height, 0.1, 10000.0);
    Window::initProjection = Window::projection;
}

void Window::idleCallback()
{
    // update time
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    // update particles
    particleEffect->Update(deltaTime);

	if (target->found == true) {
		float x_rand_loc = rand() % 10 + 25.0;
		float y_rand_loc = rand() % 10 + 25.0;
		glm::mat4 translate = glm::translate(glm::vec3(x_rand_loc, 2.1f, y_rand_loc));
		target->update(translate);
		target->x_loc = x_rand_loc * 100;
		target->y_loc = y_rand_loc * 100;
		target->found = false;
	}
}

void Window::displayCallback(GLFWwindow* window)
{
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Enable clip distance
    glEnable(GL_CLIP_DISTANCE0);

	waterShader->loadProjectionMatrix(projection);

    //Render reflection texture
    fbos->bindReflectionFrameBuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float distance = 2 * (camera.Position.y - waters[0]->getHeight());
    camera.Position.y -= distance;
    camera.invertPitch();
    renderScene(                 glm::vec4(0, 1, 0, -(waters[0]->getHeight())));
    renderSkybox(cubemapTexture, glm::vec4(0, 1, 0, -(waters[0]->getHeight())));
    if(showParticles){
        particleEffect->Render();
    }
    camera.Position.y += distance;
    camera.invertPitch();

    //Render refraction texture
    fbos->bindRefractionFrameBuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderScene(                 glm::vec4(0, -1, 0 , waters[0]->getHeight()));
    renderSkybox(cubemapTexture, glm::vec4(0, -1, 0 , waters[0]->getHeight()));
    if(showParticles){
        particleEffect->Render();
    }
    
    fbos->unbindCurrentFrameBuffer();
    glDisable(GL_CLIP_DISTANCE0);

    //Render Scene to screen
    renderScene(glm::vec4(0, 1, 0, -(waters[0]->getHeight())));
    waterRenderer->render(waters, &camera, lightColor, lightPosition);

    //Render skybox
    renderSkybox(cubemapTexture, glm::vec4(0, 1, 0, -250));

    //Render particles
    if(showParticles){
        particleEffect->Render();
    }

    //Render water refract/reflect debug gui
    if(showWaterGUI){
        glClear(GL_DEPTH_BUFFER_BIT);
        guiRenderer->render(guis);
    }
    
	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::renderScene(glm::vec4 plane){
    view = camera.GetViewMatrix();
    glUseProgram(terrainProgram);
	glUniformMatrix4fv(glGetUniformLocation(terrainProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(terrainProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glm::mat4 terrainModel = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 100.0f));
	terrainModel = glm::translate(terrainModel, glm::vec3(0, 0, 0));
	glUniformMatrix4fv(glGetUniformLocation(terrainProgram, "model"), 1, GL_FALSE, glm::value_ptr(terrainModel));
	glUniform3fv(glGetUniformLocation(terrainProgram, "lightPos"), 1, glm::value_ptr(lightPosition));
	glUniform3fv(glGetUniformLocation(terrainProgram, "view"), 1, glm::value_ptr(camera.Position));
	glUniform4fv(glGetUniformLocation(terrainProgram, "plane"), 1, glm::value_ptr(plane));

    // Render the object.
    terrain->draw();

	glUseProgram(ufoProgram);
	//render ufo
	glUniformMatrix4fv(glGetUniformLocation(ufoProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(ufoProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniform3fv(glGetUniformLocation(ufoProgram, "lightPos"), 1, glm::value_ptr(lightPosition));
	glUniform3fv(glGetUniformLocation(ufoProgram, "viewPos"), 1, glm::value_ptr(camera.Position));
	glUniform3fv(glGetUniformLocation(ufoProgram, "lightColor"), 1, glm::value_ptr(lightColor));
	glUniform3fv(glGetUniformLocation(ufoProgram, "objectColor"), 1, glm::value_ptr(glm::vec3(0.3, 0.5, 0.6)));
	glUniform3fv(glGetUniformLocation(ufoProgram, "stength"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	ufo->draw();


	target->draw();
}

// draw skybox
void Window::renderSkybox(unsigned int cubemapTexture, glm::vec4 plane)
{	
    // draw skybox as last
    view = camera.GetViewMatrix();
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    glUseProgram(skyboxProgram);
    
    glm::mat4 n_view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
    glUniformMatrix4fv(SB_viewLoc, 1, GL_FALSE, glm::value_ptr(n_view));
    
    glUniformMatrix4fv(SB_projectionLoc, 1, GL_FALSE, glm::value_ptr(initProjection));
    glUniform1f(SB_skyboxLoc, 0.f);
    glUniform4fv(SB_planeLoc, 1, glm::value_ptr(plane));
    
    skybox->draw(cubemapTexture);
    
    // set depth function & shader back to default
    glDepthFunc(GL_LESS);
}


void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    // Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key){
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
			case GLFW_KEY_W:
				ufo->update(glm::translate(glm::vec3(1000.0f * deltaTime, 0, 0)));
				ufo->x_loc += (1000.0f * deltaTime);
				break;
			case GLFW_KEY_A:
				ufo->update(glm::translate(glm::vec3(0, 0, -1000.0f * deltaTime)));
				ufo->y_loc -= (1000.0f * deltaTime);
				break;
			case GLFW_KEY_S:
				ufo->update(glm::translate(glm::vec3(-1000.0f * deltaTime, 0, 0)));
				ufo->x_loc -= (1000.0f * deltaTime);
				break;
			case GLFW_KEY_D:
				ufo->update(glm::translate(glm::vec3(0, 0, 1000.0f * deltaTime)));
				ufo->y_loc += (1000.0f * deltaTime);
				break;
			/*
            case GLFW_KEY_UP:
                //move camera up (+Z)
                camera.ProcessKeyboard(FORWARD, deltaTime);
                view = camera.GetViewMatrix();
                break;
            case GLFW_KEY_DOWN:
                //move camera down (-Z)
                camera.ProcessKeyboard(BACKWARD, deltaTime);
                view = camera.GetViewMatrix();
                break;
            case GLFW_KEY_RIGHT:
               //move camera right (+x)
               camera.ProcessKeyboard(RIGHT, deltaTime);
               view = camera.GetViewMatrix();
               break;
            case GLFW_KEY_LEFT:
               //move camera left (-x)
               camera.ProcessKeyboard(LEFT, deltaTime);
               view = camera.GetViewMatrix();
               break;
			*/
            case GLFW_KEY_1:
                showWaterGUI = !showWaterGUI;
                break;
            case GLFW_KEY_2:
                currentCubemapsIndex = (++currentCubemapsIndex) % cubemaps.size();
                cubemapTexture = cubemaps[currentCubemapsIndex];
                break;
            case GLFW_KEY_3:
                showParticles = !showParticles;
            break;
            default:
                break;
        }
        view = camera.GetViewMatrix();
		//std::cout << "ufo: " << ufo->x_loc << " " << ufo->y_loc << std::endl;
		//std::cout << "target: " << target->x_loc << " " << target->y_loc << std::endl;
		if (glm::distance(glm::vec2(ufo->x_loc, ufo->y_loc), glm::vec2(target->x_loc, target->y_loc)) < 100) {
			target->found = true;
		}
    }
}

void Window::cursorCallback(GLFWwindow* window, double xpos, double ypos) {
	currX = xpos;
	currY = ypos;

	if (moveCam) {
		float xoffset = lastX - currX;
		float yoffset = currY - lastY;

		lastX = currX;
		lastY = currY;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	//std::cout << "Front: " << camera.Yaw << " " << camera.Pitch << std::endl;
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == 1 && button == 0) {
		lastX = currX;
		lastY = currY;
		moveCam = true;
	}

	if (action == 0 && button == 0) {
		moveCam = false;
	}
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	/*
    if(yoffset < 0){
        camera.ProcessKeyboard(FORWARD, deltaTime);
        view = camera.GetViewMatrix();
    } else {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        view = camera.GetViewMatrix();
    }
	*/

	/*
	double fovDeg = glm::degrees(fovy);
	if (fovDeg >= 0.5f && fovDeg <= 90.0f)
		fovDeg -= (yoffset * 3);
	if (fovDeg <= 0.5f)
		fovDeg = 0.5f;
	if (fovDeg >= 90.0f)
		fovDeg = 90.0f;

	fovy = glm::radians(fovDeg);

	Window::projection = glm::perspective((double)fovy,
		double(width) / (double)height, 0.1, 10000.0);
	*/
}

unsigned int Window::loadCubemap(std::vector<std::string> faces){
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
