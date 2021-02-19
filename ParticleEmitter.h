//
//  ParticleEmitter.h
//  CSE167FinalProject
//
//  Created by Connie Luong on 12/10/19.
//  Copyright © 2019 Connie Luong. All rights reserved.
//

#ifndef ParticleEmitter_h
#define ParticleEmitter_h

#pragma once

#include "Particle.h"

class ParticleEmitter
{
public:
    virtual ~ParticleEmitter() {}
    virtual void EmitParticle( Particle& particle ) = 0;

//    virtual void DebugRender() {}
};


#endif /* ParticleEmitter_h */
