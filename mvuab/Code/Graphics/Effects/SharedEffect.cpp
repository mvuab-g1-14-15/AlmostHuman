#include "SharedEffect.h"
#include "EffectManager.h"
#include "Defines.h"
#include "Logger\Logger.h"
#include "EngineManagers.h"
#include "Lights\LightManager.h"
#include "Timer\Timer.h"
#include "GraphicsManager.h"

namespace {
bool BindMatrices( CSharedEffect* aSharedEffect, CEffectManager* aEffectManager )
{
    bool lOk = true;

    // Obtain the matrices only once then multiply or invert depending on the case
    const Math::Mat44f lProjectionMtx = aEffectManager->GetProjectionMatrix();
    const Math::Mat44f lViewMtx = aEffectManager->GetViewMatrix();

    // Then set the parameters
    lOk = lOk && aSharedEffect->SetProjectionMatrix( lProjectionMtx );
    lOk = lOk && aSharedEffect->SetViewMatrix( lViewMtx );
    lOk = lOk && aSharedEffect->SetViewProjectionMatrix( lProjectionMtx * lViewMtx);
    lOk = lOk && aSharedEffect->SetInverseProjectionMatrix( lProjectionMtx );
    lOk = lOk && aSharedEffect->SetInverseViewMatrix( lViewMtx );

    return lOk;
}
}

CSharedEffect::CSharedEffect( )
    : CEffect( "SharedEffect" )
    , CTOR_EFFECT_PARAMETER( ViewMatrix )
    , CTOR_EFFECT_PARAMETER( ProjectionMatrix )
    , CTOR_EFFECT_PARAMETER( VPMatrix )
    , CTOR_EFFECT_PARAMETER( InverseViewMatrix )
    , CTOR_EFFECT_PARAMETER( InverseProjectionMatrix )
    , CTOR_EFFECT_PARAMETER( CameraPosition )
    , CTOR_EFFECT_PARAMETER( AmbientLight )
    , CTOR_EFFECT_PARAMETER( FBWidth )
    , CTOR_EFFECT_PARAMETER( FBHeight )
    , CTOR_EFFECT_PARAMETER( DeltaTime )
{
}

CSharedEffect::~CSharedEffect()
{
}

void CSharedEffect::Bind()
{
    if( !BindMatrices( this, EffectManagerInstance ) )
    {
        LOG_ERROR_APPLICATION( "Error binding the matrices to the shared effect!!" );
    }

    if( !SetCameraPosition( EffectManagerInstance->GetCameraEye() ) )
    {
        LOG_ERROR_APPLICATION( "Error binding the camera to the shared effect!!" );
    }

    if( !SetAmbientLightColor( LightMInstance->GetAmbientLight() ) )
    {
        LOG_ERROR_APPLICATION( "Error binding the ambient light to the shared effect!!" );
    }

    if( !SetDeltaTime( deltaTimeMacro ) )
    {
        LOG_ERROR_APPLICATION( "Error binding the delta time to the shared effect!!" );
    }

    Math::Vect2u lFBSize;
    GraphicsInstance->GetWidthAndHeight(lFBSize.x, lFBSize.y);
    if( !SetFBSize( lFBSize ) )
    {
        LOG_ERROR_APPLICATION( "Error binding the fb size to the shared effect!!" );
    }
}

void CSharedEffect::SetNullParameters()
{
    RESET_EFFECT_PARAMETER( VPMatrix );

    RESET_EFFECT_PARAMETER( ViewMatrix );
    RESET_EFFECT_PARAMETER( ProjectionMatrix );

    RESET_EFFECT_PARAMETER( InverseViewMatrix );
    RESET_EFFECT_PARAMETER( InverseProjectionMatrix );

    RESET_EFFECT_PARAMETER( CameraPosition );
    RESET_EFFECT_PARAMETER( AmbientLight );

    RESET_EFFECT_PARAMETER( FBWidth );
    RESET_EFFECT_PARAMETER( FBHeight );
    RESET_EFFECT_PARAMETER( DeltaTime );
}

void CSharedEffect::LinkSemantics()
{
    LINK_EFFECT_PARAMETER( VPMatrix );

    LINK_EFFECT_PARAMETER( ViewMatrix );
    LINK_EFFECT_PARAMETER( ProjectionMatrix );

    LINK_EFFECT_PARAMETER( InverseViewMatrix );
    LINK_EFFECT_PARAMETER( InverseProjectionMatrix );

    LINK_EFFECT_PARAMETER( CameraPosition );
    LINK_EFFECT_PARAMETER( AmbientLight );

    LINK_EFFECT_PARAMETER( FBWidth );
    LINK_EFFECT_PARAMETER( FBHeight );
    LINK_EFFECT_PARAMETER( DeltaTime );
}

bool CSharedEffect::SetCameraPosition( const Math::Vect3f &CameraPosition )
{
    float32 l_Camera[3];
    l_Camera[0] = CameraPosition.x;
    l_Camera[1] = CameraPosition.y;
    l_Camera[2] = CameraPosition.z;
    return ( m_Effect->SetFloatArray( m_CameraPosition, l_Camera, 3 ) == S_OK );
}

bool CSharedEffect::SetViewMatrix( const Math::Mat44f& Matrix )
{
    return S_OK == SET_MATRIX_PARAMETER( ViewMatrix, Matrix );
}

bool CSharedEffect::SetProjectionMatrix( const Math::Mat44f& Matrix )
{
    return S_OK == SET_MATRIX_PARAMETER( ProjectionMatrix, Matrix );
}

bool CSharedEffect::SetInverseViewMatrix( const Math::Mat44f& Matrix )
{
    return ( m_Effect->SetMatrix( m_InverseViewMatrix, &Matrix.GetInverted().GetD3DXMatrix() ) == S_OK );
}

bool CSharedEffect::SetInverseProjectionMatrix( const Math::Mat44f& Matrix )
{
    return ( m_Effect->SetMatrix( m_InverseProjectionMatrix, &Matrix.GetInverted().GetD3DXMatrix() ) == S_OK );
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

bool CSharedEffect::SetFBSize( const Math::Vect2u aSize )
{
    return ( S_OK == SET_INT_PARAMETER( FBWidth, aSize.x ) ) && ( S_OK == SET_INT_PARAMETER( FBHeight, aSize.y ) );
}

bool CSharedEffect::SetDeltaTime( const float dt )
{
    return S_OK == SET_FLOAT_PARAMETER( DeltaTime, dt );
}