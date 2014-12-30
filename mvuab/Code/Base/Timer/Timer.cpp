#include <Windows.h>
#include <assert.h>
#include <float.h>
#include "Timer.h"

//Inicializamos todas las variables
CTimer::CTimer (uint32 avgSamples)
    : m_uSamples(avgSamples)
    , m_DeltaTime(0.0f)
    , m_dLastTime(0)
    , m_FPS(0.0f)
    , m_fFPSTime(0.0f)
    , m_NumberFPSCount(0)
    , m_fLastFps(0.0f)
    , m_uIndex(0)
    , m_MinFps(FLT_MAX)
    , m_MaxFps(-FLT_MAX)
{
    m_Deltas = new float32[m_uSamples];
    for(uint32 j=0;j<m_uSamples;j++)
    {
            m_Deltas[j] = 0;
}
}

CTimer::~CTimer( void )
{
    delete[] m_Deltas;
}

// Actualiza los valores FPS, deltaTime y los timers (los normales y los inputs)
void CTimer::Update( void )
{
    //-----Actualizacion de los valores FPS y ElpasedTime-----
    //Calculo de la diferencia de tiempo (m_fElpasedTime)
    float64  l_dCurTime = timeGetTime();

    if( m_dLastTime == 0 ) 
        m_dLastTime = l_dCurTime;

    m_Deltas[m_uIndex] = (float32)((l_dCurTime - m_dLastTime) * 0.001);
    m_dLastTime    = l_dCurTime;

    float32 d = 0;
    for(unsigned int j=0; j<m_uSamples; j++)
    {
            d += m_Deltas[j];
    }
    d /= (float32)m_uSamples;
    m_DeltaTime = d;
    m_uIndex = (++m_uIndex) % m_uSamples;

    //Calculo de los frames por segundo (m_FPS)
    m_NumberFPSCount++;
    m_fFPSTime += m_DeltaTime;

    m_FPS = (m_fFPSTime > 0.0f ) ? m_NumberFPSCount/m_fFPSTime : 0.0f;

    if( l_dCurTime - m_fLastFps > 1000 )    //Solo mostramos los fps cada segundo
    {
            m_FPS = m_NumberFPSCount/m_fFPSTime;
            m_NumberFPSCount = 0;
            m_fFPSTime = 0;
            m_fLastFps = (float32)l_dCurTime;
            if( m_NumberFPSCount > 0 )
            {
                m_MinFps = ( m_MinFps < m_FPS ) ? m_MinFps : m_FPS;
                m_MaxFps = ( m_MaxFps > m_FPS ) ? m_MaxFps : m_FPS;
            }
    }
    //---------------------------------------------//
}