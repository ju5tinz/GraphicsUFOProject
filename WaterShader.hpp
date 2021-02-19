//
//  WaterShader.hpp
//  CSE167FinalProject
//
//  adapted from youtube tutorial by ThinMatrix
//  https://www.youtube.com/watch?v=HusvGeEDU_U&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh
//

#ifndef WaterShader_hpp
#define WaterShader_hpp

//opengl
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "shader.h"
#include "Camera.h"

class WaterShader {
private:
    const char * VERTEX_FILE;
    const char * FRAGMENT_FILE;
    GLuint programID, location_projectionMatrix, location_viewMatrix, location_modelMatrix;
    GLuint location_reflectionTexture, location_refractionTexture;
    GLuint location_dudvMap, location_moveFactor, location_cameraPosition, location_normalMap;
    GLuint location_lightColor, location_lightPosition;
    GLuint location_depthMap;
    
public:
    WaterShader();
    ~WaterShader();
    void bindAttributes();
    void getAllUniformLocations();
    void loadProjectionMatrix(glm::mat4 projection);
    void loadViewMatrix(Camera * camera);
    void loadModelMatrix(glm::mat4 modelMatrix);
    void loadMoveFactor(float factor);
    void loadLight(glm::vec3 lightColor, glm::vec3 lightPosition);
        
    void connectTextureUnits();
    
    void start();
    void stop();
};

#endif /* WaterShader_hpp */
