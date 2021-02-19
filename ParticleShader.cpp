  
//
//  ParticleShader.cpp
//  CSE167FinalProject
//
//  adapted from youtube tutorial by ThinMatrix
//  https://www.youtube.com/watch?v=6PkjU9LaDTQ&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP&index=34
//

#include "ParticleShader.hpp"
using namespace std;

ParticleShader::ParticleShader(){
    programID = LoadShaders("shaders/particle.vert", "shaders/particle.frag");
    bindAttributes();
    getAllUniformLocations();
}
void ParticleShader::getAllUniformLocations(){
    location_modelViewMatrix = glGetUniformLocation(programID, "modelViewMatrix");
    location_projectionMatrix = glGetUniformLocation(programID, "projectionMatrix");

}
void ParticleShader::bindAttributes(){
    glBindAttribLocation(programID, 0, "position");
}
void ParticleShader::loadModelViewMatrix(glm::mat4 modelView){
    glUniformMatrix4fv(location_modelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelView));
}
void ParticleShader::loadProjectionMatrix(glm::mat4 projectionMatrix){
    glUniformMatrix4fv(location_projectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}
void ParticleShader::start(){
    glUseProgram(programID);
}

void ParticleShader::stop(){
    glUseProgram(0);
}
