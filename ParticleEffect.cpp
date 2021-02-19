#include "ParticleEffect.hpp"
#include "stb_image.h"

ParticleEffect::ParticleEffect( unsigned int numParticles, ParticleEmitter* PE, Camera* cam, glm::mat4 projectionMatrix)
: numParticles(numParticles), camera(cam), particleEmitter(PE), m_Force(glm::vec3(0, -9.81f, 0))
{
    // initializee particle buffer with 200 particle
    for(int i=0; i<numParticles; i++){
        particleBuffer.push_back(Particle());
    }
    
    std::cout << "particleBuffer size: " << particleBuffer.size() << std::endl;
    
    // make particles random
    EmitParticles();
    
    // load vertices information into the vao & vbo
    loadToVAO();
    
    // initialize shader & bind texture & load projection matrix
    shader = new ParticleShader();
    shader->start();
    shader->loadProjectionMatrix(projectionMatrix);
    shader->stop();
    
}

ParticleEffect::~ParticleEffect(){

}

// Emit 1 particle
void ParticleEffect::EmitParticle( Particle& particle ){
    particleEmitter->EmitParticle( particle );
}
// Emit all particles in particle Buffer
void ParticleEffect::EmitParticles(){
    for ( unsigned int i = 0; i < particleBuffer.size(); ++i ){
        EmitParticle(particleBuffer[i]);
    }
}

void ParticleEffect::Update(float fDeltaTime){
    for ( unsigned int i = 0; i < particleBuffer.size(); ++i ){
        //grab particle and calculate new age
        Particle& particle = particleBuffer[i];
        particle.fAge += fDeltaTime;
        
        //if particle is past lifeTime, re-emit particle
        if ( particle.fAge > particle.fLifeTime ){
            EmitParticle(particle);
        }
        
        //adjust particle's characteristic based on its life ratio
        float lifeRatio = particle.fAge / particle.fLifeTime;
//        particle.Velocity += ( m_Force * fDeltaTime );
        particle.Position.y -= ( particle.Velocity.y * fDeltaTime );
//        particle.fRotate = glm::lerp<float>( 0.0f, 720.0f, lifeRatio );
        particle.fSize = glm::lerp<float>( 3.0f, 0.5f, lifeRatio );
    }
}

void ParticleEffect::Render(){
    shader->start();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    for(Particle particle : particleBuffer){
        updateModelViewMatrix(particle.Position, particle.fRotate, particle.fSize, viewMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    glDisable(GL_BLEND);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    shader->stop();
}

void ParticleEffect::loadToVAO(){
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
    // Generate & bind VAO & VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // We will use the VBO to store the vertices.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Unbind from the VBOs & VAO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParticleEffect::updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix){
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(position) * modelMatrix;
    
    //want to make sure there is no rotation applied bc want particle to always face camera
    //want the upder 3x3 corner of the model matrix to be the transpose of that of the view matrix
    //so that when model*view the upper 3x3 corner is and identity matrix and therefore there will be no rotation
    modelMatrix[0][0] = viewMatrix[0][0];
    modelMatrix[0][1] = viewMatrix[1][0];
    modelMatrix[0][2] = viewMatrix[2][0];
    modelMatrix[1][0] = viewMatrix[0][1];
    modelMatrix[1][1] = viewMatrix[1][1];
    modelMatrix[1][2] = viewMatrix[2][1];
    modelMatrix[2][0] = viewMatrix[0][2];
    modelMatrix[2][1] = viewMatrix[1][2];
    modelMatrix[2][2] = viewMatrix[2][2];
    
    //apply rotation and scale
    modelMatrix = glm::rotate(glm::mat4(1), glm::radians(rotation), glm::vec3(0.f,0.f,1.f)) * modelMatrix;
    modelMatrix = glm::scale(glm::vec3(scale)) * modelMatrix;
    
    //load model-view matrix into shader
    glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
    shader->loadModelViewMatrix(modelViewMatrix);
}
