//
//  WaterTile.cpp
//
//  adapted from youtube tutorial by ThinMatrix
//  https://www.youtube.com/watch?v=HusvGeEDU_U&list=PLRIWtICgwaX23jiqVByUs0bqhnalNTNZh
//
#include "WaterTile.hpp"
using namespace std;

float WaterTile::TILE_SIZE;

WaterTile::WaterTile(float centerX, float centerZ, float height) : x(centerX), z(centerZ), height(height)
{
    TILE_SIZE = 4000;
}

float WaterTile::getHeight(){
    return height;
}
float WaterTile::getX(){
    return x;
}
float WaterTile::getZ(){
    return z;
}
