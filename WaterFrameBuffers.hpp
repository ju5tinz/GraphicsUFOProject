//
//  WaterFrameBuffers.hpp
//  CSE167FinalProject
//
//  adapted from youtube tutorial by ThinMatrix
//  https://www.youtube.com/watch?v=HusvGeEDU_U&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh
//

#ifndef WaterFrameBuffers_hpp
#define WaterFrameBuffers_hpp

//opengl
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <glm/glm.hpp>

class WaterFrameBuffers {

private:
    static int REFLECTION_HEIGHT;
    static int REFRACTION_HEIGHT;
    
    static int REFLECTION_WIDTH;
    static int REFRACTION_WIDTH;
    
    static int windowWidth;
    static int windowHeight;
    
    static GLuint reflectionFrameBuffer;
    static GLuint reflectionTexture;
    static GLuint reflectionDepthBuffer;
    
    static GLuint refractionFrameBuffer;
    static GLuint refractionTexture;
    static GLuint refractionDepthTexture;
    
    void initializeReflectionFrameBuffer();
    void initializeRefractionFrameBuffer();
    
    void bindFrameBuffer(GLuint frameBuffer, int width, int height);
    GLuint createFrameBuffer();
    GLuint createTextureAttachment(int width, int height);
    GLuint createDepthTextureAttachment(int width, int height);
    GLuint createDepthBufferAttachment(int width, int height);
    
public:
    WaterFrameBuffers(int windowWidth, int windowHeight);
    ~WaterFrameBuffers();
    void bindReflectionFrameBuffer();
    void bindRefractionFrameBuffer();
    void unbindCurrentFrameBuffer();
    void updateWindowHeight(int w, int h);
    GLuint getReflectionTexture();
    GLuint getRefractionTexture();
    GLuint getRefractionDepthTexture();
   
    
};

#endif /* WaterFrameBuffers_hpp */
