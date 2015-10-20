#include "BillboardInstance.h"
#include "Texture\Texture.h"
#include "Texture\TextureManager.h"

#include "Cameras\CameraFPShooter.h"
#include "Cameras\CameraManager.h"
#include "GraphicsManager.h"

#include "RenderableVertex\VertexTypes.h"
#include "RenderableVertex\RenderableVertex.h"
#include "RenderableVertex\IndexedVertexs.h"
#include "Effects/EffectManager.h"
#include "EngineManagers.h"
#include "EngineConfig.h"
#include <string>

CBillboardInstance::CBillboardInstance()
    : CName()
    , CObject3D()
    , mAngle( 0.1f )
{
}

CBillboardInstance::~CBillboardInstance()
{
}

bool CBillboardInstance::Init( const CXMLTreeNode& atts )
{
    bool lOk( true );
    SetName( atts.GetAttribute<std::string>( "name", "unknown" ) );
    lOk = lOk && CObject3D::Init( atts );
    mIsVisible = atts.GetAttribute<bool>( "active", false );
    return lOk;
}

void CBillboardInstance::Update()
{
    MakeTransform();
}

void CBillboardInstance::Render(CRenderableVertexs* aRV,CGraphicsManager* aGM, CEffectTechnique* aTechnique )
{
    if ( mIsVisible )
    {
        aTechnique->GetEffect()->SetAngle( mAngle );
        aGM->SetTransform( GetTransform() );
        aRV->Render( aGM, aTechnique );
        aGM->SetTransform( Math::Mat44f() );
    }
}