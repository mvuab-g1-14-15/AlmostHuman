#include "Timer/CountDownTimerManager.h"
#include "CountDownTimer.h"

#include "Timer\Timer.h"

CCountDownTimerManager::CCountDownTimerManager()
    : CManager()
{
}

CCountDownTimerManager::CCountDownTimerManager( CXMLTreeNode& atts )
    : CManager( atts )

{
}


CCountDownTimerManager::~CCountDownTimerManager()
{
}

void CCountDownTimerManager::Init()
{
}

void CCountDownTimerManager::AddTimer( std:: string _KeyName, float _TotalTime, bool _AutomaticReset )
{
    CCountDownTimer* l_Timer = GetResource( _KeyName );

    if ( l_Timer )
        l_Timer->SetTime( _TotalTime, _AutomaticReset );
    else
        AddResource( _KeyName, new CCountDownTimer( _TotalTime, _AutomaticReset ) );
}

void CCountDownTimerManager::Update()
{
    TMapResource::iterator it = m_Resources.begin(),
                           it_end = m_Resources.end();

    for ( ; it != it_end; ++it )
        it->second->Update( deltaTimeMacro );
}

bool CCountDownTimerManager::isTimerFinish( std::string _KeyName )
{
    CCountDownTimer* l_Timer = ( CCountDownTimer* )GetResource( _KeyName );
    return l_Timer->Finished();
}

void CCountDownTimerManager::SetTime( std::string _KeyName , float32 _Time, bool _AutomaticReset )
{
    CCountDownTimer* l_Timer = ( CCountDownTimer* )GetResource( _KeyName );
    l_Timer->SetTime( _Time, _AutomaticReset );
}

float32 CCountDownTimerManager::GetTime( std::string _KeyName )
{
    CCountDownTimer* l_Timer = ( CCountDownTimer* )GetResource( _KeyName );
    return l_Timer->GetTotalTime();
}

float32 CCountDownTimerManager::GetElpasedTime( std::string _KeyName )
{
    CCountDownTimer* l_Timer = ( CCountDownTimer* )GetResource( _KeyName );
    return l_Timer->GetElapsedTime();
}

float32 CCountDownTimerManager::GetLeftoverTime( std::string _KeyName )
{
    CCountDownTimer* l_Timer = ( CCountDownTimer* )GetResource( _KeyName );
    return l_Timer->GetLeftoverTime();
}

float32 CCountDownTimerManager::GetElapsedTimeInPercent( std::string _KeyName )
{
    CCountDownTimer* l_Timer = ( CCountDownTimer* )GetResource( _KeyName );
    return l_Timer->GetElapsedTimeInPercent();
}

void CCountDownTimerManager::Reset( std::string _KeyName )
{
    CCountDownTimer* l_Timer = ( CCountDownTimer* )GetResource( _KeyName );
    l_Timer->Reset();
}

void CCountDownTimerManager::ChangeTotalTime( std::string _KeyName , float32 _Time )
{
    CCountDownTimer* l_Timer = ( CCountDownTimer* )GetResource( _KeyName );
    l_Timer->ChangeTotalTime( _Time );
}
