//
//  Particle.h
//
//  Defines the particle object

#ifndef Particle_h
#define Particle_h

#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Particle
{
    Particle()
        : Position(0)
        , Velocity(0)
        , Color(0)
        , fRotate(0)
        , fAge(0)
        , fLifeTime(0)
    {}

    glm::vec3   Position; // Center point of particle
    glm::vec3   Velocity; // Current particle velocity
    glm::vec4   Color;    // Particle color
    float       fRotate;  // Rotate the particle the center
    float       fSize;    // Size of the particle
    float       fAge;
    float       fLifeTime;
};

typedef std::vector<Particle> ParticleBuffer;


#endif /* Particle_h */
