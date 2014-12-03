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

class CTimer
{

public:

        CTimer(uint32 avgSamples);
        ~CTimer();

        void                    Update                                  ();
        float32                   GetElapsedTime  () const { return m_fElpasedTime; }
        float32                   GetFPS                                  () const { return m_fFPS; }

private:

        float32                   m_fElpasedTime; // deltaTime
        float32                   m_fFPS;                                 // FPS
        float32                   m_fLastFps;
        float32                   m_fFPSTime;
        double          m_dLastTime;
        uint32          m_uFPSCount;
        float32*          m_Deltas;                               // array of instant delta times
        uint32          m_uSamples;                     // deltas arrays length
        uint32          m_uIndex;                               // current array position
};

#endif //INC_TIMERMANAGER_H_