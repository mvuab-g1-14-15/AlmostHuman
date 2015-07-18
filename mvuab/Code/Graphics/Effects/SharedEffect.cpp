#include "SharedEffect.h"

CSharedEffect::CSharedEffect( )
    : CEffect( "SharedEffect" )
    , CTOR_EFFECT_PARAMETER( WorldMatrix )
    , CTOR_EFFECT_PARAMETER( ViewMatrix )
    , CTOR_EFFECT_PARAMETER( ProjectionMatrix )
{
}

CSharedEffect::~CSharedEffect()
{
}

void CSharedEffect::SetNullParameters()
{
    RESET_EFFECT_PARAMETER( WVMatrix );
    RESET_EFFECT_PARAMETER( VPMatrix );
    RESET_EFFECT_PARAMETER( WVPMatrix );

    RESET_EFFECT_PARAMETER( WorldMatrix );
    RESET_EFFECT_PARAMETER( ViewMatrix );
    RESET_EFFECT_PARAMETER( ProjectionMatrix );

    RESET_EFFECT_PARAMETER( InverseWorldMatrix );
    RESET_EFFECT_PARAMETER( InverseViewMatrix );
    RESET_EFFECT_PARAMETER( InverseProjectionMatrix );
}

void CSharedEffect::LinkSemantics()
{
    RESET_EFFECT_PARAMETER( WVMatrix );
    RESET_EFFECT_PARAMETER( VPMatrix );
    RESET_EFFECT_PARAMETER( WVPMatrix );

    RESET_EFFECT_PARAMETER( WorldMatrix );
    RESET_EFFECT_PARAMETER( ViewMatrix );
    RESET_EFFECT_PARAMETER( ProjectionMatrix );

    RESET_EFFECT_PARAMETER( InverseWorldMatrix );
    RESET_EFFECT_PARAMETER( InverseViewMatrix );
    RESET_EFFECT_PARAMETER( InverseProjectionMatrix );
}

bool CSharedEffect::SetCameraPosition( const Math::Vect3f &CameraPosition )
{
    float32 l_Camera[3];
    l_Camera[0] = CameraPosition.x;
    l_Camera[1] = CameraPosition.y;
    l_Camera[2] = CameraPosition.z;
    return ( m_Effect->SetFloatArray( m_CameraPosition, l_Camera, 3 ) == S_OK );
}

bool CSharedEffect::SetWorldMatrix( const Math::Mat44f& Matrix )
{
    return S_OK == SET_MATRIX_PARAMETER( WorldMatrix, Matrix );
}

bool CSharedEffect::SetViewMatrix( const Math::Mat44f& Matrix )
{
    return S_OK == SET_MATRIX_PARAMETER( ViewMatrix, Matrix );
}

bool CSharedEffect::SetProjectionMatrix( const Math::Mat44f& Matrix )
{
    return S_OK == SET_MATRIX_PARAMETER( ProjectionMatrix, Matrix );
}

bool CSharedEffect::SetInverseWorldMatrix( const Math::Mat44f& Matrix )
{
   return S_OK == SET_MATRIX_PARAMETER( InverseWorldMatrix, Matrix.GetInverted() );
}

bool CSharedEffect::SetInverseViewMatrix( const Math::Mat44f& Matrix )
{
   return S_OK == SET_MATRIX_PARAMETER( InverseViewMatrix, Matrix.GetInverted() );
}

bool CSharedEffect::SetInverseProjectionMatrix( const Math::Mat44f& Matrix )
{
   return S_OK == SET_MATRIX_PARAMETER( InverseProjectionMatrix, Matrix.GetInverted() );
}

bool CSharedEffect::SetWorldViewMatrix( const Math::Mat44f& Matrix )
{
   return S_OK == SET_MATRIX_PARAMETER( WVMatrix, Matrix );
}

bool CSharedEffect::SetWorldViewProjectionMatrix( const Math::Mat44f& Matrix )
{
   return S_OK == SET_MATRIX_PARAMETER( WVPMatrix, Matrix );
}

bool CSharedEffect::SetViewProjectionMatrix( const Math::Mat44f& Matrix )
{
   return S_OK == SET_MATRIX_PARAMETER( VPMatrix, Matrix );
}

bool CSharedEffect::SetAmbientLightColor( const Math::Vect3f &aAmbienLightColor )
{
    float lAmbientColor[3] = {aAmbienLightColor.x, aAmbienLightColor.y, aAmbienLightColor.z};

    HRESULT lRes = m_Effect->SetFloatArray( m_AmbientLight, lAmbientColor, 3 );
    ASSERT( lRes == S_OK, "Error setting ambient color");

    return lRes == S_OK;
}