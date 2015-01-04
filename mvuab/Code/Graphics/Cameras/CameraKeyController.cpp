#include "CameraKeyController.h"
#include "Utils\Defines.h"
#include "Logger\Logger.h"
#include "CameraKey.h"
#include "CameraInfo.h"

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
    m_FileName("")
{
}

void CCameraKeyController::LoadXML(const std::string &FileName)
{
    // Obtain the filename
    m_FileName = FileName;
    
    // Check if the file exist
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(m_FileName.c_str()))
    {
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CCameraKeyController::Load No se puede abrir \"%s\"!", m_FileName.c_str());
        return;
    }

    // Parse the file and search for the key's
    CXMLTreeNode l_Node = newFile["camera_key_controller"];
    if(!l_Node.Exists())
    {
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CCameraKeyController::Load Tag \"%s\" no existe", "camera_key_controller");
        return;
    }

    SetName( l_Node.GetPszProperty("name","no_name") );
    m_Cycle = l_Node.GetBoolProperty("cycle", false);
    m_Reverse = l_Node.GetBoolProperty("reverse", false);
    m_TotalTime = l_Node.GetFloatProperty("total_time", 0.0f);

    for(int i = 0; i < l_Node.GetNumChildren(); i++)
    {
        CXMLTreeNode &l_CurrentNode =l_Node(i);
        const std::string &l_TagName = l_CurrentNode.GetName();

        if( l_TagName == "key" )
        {
            float32 l_Time = l_CurrentNode.GetFloatProperty("time", 0.0f);
            m_Keys.push_back( new CCameraKey( CCameraInfo( l_CurrentNode ) , l_Time ) );
        }
    }

    //Check that there are more than one key, in order to set the next key to 0
    if(m_Keys.size() == 1 )
        m_NextKey = 0;
}

void CCameraKeyController::GetCurrentKey()
{
    
}

void CCameraKeyController::GetCurrentKeyForward()
{
    for( uint32 i = m_CurrentKey; i < m_Keys.size(); ++i )
    {
        if(m_Keys[i]->GetTime() > m_CurrentTime )
        {
            m_CurrentKey = i - 1;
            return;
        }
    }

    // Something is wrong, therfore use always the first key
    m_CurrentKey = 0;
}

void CCameraKeyController::GetCurrentKeyBackward()
{
    for( uint32 i = m_CurrentKey; i > -1 ; --i )
    {
        if(m_Keys[i]->GetTime() < m_CurrentTime )
        {
            m_CurrentKey = i + 1;
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
}

void CCameraKeyController::Update(float32 deltaTime)
{
    // Check if the animation of the camera has finished
    if( m_Finish )
        return;

    if( m_PlayingBackward )
    {
        // Decrement the time
        m_CurrentTime -= deltaTime;
        GetCurrentKeyBackward();

        if( m_CurrentTime <= 0.0f )
        {
            // If the animation was going Backwards, means that it is a cycle
            m_PlayingForward = true;
            m_PlayingBackward = false;
            m_CurrentTime = 0.0f;
            m_CurrentKey = 0;
        }
    }
    else if( m_PlayingForward )
    {
        // Increment the time
        m_CurrentTime += deltaTime;
        GetCurrentKeyForward();

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
                    m_CurrentTime = 0.0f;
                }
            }
            else
            {
                m_Finish = true;
            }
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
