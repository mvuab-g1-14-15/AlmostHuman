//----------------------------------------------------------------------------------
// CTimer class
// Author: Enric Vergara
//
// Description:
// Contiene toda la informacion relacionada con el tiempo, es decir:
//      - FPS: los frames por segundo a que va la aplicacion
//      - deltaTime: el tiempor que ha pasado dsd la ultima llamad a Update
//----------------------------------------------------------------------------------
#pragma once 
#ifndef INC_TIMERMANAGER_H_
#define INC_TIMERMANAGER_H_

#include "Utils\Types.h"
#include "Utils\SingletonPattern.h"

class CTimer : public CSingleton<CTimer>
{

public:

        CTimer(uint32 avgSamples);
        ~CTimer();

        void Update();
        float32 GetElapsedTime() const { return m_DeltaTime; }
        float32 GetFPS() const { return m_FPS; }
        float32 GetMaxFPS() const { return m_MaxFps; }
        float32 GetMinFPS() const { return m_MinFps; }

private:

        float32         m_DeltaTime;            // deltaTime
        float32         m_FPS;                  // FPS
        float32         m_MinFps;
        float32         m_MaxFps;
        float32         m_fLastFps;
        float32         m_fFPSTime;
        float64         m_dLastTime;
        uint32          m_NumberFPSCount;
        float32*        m_Deltas;               // array of instant delta times
        uint32          m_uSamples;             // deltas arrays length
        uint32          m_uIndex;               // current array position
};

#define deltaTime CTimer::GetSingletonPtr()->GetElapsedTime()
#define FPS       CTimer::GetSingletonPtr()->GetFPS()
#define MinFPS    CTimer::GetSingletonPtr()->GetMinFPS()
#define MaxFPS    CTimer::GetSingletonPtr()->GetMaxFPS()

#endif //INC_TIMERMANAGER_H_