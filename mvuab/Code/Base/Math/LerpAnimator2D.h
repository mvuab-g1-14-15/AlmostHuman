//-----------------------------------------------------------------------
// CLerpAnimator2D class
// ...
//-----------------------------------------------------------------------
#ifndef LERP_ANIMATOR_2D_H
#define LERP_ANIMATOR_2D_H

#include "Math\Vector2.h"
#include "Utils\Types.h"


typedef enum ETypeFunction { FUNC_CONSTANT, FUNC_INCREMENT, FUNC_DECREMENT};


class CLerpAnimator2D
{
public:
 CLerpAnimator2D ():    m_fElapsedTime(0.f), m_bPause(false), m_vInitValue(v2fZERO),
                                            m_vEndValue(v2fZERO), m_fTotalTime(0.f), m_eFunction(FUNC_CONSTANT), m_uDegree(2) {}

    virtual ~CLerpAnimator2D () {/*NOTHING*/}
    
    void        SetValues            (Vect2f initValue, Vect2f endValue, float32 totalTime, ETypeFunction type); 
    void        SetDegree            (uint32 degree) { m_uDegree = degree;}
    bool        Update                (float32 deltaTime, Vect2f &value);
    void        Pause                    (bool flag) {m_bPause = flag;}
    
private:
    ETypeFunction    m_eFunction;
    bool                    m_bPause;
    Vect2f                m_vInitValue;
    Vect2f                m_vEndValue;
    float32                    m_fTotalTime;
    float32                 m_fElapsedTime;        
    uint32                m_uDegree;
};

#endif //INTERPOLATION_H