//  adapted from youtube tutorial
//  https://www.youtube.com/watch?v=vOmJ1lyiJ4A&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=25&t=0s
//
#include "GuiShader.hpp"


GuiShader::GuiShader(){
    VERTEX_FILE = "shaders/Gui.vert";
    FRAGMENT_FILE = "shaders/Gui.frag";
    programID = LoadShaders(VERTEX_FILE, FRAGMENT_FILE);
    bindAttributes();
    getAllUniformLocations();
}
void GuiShader::loadtexture(){
    glUniform1i(location_guiTexuture, 0);
}

void GuiShader::loadTransformation(glm::mat4 matrix){
    glUniformMatrix4fv(location_transformationMatrix, 1, GL_FALSE, glm::value_ptr(matrix));
}
void GuiShader::getAllUniformLocations(){
    location_transformationMatrix = glGetUniformLocation(programID, "transformationMatrix");
    location_guiTexuture =  glGetUniformLocation(programID, "guiTexture");
}
void GuiShader::bindAttributes(){
    glBindAttribLocation(programID, 0, "position");
}

GuiShader::~GuiShader(){
    glDeleteProgram(programID);
}

void GuiShader::start(){
    glUseProgram(programID);
}

void GuiShader::stop(){
    glUseProgram(0);
}
