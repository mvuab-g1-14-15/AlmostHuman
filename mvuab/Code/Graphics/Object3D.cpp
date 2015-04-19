#include "Object3D.h"
#include "XML\XMLTreeNode.h"

CObject3D::CObject3D( const CXMLTreeNode& atts )
    : m_Position( atts.GetVect3fProperty( "pos", Math::Vect3f( 0, 0, 0 ) ) )
    , m_fYaw( atts.GetFloatProperty( "yaw", 0.0f ) )
    , m_fPitch( atts.GetFloatProperty( "pitch", 0.0f ) )
    , m_fRoll( atts.GetFloatProperty( "roll", 0.0f ) )
    , m_Scale( atts.GetVect3fProperty( "scale", Math::Vect3f( 1.0f, 1.0f, 1.0f ) ) )
{
    MakeTransform();
}

CObject3D::CObject3D( const Math::Vect3f& pos, float32 yaw, float32 pitch, float32 roll, Math::Vect3f scale )

{
    MakeTransform();
}

CObject3D::CObject3D()
    : m_Position( Math::Vect3f( 0.f, 0.f, 0.f ) )
    , m_fYaw( 0.f )
    , m_fPitch( 0.f )
    , m_fRoll( 0.f )
    , m_Scale( Math::Vect3f( 1.0f, 1.0f, 1.0f ) )
{
}

void CObject3D::MakeTransform()
{
    Math::Mat44f world;
    Math::Mat44f scale;

    Math::Mat44f rotationY;
    Math::Mat44f rotationX;
    Math::Mat44f rotationZ;
    Math::Mat44f translation;

    translation.SetFromPos( m_Position );
    scale.SetFromScale( m_Scale.x, m_Scale.y, m_Scale.z );

    rotationX.SetFromAngleX( m_fPitch );
    rotationY.SetFromAngleY( m_fYaw );
    rotationZ.SetFromAngleZ( m_fRoll );

    m_Transform = translation * scale * ( rotationX * rotationY * rotationZ );
}

Math::Vect3f CObject3D::GetDirection() const
{
	Math::Vect3f l_Direction( Math::Utils::Cos( m_fYaw ) * Math::Utils::Cos( m_fPitch ), Math::Utils::Sin( m_fPitch ),
							Math::Utils::Sin( m_fYaw ) * Math::Utils::Cos( m_fPitch ) );
	return l_Direction;
}	