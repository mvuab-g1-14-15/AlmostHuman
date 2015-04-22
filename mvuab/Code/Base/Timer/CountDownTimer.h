//-----------------------------------------------------------------------
// CountDownTimer Class
// Utilidades para el control del tiempo
//-----------------------------------------------------------------------
#ifndef __COUNT_DOWN_TIMER_H__
#define __COUNT_DOWN_TIMER_H__

#include "Utils\Types.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// CountDownTimer Class
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CCountDownTimer
{
public:
    CCountDownTimer(float32 TotalTime, bool automaticReset = false): m_fTotalTime(TotalTime), m_fElapsedTime(0.f), m_bAutomaticReset(automaticReset) {}
    CCountDownTimer():                                m_fTotalTime(0.f), m_fElapsedTime(0.f), m_bAutomaticReset(false) {}
    virtual ~CCountDownTimer() {/*Nothing*/}

    void    SetTime                                    (float32 TotalTime, bool automaticReset = false);
    void    Reset                                        ()                { m_fElapsedTime    = 0.f; }
    bool    Update                                    (float32 deltaTime);
    bool    Finished                                ();

    float32    GetTotalTime                        () const    {return m_fTotalTime;}
    float32    GetElapsedTime                    () const    {return m_fElapsedTime;}
    float32    GetLeftoverTime                    () const    {return m_fTotalTime - m_fElapsedTime;}
    float32    GetElapsedTimeInPercent    () const    {return (m_fElapsedTime/m_fTotalTime)*100.f;}

private:
    float32    m_fTotalTime;
    float32    m_fElapsedTime;
    bool    m_bAutomaticReset;
};

#endif //__COUNT_DOWN_TIMER_H__
