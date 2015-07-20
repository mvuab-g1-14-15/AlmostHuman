#include "Defines.h"
#include "EffectFlags.h"

CEffectFlags::CEffectFlags(const CXMLTreeNode& aFlags)
    : mMatrixFlags( 0 )
    , mLightsFlags( 0 )
    , m_UseFog( false )
    , m_FogEnd( 0.0f )
    , m_FogStart( 0.0f )
    , m_FogFun( eFogExponentianl )
    , m_FogExp( 0.0f )
    , m_UseDebugColor( false )
{
    for ( uint32 j = 0, lCount = aFlags.GetNumChildren(); j < lCount; j++ )
    {
        const CXMLTreeNode& lCurrentFlag = aFlags( j );
        const std::string& l_TagName = lCurrentFlag.GetName();

        if( l_TagName == "matrices" )
        {
            FillMatrixFlags( lCurrentFlag );
        }
        else if( l_TagName == "lights" )
        {
            FillLightsFlags( lCurrentFlag );
        }
        else if( l_TagName == "fog" )
        {
            FillFogFlags( lCurrentFlag );
        }
        else if( l_TagName == "debug" )
        {
            FillDebugFlags( lCurrentFlag );
        }
    }
}

CEffectFlags::~CEffectFlags()
{
}

void CEffectFlags::FillMatrixFlags(const CXMLTreeNode& aFlags )
{
    mMatrixFlags |= ( aFlags.GetAttribute<bool>("use_world_matrix", false ) )                    ? eWorldMatrix : 0;
    mMatrixFlags |= ( aFlags.GetAttribute<bool>("use_world_view_matrix", false ) )               ? eWVMatrix : 0;
    mMatrixFlags |= ( aFlags.GetAttribute<bool>("use_world_view_projection_matrix", false ) )    ? eWVPMatrix : 0;
    mMatrixFlags |= ( aFlags.GetAttribute<bool>("use_inverse_world_matrix", false ) )            ? eInverseWorld : 0;
}

const uint32 CEffectFlags::GetMatrixFlags() const
{
    return mMatrixFlags;
}

void CEffectFlags::FillLightsFlags(const CXMLTreeNode& aFlags )
{
    mLightsFlags |= ( aFlags.GetAttribute<bool>("use_lights", false ) )              ? eUseLights : 0;
    mLightsFlags |= ( aFlags.GetAttribute<bool>("use_view_to_light_projection_matrix", false ) ) ? eViewToLightProjectionMatrix : 0;
    mNumLights    = aFlags.GetAttribute<uint32>("num_of_lights", 0 );
}

void CEffectFlags::FillFogFlags(const CXMLTreeNode& aFlags )
{
    m_UseFog    = aFlags.GetAttribute<bool>( "use_fog", false );
    m_FogStart  = aFlags.GetAttribute<float>( "fog_start", 0 );
    m_FogEnd    = aFlags.GetAttribute<float>( "fog_end", 0 );
    m_FogExp    = aFlags.GetAttribute<float>( "fog_exp", 0 );
    m_FogFun    = ( EFogFunction )aFlags.GetAttribute<int32>( "fog_fun", 1 );
}

void CEffectFlags::FillDebugFlags(const CXMLTreeNode& aFlags )
{
    m_UseDebugColor = aFlags.GetAttribute<bool>( "use_fog", false );
}