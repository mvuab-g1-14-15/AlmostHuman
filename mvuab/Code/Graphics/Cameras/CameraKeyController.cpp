#include "CameraKeyController.h"
#include "Utils\Defines.h"
#include "Logger\Logger.h"
#include "CameraKey.h"
#include "CameraInfo.h"
#include "Timer/Timer.h"
#include "Utils/BaseUtils.h"
#include "Utils\StringUtils.h"
#include "Math\LerpAnimator3D.h"
#include "Math\Vector3.h"
#include "Math\LerpAnimator1D.h"
#include <sstream>
#include "ScriptManager.h"
#include "EngineManagers.h"

CCameraKeyController::CCameraKeyController()
    :
    m_CurrentKey(0),
    m_NextKey(1),
    m_CurrentTime(0),
    m_TotalTime(0),
    m_Cycle(false),
    m_Reverse(false),
    m_Finish(false),
    m_PlayingBackward(false),
    m_PlayingForward(true),
    m_FileName(""),
    m_pCameraInfo(new CCameraInfo()),
	m_bLuaEnable( false ),
	m_LuaCode( "" ),
	m_KeyAction( 0.0f )
{
}

CCameraKeyController::CCameraKeyController(CXMLTreeNode &atts)
    :
    m_CurrentKey(0),
    m_NextKey(1),
    m_CurrentTime(0),
    m_TotalTime(0),
    m_Cycle(false),
    m_Reverse(false),
    m_Finish(false),
    m_PlayingBackward(false),
    m_PlayingForward(true),
    m_FileName(""),
    m_pCameraInfo(new CCameraInfo()),
	m_bLuaEnable( false ),
	m_LuaCode( "" ),
	m_KeyAction( 0.0f )
{
}

bool CCameraKeyController::LoadXML(const std::string &FileName)
{
    bool lOk = false;
    // Obtain the filename
    m_FileName = FileName;

    // Check if the file exist
    CXMLTreeNode newFile, l_Node;
    if (!newFile.LoadAndFindNode(m_FileName.c_str(), "camera_key_controller", l_Node ))
    {
        SetName( l_Node.GetAttribute<std::string>("name", "no_name") );
        m_Cycle = (l_Node.GetAttribute<int32>("cycle", 0) != 0);
        m_Reverse = (l_Node.GetAttribute<int32>("reverse", 0) != 0);
        m_TotalTime = l_Node.GetAttribute<float>("total_time", 0.0f);
		m_LuaCode = l_Node.GetAttribute<std::string>("lua_code", "");
		m_KeyAction = l_Node.GetAttribute<float>("key_action", 0.0f);
		if(m_LuaCode != "")
			m_bLuaEnable = true;
        for(uint32 i = 0, lCount = l_Node.GetNumChildren(); i < lCount ; i++)
        {
            const CXMLTreeNode &l_CurrentNode = l_Node(i);
            const std::string &l_TagName = l_CurrentNode.GetName();

            if( l_TagName == "key" )
            {
                float32 l_Time = l_CurrentNode.GetAttribute<float>("time", 0.0f);
                m_Keys.push_back( new CCameraKey( CCameraInfo( l_CurrentNode ) , l_Time ) );
            }
        }

        //Check that there are more than one key, in order to set the next key to 0
        if(m_Keys.size() == 1 )
        {
            m_NextKey = 0;
        }
        lOk = true;
    }


    return lOk;
}

bool CCameraKeyController::LoadXML(CXMLTreeNode &aNode)
{
    if(!aNode.Exists())
    {
        LOG_ERROR_APPLICATION( "CCameraKeyController::Load Tag \"%s\" no existe", "camera_key_controller");
        return false;
    }

    SetName( aNode.GetAttribute<std::string>("name", "no_name") );
    m_Cycle = (aNode.GetAttribute<int32>("cycle", 0) != 0);
    m_Reverse = (aNode.GetAttribute<int32>("reverse", 0) != 0);
    m_TotalTime = aNode.GetAttribute<float>("total_time", 0.0f);
    m_LuaCode = aNode.GetAttribute<std::string>("lua_code", "");
    m_KeyAction = aNode.GetAttribute<float>("key_action", 0.0f);
    if(m_LuaCode != "")
      m_bLuaEnable = true;

    for(uint32 i = 0, lCount = aNode.GetNumChildren(); i < lCount ; i++)
    {
        const CXMLTreeNode &l_CurrentNode = aNode(i);
        const std::string &l_TagName = l_CurrentNode.GetName();

        if( l_TagName == "key" )
        {
            float32 l_Time = l_CurrentNode.GetAttribute<float>("time", 0.0f);
            m_Keys.push_back( new CCameraKey( CCameraInfo( l_CurrentNode ) , l_Time ) );
        }
    }

    //Check that there are more than one key, in order to set the next key to 0
    if(m_Keys.size() == 1 )
    {
        m_NextKey = 0;
    }

    return true;
}

void CCameraKeyController::GetCurrentKeyForward()
{
    for( uint32 i = m_CurrentKey; i < m_Keys.size(); ++i )
    {
        if(m_Keys[i]->GetTime() > m_CurrentTime )
        {
            m_CurrentKey = i - 1;
            m_NextKey = i;
            return;
        }
    }
}

void CCameraKeyController::GetCurrentKeyBackward()
{
    for( uint32 i = m_CurrentKey; i > -1 ; --i )
    {
        if(m_Keys[i]->GetTime() < m_CurrentTime )
        {
            m_CurrentKey = i + 1;
            m_NextKey = i;
            return;
        }
    }

    // Something is wrong, therfore use always the first key
    m_CurrentKey = 0;
}

CCameraKeyController::~CCameraKeyController()
{
    for( uint32 i = 0; i < m_Keys.size(); ++i )
    {
        CHECKED_DELETE(m_Keys[i])
    }

    m_Keys.clear();

    CHECKED_DELETE(m_pCameraInfo)
}

void CCameraKeyController::Update()
{
    // Check if the animation of the camera has finished
    if( m_Finish )
    {
        return;
    }

    if( m_PlayingBackward )
    {
        PlayBackward();
		if(m_bLuaEnable && m_KeyAction > m_CurrentTime)
		{
			ScriptMInstance->RunCode(m_LuaCode);
			m_bLuaEnable = false;
		}
    }
    else if( m_PlayingForward )
    {
        PlayFoward();
		if(m_bLuaEnable && m_KeyAction < m_CurrentTime)
		{
			ScriptMInstance->RunCode(m_LuaCode);
			m_bLuaEnable = false;
		}
    }
	
}

void CCameraKeyController::SetCurrentTime(float32 CurrentTime)
{
    m_CurrentTime = CurrentTime;
}

void CCameraKeyController::ResetTime()
{
    m_CurrentTime = 0;
}

float32 CCameraKeyController::GetTotalTime()
{
    return m_TotalTime;
}

bool CCameraKeyController::IsCycle() const
{
    return m_Cycle;
}

void CCameraKeyController::SetCycle(bool Cycle)
{
    m_Cycle = Cycle;
}

bool CCameraKeyController::IsReverse() const
{
    return m_Reverse;
}

void CCameraKeyController::SetReverse(bool Reverse)
{
    m_Reverse = Reverse;
}

void CCameraKeyController::PlayBackward()
{
    // Decrement the time
    m_CurrentTime -= deltaTimeMacro;

    // Obtain the current key
    GetCurrentKeyBackward();
	
    // Obtain the current camera info and the next one
    CCameraInfo l_CurrentCameraInfo = m_Keys[m_CurrentKey]->GetCameraInfo();
    CCameraInfo l_NextCameraInfo = m_Keys[m_NextKey]->GetCameraInfo();

    // Calculate the % of the animation, in order to interpolate key by key
    float32 l_CurrentP = (m_CurrentTime - m_Keys[m_CurrentKey]->GetTime()) / (m_Keys[m_NextKey]->GetTime() -
                         m_Keys[m_CurrentKey]->GetTime());

    InterpolateKeys( l_CurrentP , l_CurrentCameraInfo, l_NextCameraInfo );

    if( m_CurrentTime <= 0.0f )
    {
        // If the animation was going Backwards, means that it is a cycle
        m_PlayingForward = true;
        m_PlayingBackward = false;
        m_CurrentTime = 0.0f;
        m_CurrentKey = 0;
    }
}

void CCameraKeyController::PlayFoward()
{
    // Increment the time
    m_CurrentTime += deltaTimeMacro;

    // Obtain the current key
    GetCurrentKeyForward();
    CCameraInfo l_CurrentCameraInfo = m_Keys[m_CurrentKey]->GetCameraInfo();
    CCameraInfo l_NextCameraInfo = m_Keys[m_NextKey]->GetCameraInfo();

    // Calculate the % of the animation, in order to interpolate key by key
    float32 l_CurrentP = (m_CurrentTime - m_Keys[m_CurrentKey]->GetTime()) / (m_Keys[m_NextKey]->GetTime() -
                         m_Keys[m_CurrentKey]->GetTime());

    InterpolateKeys( l_CurrentP , l_CurrentCameraInfo, l_NextCameraInfo );

    // Check if the animation has finished
    if( m_CurrentTime >= m_TotalTime )
    {
        if( m_Cycle )
        {
            if(m_Reverse)
            {
                m_PlayingForward = false;
                m_PlayingBackward = true;
                m_CurrentTime = m_TotalTime;
                m_CurrentKey = m_Keys.size() - 1;
            }
            else
            {
                m_CurrentKey = 0;
                m_CurrentTime = 0.0f;
            }
        }
        else
        {
            m_Finish = true;
        }
    }
}

void CCameraKeyController::InterpolateKeys(float32 Percentage, CCameraInfo A, CCameraInfo B)
{
    Math::CLerpAnimator3D l_Interpolator3D;
    Math::CLerpAnimator1D l_Interpolator1D;

    // Obtain the current camera position
    l_Interpolator3D.SetValues(A.GetEye(), B.GetEye(), 1.0f, Math::FUNC_CONSTANT);
    Math::Vect3f l_Eye;
    l_Interpolator3D.Update(Percentage, l_Eye);
    m_pCameraInfo->SetEye(l_Eye);

    // Obtain the current camera look at
    l_Interpolator3D.SetValues(A.GetLookAt(), B.GetLookAt(), 1.0f, Math::FUNC_CONSTANT);
    Math::Vect3f l_LookAt;
    l_Interpolator3D.Update(Percentage, l_LookAt);
    m_pCameraInfo->SetLookAt(l_LookAt);

    // Obtain the current camera up vector
    l_Interpolator3D.SetValues(A.GetUp(), B.GetUp(), 1.0f, Math::FUNC_CONSTANT);
    Math::Vect3f l_Up;
    l_Interpolator3D.Update(Percentage, l_Up);
    m_pCameraInfo->SetUp(l_Up);

    // Obtain the current camera far plane
    l_Interpolator1D.SetValues(A.GetFarPlane(), B.GetFarPlane(), 1.0f, Math::FUNC_CONSTANT);
    float32 l_FarPlane;
    l_Interpolator1D.Update(Percentage, l_FarPlane);
    m_pCameraInfo->SetFarPlane(l_FarPlane);

    // Obtain the current camera near plane
    l_Interpolator1D.SetValues(A.GetNearPlane(), B.GetNearPlane(), 1.0f, Math::FUNC_CONSTANT);
    float32 l_NearPlane;
    l_Interpolator1D.Update(Percentage, l_NearPlane);
    m_pCameraInfo->SetNearPlane(l_NearPlane);

    // Obtain the current camera FOV
    /*l_Interpolator1D.SetValues(A.GetFOV(), B.GetFOV(), 1.0f, Math::FUNC_CONSTANT);
    float32 l_FOV;
    l_Interpolator1D.Update(Percentage, l_FOV);
    m_pCameraInfo->SetFOV(l_FOV);*/
}