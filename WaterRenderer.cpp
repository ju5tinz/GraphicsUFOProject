//
//  WaterRenderer.cpp
//  CSE167FinalProject
//
//  adapted from youtube tutorial by ThinMatrix
//  https://www.youtube.com/watch?v=HusvGeEDU_U&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh
//

#include "WaterRenderer.hpp"

#include "stb_image.h"

float WaterRenderer::lastFrameTime;

WaterRenderer::WaterRenderer(WaterShader * shader, glm::mat4 projectionMatrix, WaterFrameBuffers * fbos) : shader(shader), fbos(fbos)
{
    //dudv map
    waveSpeed = 0.03f;
    moveFactor = 0;
    lastFrameTime = (float) (glfwGetTime() * 1000);
    glGenTextures(1, &dudvTexture);
    glBindTexture(GL_TEXTURE_2D, dudvTexture);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("waterDUDV.png", &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
    } else{
        std::cout << "dudvMap texture failed to load at path: waterDUDV.png " << std::endl;
        stbi_image_free(data);
    }
    
    //normal map
    glGenTextures(1, &normalTexture);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    int width2, height2, nrChannels2;
    unsigned char *data2 = stbi_load("normalMap.png", &width2, &height2, &nrChannels2, 0);
    if(data2){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
    } else{
        std::cout << "normalMap texture failed to load at path: normalMap.png " << std::endl;
        stbi_image_free(data2);
    }
    
    
    shader->start();
    shader->connectTextureUnits();
    shader->loadProjectionMatrix(projectionMatrix);
    shader->stop();
    setupVAO();
}

WaterRenderer::~WaterRenderer(){
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void WaterRenderer::render(std::vector<WaterTile *> water, Camera * camera, glm::vec3 lightColor, glm::vec3 lightPosition){
    prepareRender(camera, lightColor, lightPosition);
    for (WaterTile * tile : water){
        glm::mat4 modelMatrix = createTransformationMatrix(glm::vec3(tile->getX(), tile->getHeight(), tile->getZ()), WaterTile::TILE_SIZE);
        shader->loadModelMatrix(modelMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    unbind();
}

glm::mat4 WaterRenderer::createTransformationMatrix(glm::vec3 translation, float scale){
    glm::mat4 matrix = glm::mat4(1);
	matrix = glm::translate(translation) * matrix;
	matrix = glm::scale(glm::vec3(scale)) * matrix;
    return matrix;
}

void WaterRenderer::prepareRender(Camera * camera, glm::vec3 lightColor, glm::vec3 lightPosition){
    shader->start();
    shader->loadViewMatrix(camera);
    
    //update time
    float currentFrameTime = (float) (glfwGetTime() * 1000);
    float delta = (currentFrameTime - lastFrameTime);
    lastFrameTime = currentFrameTime;
    //get frameTimeSeconds
    float frameTimeSeconds = delta/1000.f;
    
    //calculate and load moveFactor into water shader
    moveFactor = fmod( moveFactor + (waveSpeed * frameTimeSeconds) , 1) ;
    shader->loadMoveFactor(moveFactor);
    
    //load light info
    shader->loadLight(lightColor, lightPosition);
    
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    
    //fbos reflect and refract textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbos->getReflectionTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, fbos->getRefractionTexture());
    //dudv texture
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, dudvTexture);
    //normal map texture
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    //depthMap
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, fbos->getRefractionDepthTexture());
    
    //enable alpha blending for softer water edges
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WaterRenderer::unbind(){
    glDisable(GL_BLEND);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    shader->stop();
}

void WaterRenderer::setupVAO(){
    std::vector<float> vertices = {
        //triangle 1
        -1.f, -1.f,
        -1.f,  1.f,
         1.f, -1.f,
        //triangle 2
         1.f, -1.f,
        -1.f,  1.f,
         1.f,  1.f
    };

        
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Bind to the VAO.
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the vertices.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Unbind from the VBOs & VAO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
