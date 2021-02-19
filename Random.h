//
//  Random.h
//  CSE167FinalProject
//
//  Created by Connie Luong on 12/10/19.
//  Copyright © 2019 Connie Luong. All rights reserved.
//

#ifndef Random_h
#define Random_h

#pragma once

// Generate a random number between [0..1)
inline float Random()
{
    return rand() / (float)RAND_MAX;
}

inline float RandRange( float fMin, float fMax )
{
    if ( fMin > fMax ) std::swap( fMin, fMax );
    return ( Random() * ( fMax - fMin ) ) + fMin;
}

inline glm::vec3 RandUnitVec()
{
    float x = ( Random() * 2.0f ) - 1.0f;
    float y = ( Random() * 2.0f ) - 1.0f;
    float z = ( Random() * 2.0f ) - 1.0f;

    return glm::normalize( glm::vec3(x,y,z) );
}


#endif /* Random_h */
