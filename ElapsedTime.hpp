//
//  ElapsedTime.h
// Helper class to count frame time
//

#ifndef ElapsedTime_h
#define ElapsedTime_h

#include "ParticleSystemPCH.h"


class ElapsedTime {
public:
    ElapsedTime( float maxTimeStep = 0.03333f );
    float GetElapsedTime() const;

private:
    float m_fMaxTimeStep;
    mutable float m_fPrevious;
};

ElapsedTime::ElapsedTime(float maxTimeStep) : m_fMaxTimeStep( maxTimeStep ), m_fPrevious ( std::clock() / (float)CLOCKS_PER_SEC ) {}

float ElapsedTime::GetElapsedTime() const {
    float fCurrentTime = std::clock() / (float)CLOCKS_PER_SEC;
    float fDeltaTime = fCurrentTime - m_fPrevious;
    m_fPrevious = fCurrentTime;

    // Clamp to the max time step
    fDeltaTime = std::min( fDeltaTime, m_fMaxTimeStep );

    return fDeltaTime;
}



#endif /* ElapsedTime_h */
