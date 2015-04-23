#include "SceneRenderComands\DeveloperInfoSceneRendererCommand.h"

#include "GraphicsManager.h"
#include "Fonts\FontManager.h"

#include "Timer\Timer.h"
#include "Memory\Allocator.h"
#include "Memory\LinearAllocator.h"
#include "Memory\AllocatorManager.h"
#include "Memory\FreeListAllocator.h"

#include "Cameras\Camera.h"
#include "Cameras\CameraManager.h"

#include "Utils\GPUStatics.h"
#include "EngineManagers.h"

CDeveloperInfoSceneRenderCommand::CDeveloperInfoSceneRenderCommand( CXMLTreeNode& atts )
    : CSceneRendererCommand( atts )
    , m_Quad2dColor( 0.2f, 0.2f, 0.2f, 0.7f )
    , m_Quad2dEdgeColor( Math::colWHITE )
{
}

void CDeveloperInfoSceneRenderCommand::Execute( CGraphicsManager& GM )
{
    CFontManager* FM = FontInstance;
    int32 FontId = FM->GetTTF_Id( "Consolas" );
    uint32 width, height;
    GM.GetWidthAndHeight( width, height );

    CCamera* lCurrentCamera = CameraMInstance->GetCurrentCamera();
    Math::Vect3f lPosition = lCurrentCamera->GetPosition();
    Math::Vect3f lTarget = lCurrentCamera->GetLookAt();

    CAllocatorManager *l_AllocatorManager = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    unsigned int l_MemUsage = l_AllocatorManager->m_pFreeListAllocator->GetUsedMemory();
    l_MemUsage += l_AllocatorManager->m_pLinearAllocator->GetUsedMemory();
    std::string l_Ext = "B";

    if(l_MemUsage >= 1024)
    {
        l_MemUsage /= 1024;
        l_Ext = "KB";
    }

    if(l_MemUsage >= 1024)
    {
        l_MemUsage /= 1024;
        l_Ext = "MB";
    }

    if(l_MemUsage >= 1024)
    {
        l_MemUsage /= 1024;
        l_Ext = "GB";
    }

    int32 lFontHeight = FM->SizeY( "FPS", FontId );
    CGPUStatics* GPU = CGPUStatics::GetSingletonPtr();

    GM.DrawRectangle2D( Math::Vect2i( 5, 7 ), width - 10, lFontHeight + 5, m_Quad2dColor, 2, 2, m_Quad2dEdgeColor );
    FM->DrawTextA( 8, 10, Math::colWHITE, FontId,
                   "Stats: FPS <%5.2f> Cam<%5.2f %5.2f %5.2f> Look<%5.2f %5.2f %5.2f> Objs<%d> Vtxs<%d> Tri<%d> Faces<%d> Mem<%d %s>",
                   FPS, lPosition.x, lPosition.y, lPosition.z, lTarget.x, lTarget.y, lTarget.z, GPU->GetDrawCount(),
                   GPU->GetVertexCount(), uint32( GPU->GetVertexCount() / 3 ), GPU->GetFacesCount(),
                   l_MemUsage, l_Ext.c_str());

    GPU->SetToZero();
}