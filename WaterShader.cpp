//
//  WaterShader.cpp
//  CSE167FinalProject
//
//  adapted from youtube tutorial by ThinMatrix
//  https://www.youtube.com/watch?v=HusvGeEDU_U&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh
//

#include "WaterShader.hpp"

WaterShader::WaterShader(){
    VERTEX_FILE = "shaders/water.vert";
    FRAGMENT_FILE = "shaders/water.frag";
    programID = LoadShaders(VERTEX_FILE, FRAGMENT_FILE);
    bindAttributes();
    getAllUniformLocations();
}

WaterShader::~WaterShader(){
    glDeleteProgram(programID);
}

void WaterShader::bindAttributes(){
    glBindAttribLocation(programID, 0, "position");
}

void WaterShader::getAllUniformLocations(){
    location_projectionMatrix = glGetUniformLocation(programID, "projectionMatrix");
    location_viewMatrix = glGetUniformLocation(programID, "viewMatrix");
    location_modelMatrix = glGetUniformLocation(programID, "modelMatrix");
    location_reflectionTexture = glGetUniformLocation(programID, "reflectionTexture");
    location_refractionTexture = glGetUniformLocation(programID, "refractionTexture");
    location_dudvMap = glGetUniformLocation(programID, "dudvMap");
    location_moveFactor = glGetUniformLocation(programID, "moveFactor");
    location_cameraPosition = glGetUniformLocation(programID, "cameraPosition");
    location_normalMap = glGetUniformLocation(programID, "normalMap");
    location_lightColor = glGetUniformLocation(programID, "lightColor");
    location_lightPosition = glGetUniformLocation(programID, "lightPosition");
    location_depthMap = glGetUniformLocation(programID, "depthMap");
}

void WaterShader::connectTextureUnits(){
    glUniform1i(location_reflectionTexture, 0);
    glUniform1i(location_refractionTexture, 1);
    glUniform1i(location_dudvMap, 2);
    glUniform1i(location_normalMap, 3);
    glUniform1i(location_depthMap, 4);
}

void WaterShader::loadProjectionMatrix(glm::mat4 projection){
    glUniformMatrix4fv(location_projectionMatrix, 1, GL_FALSE, glm::value_ptr(projection));
}

void WaterShader::loadMoveFactor(float factor){
    glUniform1f(location_moveFactor, factor);
}

void WaterShader::loadLight(glm::vec3 lightColor, glm::vec3 lightPosition){
    glUniform3fv(location_lightColor, 1, glm::value_ptr(lightColor));
    glUniform3fv(location_lightPosition, 1, glm::value_ptr(lightPosition));

}

void WaterShader::loadViewMatrix(Camera * camera){
    glm::mat4 view = camera->GetViewMatrix();
    glUniformMatrix4fv(location_viewMatrix, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3fv(location_cameraPosition, 1, glm::value_ptr(camera->Position));
}

void WaterShader::loadModelMatrix(glm::mat4 modelMatrix){
    glUniformMatrix4fv(location_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void WaterShader::start(){
    glUseProgram(programID);
}

void WaterShader::stop(){
    glUseProgram(0);
}
