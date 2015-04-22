//-----------------------------------------------------------------------
// CInterpolation class
// ...
//-----------------------------------------------------------------------
#ifndef LERP_ANIMATOR_3D_H
#define LERP_ANIMATOR_3D_H

#include "Math/Vector3.h"

namespace Math
{
class CLerpAnimator3D
{
public:
 CLerpAnimator3D ():    m_fElapsedTime(0.f), m_bPause(false), m_vInitValue(v3fZERO),
                                            m_vEndValue(v3fZERO), m_fTotalTime(0.f), m_eFunction(Math::FUNC_CONSTANT), m_uDegree(2) {}

    virtual ~CLerpAnimator3D () {/*NOTHING*/}

    void        SetValues            (Math::Vect3f initValue, Math::Vect3f endValue, float32 totalTime, ETypeFunction type);
    void        SetDegree            (uint32 degree) { m_uDegree = degree;}
    bool        Update                (float32 ElapsedTime, Math::Vect3f &value);
    void        Pause                    (bool flag) {m_bPause = flag;}

private:
    ETypeFunction    m_eFunction;
    bool                m_bPause;
    Math::Vect3f            m_vInitValue;
    Math::Vect3f            m_vEndValue;
    float32                m_fTotalTime;
    float32             m_fElapsedTime;
    uint32            m_uDegree;
};
} //namespace Math
#endif //INTERPOLATION_H