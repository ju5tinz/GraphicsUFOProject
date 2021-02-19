//
//  WaterTile.hpp
//
//  adapted from youtube tutorial by ThinMatrix
//  https://www.youtube.com/watch?v=HusvGeEDU_U&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh
//
#ifndef WaterTile_hpp
#define WaterTile_hpp

//files
#include "shader.h"
#include "WaterFrameBuffers.hpp"

class WaterTile {
private:
    //center x, z, and height
    float height;
    float x,z;
    
    //fbo
    WaterFrameBuffers * fbos;
    
public:
    //constant
    static float TILE_SIZE;
    
    //constructor
    WaterTile(float centerX, float centerZ, float height);
    
    //getters & setters
    float getHeight();
    float getX();
    float getZ();
};

#endif /* WaterTile_hpp */
