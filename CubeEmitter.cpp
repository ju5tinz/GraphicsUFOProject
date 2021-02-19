#include "CubeEmitter.hpp"
#include <iostream>
using namespace std;

CubeEmitter::CubeEmitter()
: MinWidth(-10000)
, MaxWidth(10000)
, MinHeight(100)
, MaxHeight(1000)
, MinDepth(-1000)
, MaxDepth(1000)
, MinSpeed(15)
, MaxSpeed(30)
, MinLifetime(10)
, MaxLifetime(20)
, Origin(0)
{}

void CubeEmitter::EmitParticle( Particle& particle )
{
    // generate random position
    float X = RandRange( MinWidth, MaxWidth );
    float Y = RandRange( MinHeight, MaxHeight );
    float Z = RandRange( MinDepth, MaxDepth );
    glm::vec3 vector(X, Y, Z);
    
    // generate random lifetime & speed
    float lifetime = RandRange( MinLifetime, MaxLifetime );
    float speed = RandRange( MinSpeed, MaxSpeed );

    // set particle
    particle.Position = vector + Origin;
//    particle.Velocity = glm::normalize(vector) * speed;
    particle.Velocity = glm::vec3(0,3,0) * speed;
    particle.fLifeTime = lifetime;
    particle.fAge = 0;
}
