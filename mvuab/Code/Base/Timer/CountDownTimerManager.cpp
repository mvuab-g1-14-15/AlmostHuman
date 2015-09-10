#include "Timer/CountDownTimerManager.h"
#include "CountDownTimer.h"

#include "Timer\Timer.h"

CCountDownTimerManager::CCountDownTimerManager() : CManager()
{
}

CCountDownTimerManager::CCountDownTimerManager(CXMLTreeNode& atts) : CManager(atts)
{
}


CCountDownTimerManager::~CCountDownTimerManager()
{
    Destroy();
}

void CCountDownTimerManager::Init()
{
}

void CCountDownTimerManager::AddTimer(std:: string _KeyName, float _TotalTime, bool _AutomaticReset)
{
    CCountDownTimer* l_Timer = GetResource(_KeyName);

    if(l_Timer) l_Timer->SetTime(_TotalTime, _AutomaticReset);
    else AddResource(_KeyName, new CCountDownTimer(_TotalTime, _AutomaticReset));
   
}

void CCountDownTimerManager::Update()
{
    TMapResource::iterator it = m_Resources.begin(),
                           it_end = m_Resources.end();

    if(it == it_end) return;

    do{
        CCountDownTimer* l_Timer = it->second;
        if(l_Timer->isActive()) l_Timer->Update(deltaTimeMacro);
    }while(++it != it_end);
}

bool CCountDownTimerManager::isTimerFinish(std::string _KeyName)
{
    CCountDownTimer* l_Timer = (CCountDownTimer *) GetResource(_KeyName);
    return l_Timer ? l_Timer->Finished() : false;
}

void CCountDownTimerManager::SetTime(std::string _KeyName , float32 _Time, bool _AutomaticReset)
{
    CCountDownTimer* l_Timer = (CCountDownTimer*) GetResource(_KeyName);
    l_Timer->SetTime(_Time, _AutomaticReset);
}

float32 CCountDownTimerManager::GetTime(std::string _KeyName)
{
    CCountDownTimer* l_Timer = (CCountDownTimer *) GetResource(_KeyName);
    return l_Timer->GetTotalTime();
}

float32 CCountDownTimerManager::GetElpasedTime(std::string _KeyName)
{
    CCountDownTimer* l_Timer = (CCountDownTimer *) GetResource(_KeyName);
    return l_Timer->GetElapsedTime();
}

float32 CCountDownTimerManager::GetLeftoverTime(std::string _KeyName)
{
    CCountDownTimer* l_Timer = (CCountDownTimer *) GetResource(_KeyName);
    return l_Timer->GetLeftoverTime();
}

float32 CCountDownTimerManager::GetElapsedTimeInPercent(std::string _KeyName)
{
    CCountDownTimer* l_Timer = (CCountDownTimer *) GetResource(_KeyName);
    return l_Timer->GetElapsedTimeInPercent();
}

void CCountDownTimerManager::Reset(std::string _KeyName, bool active)
{
    CCountDownTimer* l_Timer = (CCountDownTimer *) GetResource(_KeyName);
    if (l_Timer)
      l_Timer->Reset(active);
}

void CCountDownTimerManager::ChangeTotalTime(std::string _KeyName , float32 _Time)
{
    CCountDownTimer* l_Timer = (CCountDownTimer *) GetResource(_KeyName);
    l_Timer->ChangeTotalTime(_Time);
}

bool CCountDownTimerManager::ExistTimer(std::string _KeyName)
{
    CCountDownTimer* l_Timer = (CCountDownTimer *) GetResource(_KeyName);
    return l_Timer ? true : false;
}

bool CCountDownTimerManager::IsActive(std::string _KeyName)
{
    CCountDownTimer* l_Timer = (CCountDownTimer *) GetResource(_KeyName);
    return l_Timer ? l_Timer->isActive() : false;
}

void CCountDownTimerManager::SetActive(std::string _KeyName, bool active)
{
    CCountDownTimer* l_Timer = (CCountDownTimer*)GetResource(_KeyName);
    l_Timer->SetActive(active);
}
