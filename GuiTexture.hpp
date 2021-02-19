//
//  GuiTexture.hpp
//  CSE167FinalProject
//
//  adapted from youtube tutorial
//  https://www.youtube.com/watch?v=vOmJ1lyiJ4A&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=25&t=0s
//

#ifndef GuiTexture_hpp
#define GuiTexture_hpp

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

class GuiTexture{

private:
    GLuint texture;
    glm::vec2 position;
    glm::vec2 scale;
        
public:
    GuiTexture(GLuint texture, glm::vec2 position, glm::vec2 scale);
    GLuint getTexture();
    glm::vec2 getPosition();
    glm::vec2 getScale();
};

#endif /* GuiTexture_hpp */
