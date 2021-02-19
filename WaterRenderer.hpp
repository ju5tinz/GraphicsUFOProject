//
//  WaterRenderer.hpp
//  CSE167FinalProject
//
//  adapted from youtube tutorial by ThinMatrix
//  https://www.youtube.com/watch?v=HusvGeEDU_U&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh
//

#ifndef WaterRenderer_hpp
#define WaterRenderer_hpp

//opengl
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

//libraries
#include <vector>
#include <string>
#include <cmath>

//files
#include "WaterTile.hpp"
#include "WaterShader.hpp"
#include "WaterFrameBuffers.hpp"
#include "Camera.h"

class WaterRenderer{
private:
    GLuint vao, vbo;
    WaterShader * shader;
    WaterFrameBuffers * fbos;
    float waveSpeed, moveFactor;
    
    GLuint dudvTexture, normalTexture;
    static float lastFrameTime;
  
public:
    WaterRenderer(WaterShader * shader, glm::mat4 projectionMatrix, WaterFrameBuffers * fbos);
    ~WaterRenderer();
    void render(std::vector<WaterTile *> water, Camera * camera, glm::vec3 lightColor, glm::vec3 lightPosition);
    glm::mat4 createTransformationMatrix(glm::vec3 translation, float scale);
    void prepareRender(Camera * camera, glm::vec3 lightColor, glm::vec3 lightPosition);
    void unbind();
    void setupVAO();
};

#endif /* WaterRenderer_hpp */
