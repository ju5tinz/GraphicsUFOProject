//
//  ParticleEffect.hpp
//  used to emit, update, and render particles
//  source: https://www.3dgep.com/simulating-particle-effects-using-opengl/#The_ParticleEffect_Class
//
#ifndef ParticleEffect_hpp
#define ParticleEffect_hpp
#pragma once

// import libraries
#include "ParticleSystemPCH.h"

// files
#include "Particle.h"
#include "ParticleEmitter.h"
#include "Camera.h"
#include "ParticleShader.hpp"

// helper classes
#include "Random.h"

class ParticleEffect{
private:
    ParticleShader* shader;
    Camera* camera;
    ParticleEmitter* particleEmitter;

    //vertex of Particle objects
    std::vector<Particle> particleBuffer;
    unsigned int numParticles;
    
    GLuint vao,vbo;

    // Apply this force to every particle in the effect
    // ==> based off of gravitation force of -9.8
    glm::vec3 m_Force;


    void EmitParticle( Particle& particle );

    void BuildVertexBuffer();
    
public:
    ParticleEffect( unsigned int numParticles = 0 , ParticleEmitter* PE = NULL, Camera* cam = NULL, glm::mat4 projectionMatrix = glm::mat4(1));
    virtual ~ParticleEffect();

    // Test method to randomize the particles in an interesting way
    void EmitParticles();

    virtual void Update( float fDeltaTime );
    virtual void Render();
    
    void loadToVAO();
    void updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix);

};

#endif /* ParticleEffect_hpp */
