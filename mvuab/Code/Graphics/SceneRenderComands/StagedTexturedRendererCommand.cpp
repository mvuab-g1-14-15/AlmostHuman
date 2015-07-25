#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "Texture\Texture.h"
#include "GraphicsManager.h"

#include "Texture\TextureManager.h"
#include "EngineManagers.h"

#include "Memory\FreeListAllocator.h"
#include "Memory\AllocatorManager.h"
#include "Memory\LinearAllocator.h"

#ifdef _DEBUG
    #include <sstream>
#endif

CStagedTexturedRendererCommand::CStagedTexturedRendererCommand(CXMLTreeNode& atts):
    CSceneRendererCommand(atts)
{
    CGraphicsManager* gm = GraphicsInstance;

    if(atts.Exists())
    {
        int count = atts.GetNumChildren();
        CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

        for(int i = 0; i < count; ++i)
        {
            const CXMLTreeNode& lCurrentNode = atts(i);
            const std::string& TagName = atts(i).GetName();

            if(TagName == "dynamic_texture")
            {
                std::string l_Name = atts(i).GetAttribute<std::string>("name", "");
                int l_StageId = atts(i).GetAttribute<int32>("stage_id", -1);
                bool l_WidthAsFB = atts(i).GetAttribute<bool>("texture_width_as_frame_buffer", false);
                bool l_CreateDepthStencilBuffer = atts(i).GetAttribute<bool>("create_depth_stencil_buffer", true);
                uint32 l_Width, l_Height;

                if(!l_WidthAsFB)
                {
                    l_Width = atts(i).GetAttribute<int32>("width", 0);
                    l_Height = atts(i).GetAttribute<int32>("height", 0);
                }
                else
                    gm->GetWidthAndHeight(l_Width, l_Height);

                std::string l_FormatType = atts(i).GetAttribute<std::string>("format_type", "");
                CTexture::TFormatType l_iFormatType = (l_FormatType == "R32F") ? (CTexture::TFormatType)3 : (CTexture::TFormatType)0;

                CTexture* l_Texture = l_AllocatorManger->m_pFreeListAllocator->MakeNew<CTexture>();
                l_Texture->Create(l_Name, l_Width, l_Height, 0, CTexture::eUsageRenderTarget, CTexture::eDefaultPool, l_iFormatType, l_CreateDepthStencilBuffer);

                if(TextureMInstance->AddResource(l_Name, l_Texture))
                    AddStageTexture(l_StageId, l_Texture, true);
                else
                {
                    AddStageTexture(l_StageId, TextureMInstance->GetResource(l_Name), true);
                    l_AllocatorManger->m_pFreeListAllocator->MakeDelete(l_Texture);
                }
            }

            if(TagName == "texture")
            {
                std::string l_Name = atts(i).GetAttribute<std::string>("name", "");
                int l_StageId = atts(i).GetAttribute<int32>("stage_id", -1);
                CTexture* l_Texture = TextureMInstance->GetTexture(l_Name);

                if(!l_Texture)
                {
                    LOG_ERROR_APPLICATION("CStagedTexturedRendererCommand::Constructor: Error loading Texture \"%s\".", l_Name.c_str());
                    continue;
                }

                AddStageTexture(l_StageId, l_Texture, false);
            }
        }
    }
}

CStagedTexturedRendererCommand::~CStagedTexturedRendererCommand()
{
    //TODO SCALAR DELETING DESTRUCTOR (AL TENER VARIAS REFERENCIAS DE UNA MISMA TEXTURA EN DISTINTOS COMMANDS (UNO BORRA Y EL OTRO LO INTENTA BORRAR DE NUEVO)
    /*  for ( std::vector<CKGStageTexture>::iterator it = m_StageTextures.begin();
          it != m_StageTextures.end(); ++it )
        CHECKED_DELETE( it->m_Texture );

        m_StageTextures.clear();*/
}

void CStagedTexturedRendererCommand::ActivateTextures()
{
    for(size_t i = 0; i < m_StageTextures.size(); ++i)
        m_StageTextures[i].m_Texture->Activate(m_StageTextures[i].m_StageId);

    DebugTextures();
}

void CStagedTexturedRendererCommand::AddStageTexture(int StageId, CTexture* Texture, bool aIsDynamic)
{
    CKGStageTexture StageTexture = CKGStageTexture(StageId, Texture, aIsDynamic);
    m_StageTextures.push_back(StageTexture);
}

void CStagedTexturedRendererCommand::DebugTextures()
{
#ifdef _DEBUG

    if(false)
    {
        for(size_t i = 0; i < m_StageTextures.size(); ++i)
        {
            std::string l_TextureName = "CDrawQuadRendererCommand_" + GetName() + "_";
            std::stringstream l_CompletedTextureName;
            l_CompletedTextureName << l_TextureName << i;
            m_StageTextures[i].m_Texture->Save(l_CompletedTextureName.str());
        }
    }

#endif
}