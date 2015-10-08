#include "Particles\InstanceParticle.h"
#include "EngineConfig.h"
#include "EngineManagers.h"
#include "Particles\ParticleSystemManager.h"
#include "Particles\ParticleSystemCore.h"
#include "GraphicsManager.h"

CInstanceParticle::CInstanceParticle( const CXMLTreeNode& atts )
    : CObject3D( atts )
    , mCore( ParticleSystemInstance->GetResource( atts.GetAttribute<std::string>( "core", "" ) ) )
	, mActive( atts.GetAttribute<bool>( "active", false )  )
{
    ASSERT( mCore, "Null core" );
}

CInstanceParticle::~CInstanceParticle()
{

}

void CInstanceParticle::Render()
{
	if( mCore && mActive )
    {
        CGraphicsManager* lGM = GraphicsInstance;
        lGM->SetTransform( GetTransform() );
        mCore->Render();
        lGM->SetTransform( Math::Mat44f() );
    }
}