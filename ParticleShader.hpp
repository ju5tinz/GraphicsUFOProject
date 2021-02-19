//
//  ParticleShader.hpp
//  CSE167FinalProject
//
//  adapted from youtube tutorial by ThinMatrix
//  https://www.youtube.com/watch?v=6PkjU9LaDTQ&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=34
//

#ifndef ParticleShader_hpp
#define ParticleShader_hpp

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
//files
#include "shader.h"

class ParticleShader{
private:
    GLuint programID, location_modelViewMatrix, location_projectionMatrix;
    
public:
    ParticleShader();
    void getAllUniformLocations();
    void bindAttributes();
    void loadModelViewMatrix(glm::mat4 modelView);
    void loadProjectionMatrix(glm::mat4 projectionMatrix);
    void start();
    void stop();
};

#endif /* ParticleShader_hpp */
