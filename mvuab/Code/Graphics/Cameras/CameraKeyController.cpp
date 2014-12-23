#include "CameraKeyController.h"
#include "Utils\Defines.h"
#include "Logger\Logger.h"
#include "CameraKey.h"
#include "CameraInfo.h"

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
}

void CCameraKeyController::GetCurrentKey()
{
    if( m_Reverse ) 
    {
        for( uint32 i = m_Keys.size() - 1; i > -1 ; --i )
        {
            if(m_Keys[i]->GetTime() > ( m_TotalTime - m_CurrentTime ))
                m_CurrentKey = i;
        }

        // If the controller of the camera is a cycle means that it needs to start
        // again with the last key
        if( m_Cycle && m_CurrentTime > m_TotalTime )
        {
            m_CurrentTime = m_Keys.size() - 1;
        }
    }
    else
    {
        for( uint32 i = 0; i < m_Keys.size(); ++i )
        {
            if(m_Keys[i]->GetTime() > m_CurrentTime )
                m_CurrentKey = i;
        }

        // If the controller of the camera is a cycle means that it needs to start
        // again with the first key
        if( m_Cycle && m_CurrentTime > m_TotalTime )
        {
            m_CurrentTime = 0;
        }
    }
}
CCameraKeyController::CCameraKeyController(CXMLTreeNode &atts)
    :
    m_CurrentKey(0),
    m_NextKey(0),
    m_CurrentTime(0),
    m_TotalTime(0),
    m_Cycle(false),
    m_Reverse(false)
{

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
    m_CurrentTime += deltaTime;
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
