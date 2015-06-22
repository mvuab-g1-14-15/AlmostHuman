#include "SceneRenderComands\LensOfFlareRendererCommand.h"
#include "GraphicsManager.h"
#include "Lights\LightManager.h"
#include "Lights\Light.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "Effects\EffectTechnique.h"
#include "RenderableVertex\VertexTypes.h"
#include "Math\Vector2.h"
#include "Math\Color.h"
#include "Effects\Effect.h"
#include "Cameras\CameraManager.h"
#include "Cameras\Camera.h"

#include "EngineManagers.h"

CLensOfFlareRendererCommand::CLensOfFlareRendererCommand( CXMLTreeNode& atts ) :
    CDrawQuadRendererCommand( atts )
{
    CRenderableObjectTechniqueManager *lROTM = ROTMInstance;
    std::string l_TechniqueName = lROTM->GetRenderableObjectTechniqueNameByVertexType(
                                      SCREEN_COLOR_VERTEX::GetVertexType() );
    m_RenderableObjectTechnique = lROTM->GetResource( l_TechniqueName );
}

void CLensOfFlareRendererCommand::Execute( CGraphicsManager& GM )
{
    // Activate the textures
    ActivateTextures();

    // Obtain the first light from the manager, this will always be the sun
    CLight *Sun = GetSun();

    // Obtain the number of flares to be drawn
    int l_NumOfFlares = m_StageTextures.size();

    if ( !Sun || l_NumOfFlares == 0 )
        return;

    // Obtain the technique to draw the effect
    CEffectTechnique *l_EffectTech =  m_RenderableObjectTechnique->GetEffectTechnique();

    // Obtain the sun position in the z near plane of the camera ( make a projection )
    Math::Vect2f l_SunInScreen = GM.ToScreenCoordinates( Sun->GetPosition() );

    // Obtain the center of the screen and make a little displacement.
    uint32 width, height;
    GM.GetWidthAndHeight( width, height );
    Math::Vect2f l_ScreenMiddlePoint( float32( ( width ) / 2 ) + 100 ,
                                      float32( ( height ) / 2 )  + 100 );

    // Obtain a vector from the middle point to the sun
    Math::Vect2f l_SunToCamera = ( l_ScreenMiddlePoint - l_SunInScreen );

    float32 l_Distance = l_SunToCamera.Length();

    // Divide the space between the sun to the camera with the number of flares
    float32 l_SpaceOfFlares =  l_Distance / l_NumOfFlares;

    if ( l_Distance < 500 )
        l_NumOfFlares -= 1;

    if ( l_Distance < 300 )
        l_NumOfFlares -= 1;

    if ( l_Distance < 150 )
        l_NumOfFlares -= 1;

    if ( l_Distance < 100 )
        l_NumOfFlares = 1;

    if ( l_NumOfFlares < 1 )
        l_NumOfFlares = 1;

    // Now get a normalized vector, because the length is calculated in the previous step
    l_SunToCamera = l_SunToCamera.GetNormalized();

    // Iterate over all the flares and draw them in the calculated position
    for ( int i = 0; i < l_NumOfFlares; ++i )
    {
        //Calculate the percentage of this flare and get the wanted displacement
        float32 l_Percentage =  i * l_SpaceOfFlares;
        Math::Vect2f l_Displacement = l_SunToCamera * l_Percentage;
        Math::Vect2f l_FlarePoint = l_SunInScreen + l_Displacement;

        // The biggest must be the sun
        float32 l_FlareSize = ( i == 0 ) ? 150.0f :  100.0f  - ( i * 20.0f ) ;

        if ( l_FlareSize < 1 )
            continue;

        // Obtain the rect in screen space of the flare
        long left     = ( long )( l_FlarePoint.x - l_FlareSize );
        long top      = ( long )( l_FlarePoint.y - l_FlareSize );
        long bottom   = ( long )( l_FlarePoint.y  + l_FlareSize );
        long right    = ( long )( l_FlarePoint.x + l_FlareSize );
        RECT l_Rect = { left, top, right, bottom };

        // Modify the color of the flares with the light color
        m_Color = Sun->GetColor();

        if ( l_EffectTech )
        {
            GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique( l_EffectTech, l_Rect, m_Color,
                    m_StageTextures[i].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
        }
        else
        {
            GM.DrawColoredQuad2DTexturedInPixels( l_Rect, m_Color,
                                                  m_StageTextures[i].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
        }
    }
}

CLight *CLensOfFlareRendererCommand::GetSun()
{
    CLight *l_Sun = LightMInstance->GetLight( 0 );
    CCamera *l_CurrentCamera = CameraMInstance->GetCurrentCamera();

    Math::Vect3f l_SunPosition = l_Sun->GetPosition();

    if ( l_CurrentCamera->GetFrustum().SphereVisible( D3DXVECTOR3( l_SunPosition.x, l_SunPosition.y,
            l_SunPosition.z ), l_Sun->GetStartRangeAttenuation() ) )
        return  l_Sun;

    return 0;
}