#include "LerpAnimator2D.h"
#include <cmath>
#include <assert.h>

void Math::CLerpAnimator2D::SetValues (Math::Vect2f initValue, Math::Vect2f endValue, float32 totalTime, ETypeFunction type)
{
    assert( totalTime > 0);

    m_vInitValue        = initValue;
    m_vEndValue            = endValue;
    m_fTotalTime        = totalTime;
    m_fElapsedTime    = 0.f;
    m_eFunction            = type;
}

bool Math::CLerpAnimator2D::Update (float32 ElapsedTime, Math::Vect2f &value)
{
    m_fElapsedTime += ElapsedTime;
    bool finish = false;
    if (m_fElapsedTime >= m_fTotalTime)
    {
        finish = true;
        m_fElapsedTime = m_fTotalTime;
    }

    //mu [0-1]
    float32 mu = 0.f;
    mu = m_fElapsedTime/m_fTotalTime;

    //En funcion del tiempo la siguiente funcion nos retorna un valor entre 0-1.
    switch(m_eFunction)
    {
    case FUNC_CONSTANT:
        {
            //Linear
            //nothing to do
        }
        break;
    case FUNC_INCREMENT:
        {
            mu = Math::Utils::PowN(mu,m_uDegree);
        }
        break;
    case FUNC_DECREMENT:
        {
            mu = pow(mu,(float32)(1/m_uDegree));
        }
        break;
    }

    value = m_vInitValue.GetLerp(m_vEndValue,mu);

    return finish;
}