#include "Object3D.h"
#include "XML\XMLTreeNode.h"

CObject3D::CObject3D( const CXMLTreeNode& atts )
    : m_Position( atts.GetVect3fProperty( "pos", Math::Vect3f( 0, 0, 0 ) ) )
    , m_fYaw( atts.GetAttribute<float>( "yaw", 0.0f ) )
    , m_fPitch( atts.GetAttribute<float>( "pitch", 0.0f ) )
    , m_fRoll( atts.GetAttribute<float>( "roll", 0.0f ) )
    , m_Scale( atts.GetVect3fProperty( "scale", Math::Vect3f( 1.0f, 1.0f, 1.0f ) ) )
{
    MakeTransform();
}

CObject3D::CObject3D( const Math::Vect3f& pos, float32 yaw, float32 pitch, float32 roll, Math::Vect3f scale )
{
    m_Position  = pos;
    m_fYaw      = yaw;
    m_fPitch    = pitch;
    m_fRoll     = roll;
    m_Scale     = scale;

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

bool CObject3D::Init( const CXMLTreeNode& atts )
{
    m_Position  = atts.GetVect3fProperty  ( "pos",    Math::Vect3f( 0, 0, 0 ) );
    m_fYaw      = atts.GetAttribute<float>   ( "yaw",    0.0f );
    m_fPitch    = atts.GetAttribute<float>   ( "pitch",  0.0f );
    m_fRoll     = atts.GetAttribute<float>   ( "roll",   0.0f );
    m_Scale     = atts.GetVect3fProperty  ( "scale",  Math::Vect3f( 1.0f, 1.0f, 1.0f ) );
    MakeTransform();

    return true;
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

const Math::Vect3f CObject3D::GetDirection() const
{
    Math::Vect3f l_Direction( Math::Utils::Cos( m_fYaw ) * Math::Utils::Cos( m_fPitch ), Math::Utils::Sin( m_fPitch ),
                              Math::Utils::Sin( m_fYaw ) * Math::Utils::Cos( m_fPitch ) );

    return l_Direction;
}

void CObject3D::SetDirection( const Math::Vect3f& aDirection )
{
    SetYaw(Math::Utils::ATan2( aDirection.z, aDirection.x ) );
    SetPitch(Math::Utils::ATan2( aDirection.y,
                                 Math::Utils::Sqrt( aDirection.z * aDirection.z + aDirection.x * aDirection.x ) ));
    SetRoll( 0.0f );
    MakeTransform();
}