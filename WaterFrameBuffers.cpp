//
//  WaterFrameBuffers.cpp
//  CSE167FinalProject
//
//  adapted from youtube tutorial by ThinMatrix
//  https://www.youtube.com/watch?v=HusvGeEDU_U&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh
//

#include "WaterFrameBuffers.hpp"
using namespace std;

int WaterFrameBuffers::REFLECTION_WIDTH = 320;
int WaterFrameBuffers::REFLECTION_HEIGHT = 180;
int WaterFrameBuffers::REFRACTION_WIDTH = 1280;
int WaterFrameBuffers::REFRACTION_HEIGHT = 720;

int WaterFrameBuffers::windowWidth;
int WaterFrameBuffers::windowHeight;

GLuint WaterFrameBuffers::reflectionFrameBuffer;
GLuint WaterFrameBuffers::reflectionTexture;
GLuint WaterFrameBuffers::reflectionDepthBuffer;

GLuint WaterFrameBuffers::refractionFrameBuffer;
GLuint WaterFrameBuffers::refractionTexture;
GLuint WaterFrameBuffers::refractionDepthTexture;


//initialize reflection & refraction FBOs
WaterFrameBuffers::WaterFrameBuffers(int windowWidth, int windowHeight) {
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    
    initializeReflectionFrameBuffer();
    initializeRefractionFrameBuffer();
}


//initialize FBO (fb, texture, and depth texture)
void WaterFrameBuffers::initializeReflectionFrameBuffer(){
    //frame buffer
    reflectionFrameBuffer = createFrameBuffer();
    //create reflection texture - color attachment texture
    reflectionTexture = createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
    //create reflection depth buffer - renderbuffer object for depth and stencil attachment
    reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
    
    unbindCurrentFrameBuffer();
}
void WaterFrameBuffers::initializeRefractionFrameBuffer(){
    //frame buffer
    refractionFrameBuffer = createFrameBuffer();
    //create refraction texture - color attachment texture
    refractionTexture = createTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
    //create refraction depth texture
    refractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
    
    unbindCurrentFrameBuffer();
}
GLuint WaterFrameBuffers::createFrameBuffer(){
    GLuint frameBuffer;
    //generate name for frame buffer
    glGenFramebuffers(1, &frameBuffer);
    //create the frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    //indicate that we'll always render to color attachment 0
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    return frameBuffer;
}
//the color buffer texture attachment
GLuint WaterFrameBuffers::createTextureAttachment(int width, int height){
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    return texture;
}
//the depth buffer texture attachment
GLuint WaterFrameBuffers::createDepthTextureAttachment(int width, int height){
    GLuint depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
    return depthTexture;
}
//non texture attachment --> render buffer (stores depth information)
GLuint WaterFrameBuffers::createDepthBufferAttachment(int width, int height){
    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    return depthBuffer;
}




//bind FBO - call before rendering to this FBO
void WaterFrameBuffers::bindReflectionFrameBuffer(){
    bindFrameBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
    
}
void WaterFrameBuffers::bindRefractionFrameBuffer(){
    bindFrameBuffer(refractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}
void WaterFrameBuffers::bindFrameBuffer(GLuint frameBuffer, int width, int height){
    glBindTexture(GL_TEXTURE_2D, 0);//To make sure the texture isn't bound
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, width, height);
}
//call to switch to default frame buffer
void WaterFrameBuffers::unbindCurrentFrameBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowWidth, windowHeight);
}

void WaterFrameBuffers::updateWindowHeight(int w, int h){
    windowWidth = w;
    windowHeight = h;
}



//GETTERS
GLuint WaterFrameBuffers::getReflectionTexture(){
    return reflectionTexture;
}
GLuint WaterFrameBuffers::getRefractionTexture(){
    return refractionTexture;
}
GLuint WaterFrameBuffers::getRefractionDepthTexture(){
    return refractionDepthTexture;
}

//clean up
WaterFrameBuffers::~WaterFrameBuffers(){
    glDeleteFramebuffers(1, &reflectionFrameBuffer);
    glDeleteTextures(1, &reflectionTexture);
    glDeleteRenderbuffers(1, &reflectionDepthBuffer);
    
    glDeleteFramebuffers(1, &refractionFrameBuffer);
    glDeleteTextures(1, &refractionTexture);
    glDeleteRenderbuffers(1, &refractionDepthTexture);
}
