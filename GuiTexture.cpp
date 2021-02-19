//
//  GuiTexture.cpp
//  CSE167FinalProject
//
//  adapted from youtube tutorial
//  https://www.youtube.com/watch?v=vOmJ1lyiJ4A&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=25&t=0s
//

#include "GuiTexture.hpp"

GuiTexture::GuiTexture(GLuint texture, glm::vec2 position, glm::vec2 scale){
    this->texture = texture;
    this->position = position;
    this->scale = scale;
}

GLuint GuiTexture::getTexture(){
    return texture;
}
glm::vec2 GuiTexture::getPosition(){
    return position;
}
glm::vec2 GuiTexture::getScale(){
    return scale;
}
