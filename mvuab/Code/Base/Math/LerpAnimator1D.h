//-----------------------------------------------------------------------
// CInterpolation class
// ...
//-----------------------------------------------------------------------
#ifndef LERP_ANIMATOR_1D_H
#define LERP_ANIMATOR_1D_H

#include "Utils\Types.h"
#include "Math\MathTypes.h"

namespace Math
{
class CLerpAnimator1D
{
public:
    CLerpAnimator1D (): m_fElapsedTime(0.f), m_bPause(false), m_fInitValue(0.f),
                        m_fEndValue(0.f), m_fTotalTime(0.f), m_eFunction(Math::FUNC_CONSTANT), m_uDegree(2) {}

    virtual ~CLerpAnimator1D () {/*NOTHING*/}
    
    void        SetValues            (float32 initValue, float32 endValue, float32 totalTime, ETypeFunction type); 
    float32     Value               ( float32 ElapsedTime );
    void        SetDegree            (uint32 degree) { m_uDegree = degree;}
    bool        Update                (float32 ElapsedTime, float32 &value);
    void        Pause                    (bool flag) {m_bPause = flag;}

    bool        IsFinish();
    
private:
    ETypeFunction    m_eFunction;
    bool            m_bPause;
    float32            m_fInitValue;
    float32            m_fEndValue;
    float32            m_fTotalTime;
    float32             m_fElapsedTime;        
    uint32            m_uDegree;
};

} //namespace Math
#endif //INTERPOLATION_H