#pragma once
#ifndef __COUNT_DOWN_TIMER_MANAGER_H__
#define __COUNT_DOWN_TIMER_MANAGER_H__

#include "Utils\Types.h"
#include "Engine.h"
#include "Utils\MapManager.h"
#include "Utils/Manager.h"
#include "XML\XMLTreeNode.h"
#include <string>

class CCountDownTimer;

class CCountDownTimerManager : public CMapManager<CCountDownTimer>, public CManager
{
public:
    CCountDownTimerManager( );
    CCountDownTimerManager( CXMLTreeNode& atts );
    ~CCountDownTimerManager();

    void Update() ;
    void Render() {}
    void AddTimer( std::string _KeyName, float _TotalTime, bool _AutomaticReset = false );
    void Init();


    bool isTimerFinish( std::string _KeyName );
    void SetTime( std::string _KeyName , float32 _Time, bool AutomaticReset );
    float32 GetTime( std::string _KeyName );

    void ChangeTotalTime( std::string _KeyName , float32 _Time );

    float32 GetElapsedTimeInPercent( std::string _KeyName );
    float32 GetLeftoverTime( std::string _KeyName );
    float32 GetElpasedTime( std::string _KeyName );

    void Reset( std::string _KeyName, bool active = true );
    bool ExistTimer( std::string _KeyName );
    bool IsActive( std::string _KeyName );
    void SetActive( std::string _KeyName, bool active );
};

#endif // __COUNT_DOWN_TIMER_MANAGER_H_
