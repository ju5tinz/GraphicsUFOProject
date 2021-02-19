#ifndef Skybox_hpp
#define Skybox_hpp

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

#include <string>
#include <vector>
#include <iostream>



class Skybox {

private:
    GLuint skyboxVAO, skyboxVBO;
    
public:
    Skybox();
    ~Skybox();
    
    void draw(unsigned int cubemapTexture);
    
};


#endif /* Skybox_hpp */
