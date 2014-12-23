#include "CameraManager.h"
#include "CameraFPShooter.h"
#include "GraphicsManager.h"

CCameraManager::CCameraManager() : m_RenderCameras( false )
{
    //---Por defecto insertamos la camara del player----
    CCamera * l_pFPSCamera = new CCameraFPShooter();
    std::string name_Cameraplayer = "player";
    AddResource("player", l_pFPSCamera);

    SetCurrentCamera("player");
}

CCameraManager::~CCameraManager()
{
    Destroy();
}

CCamera* CCameraManager::GetCamera( const std::string &name )
{
    if( m_Resources.find(name) != m_Resources.end() )
        return m_Resources[name];
    return 0;
}

void CCameraManager::SetCurrentCamera( const std::string &name )
{
    CCamera* l_pNewCurrentCamera = GetCamera(name);
    if( l_pNewCurrentCamera )
        m_pCurrentCamera = l_pNewCurrentCamera;
}

std::string CCameraManager::GetCurrentCameraName()
{
    assert(m_pCurrentCamera);
    return m_pCurrentCamera->GetName();
}

void CCameraManager::NewCamera( CCamera::Type id_cam, const std::string &name, const Math::Vect3f &Eye,
                                const Math::Vect3f &LookAt )
{
    CCamera * l_pNewCamera = 0;
        
    switch(id_cam)
    {
        case CCamera::FPS:
            l_pNewCamera = new  CCameraFPShooter();
            break;
        case CCamera::TPS:
             //l_pNewCamera = new CCameraThPShooter();
           break;
        case CCamera::SPHERICAL:
             //l_pNewCamera =new CCameraFPShooter();
           break;
        case CCamera::CINEMATICAL:
             //new_l_pNewCameracamera =new CCameraFPShooter();
           break;
    }

    assert(l_pNewCamera);

    Math::Vect3f d = LookAt-Eye;
    l_pNewCamera->SetYaw(Math::Utils::ATan2(d.z, d.x));
    l_pNewCamera->SetPitch(Math::Utils::ATan2(d.y,Math::Utils::Sqrt(d.z * d.z + d.x * d.x)));
    l_pNewCamera->SetPos(Eye);
    l_pNewCamera->SetName(name);
    AddResource(name,l_pNewCamera);
}



void CCameraManager::DeleteCamera(const std::string &name)
{
    assert(m_pCurrentCamera);
    if( m_pCurrentCamera->GetName() != name )
    {
        m_Resources.erase( name );
    }
}

void CCameraManager::RenderCameras()
{
#ifdef _DEBUG
    if(m_RenderCameras)
    {
        LPDIRECT3DDEVICE9 l_pDirectXDevice = CGraphicsManager::GetSingletonPtr()->GetDevice();
        TMapResource::iterator itb = m_Resources.begin(), ite = m_Resources.end();
        for( ; itb != ite; ++itb )
            itb->second->RenderCamera( l_pDirectXDevice );
    }
#endif
}

void CCameraManager::Update( float dt )
{
    TMapResource::iterator itb = m_Resources.begin(), ite = m_Resources.end();
    for( ; itb != ite; ++itb )
        itb->second->Update(dt);
}