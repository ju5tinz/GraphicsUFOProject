//  adapted from youtube tutorial
//  https://www.youtube.com/watch?v=vOmJ1lyiJ4A&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=25&t=0s
//
#ifndef GuiShader_hpp
#define GuiShader_hpp

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

#include <vector>
#include <string>

#include "shader.h"

class GuiShader {
private:
    const char * VERTEX_FILE;
    const char * FRAGMENT_FILE;
    GLuint programID, location_transformationMatrix, location_guiTexuture;
    
public:
    GuiShader();
    ~GuiShader();
    void loadTransformation(glm::mat4 matrix);
    void getAllUniformLocations();
    void loadtexture();
    void bindAttributes();
    void start();
    void stop();
    
};

#endif /* GuiShader_hpp */
