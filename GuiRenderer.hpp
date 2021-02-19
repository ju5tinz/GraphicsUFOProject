//  adapted from youtube tutorial
//  https://www.youtube.com/watch?v=vOmJ1lyiJ4A&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=25&t=0s
//
#ifndef GuiRenderer_hpp
#define GuiRenderer_hpp

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

#include "GuiTexture.hpp"
#include "GuiShader.hpp"

class GuiRenderer {
 
private:
    GLuint vao, vbo;
    GuiShader * shader;
    
public:
    GuiRenderer();
    ~GuiRenderer();
    void render(std::vector<GuiTexture *> guis);
    glm::mat4 createTransformationMatrix(glm::vec2 position, glm::vec2 scale);

};

#endif /* GuiRenderer_hpp */
