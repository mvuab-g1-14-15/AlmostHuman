#include "Gizmos/GizmoElement.h"


#include "GraphicsManager.h"
#include "EngineManagers.h"

#include "Math/Vector3.h"
#include "Math/Matrix44.h"

#include "Shapes\Shapes.h"

#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"

CGizmoElement::CGizmoElement( EGizmoElementType type, float size, Math::Vect3f position, float yaw, float pitch,
                              Math::CColor color )
    : CObject3D( position, yaw, pitch )
    , m_Type( type )
    , m_Size( size )
    , m_Color( color )
	, mShape( 0 )
	, mTechnique( EffectManagerInstance->GetEffectTechnique("RenderForwardDebugShapeTechnique") )
{
	if ( m_Type == eCube )
		mShape = new CBoxShape();

    if ( m_Type == eSphere )
	{
		//ASSERT( false, "Sphere shape not implemented, using box shape" );
		mShape = new CBoxShape();
	}
}

CGizmoElement::~CGizmoElement()
{
}

void CGizmoElement::Init()
{
}

void CGizmoElement::Update()
{
    MakeTransform();
}

void CGizmoElement::Render( Math::Mat44f transform )
{
    mShape->SetTransform( transform * m_Transform );
    mShape->Render( mTechnique );
}
