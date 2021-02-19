//
//  CubeEmitter.hpp
//  CSE167FinalProject
//
//  Created by Connie Luong on 12/10/19.
//  Copyright © 2019 Connie Luong. All rights reserved.
//

#ifndef CubeEmitter_hpp
#define CubeEmitter_hpp

#pragma once
#include "ParticleSystemPCH.h"
#include "Random.h"
#include "ParticleEmitter.h"

class CubeEmitter : public ParticleEmitter{
public:
    CubeEmitter();

    virtual void EmitParticle( Particle& particle );

    float MinWidth;
    float MaxWidth;

    float MinHeight;
    float MaxHeight;

    float MinDepth;
    float MaxDepth;

    float MinSpeed;
    float MaxSpeed;

    float MinLifetime;
    float MaxLifetime;

    glm::vec3   Origin;

};

#endif /* CubeEmitter_hpp */
