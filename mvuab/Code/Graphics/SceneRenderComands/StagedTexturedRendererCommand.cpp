#include "SceneRenderComands\StagedTexturedRendererCommand.h"


CStagedTexturedRendererCommand::CStagedTexturedRendererCommand(CXMLTreeNode &atts):CSceneRendererCommand(atts)
{
    if(atts.Exists())
    {
        int count = atts.GetNumChildren();
        for( int i = 0; i < count; ++i )
        {
            int StageId = atts(i).GetIntProperty("stage_id",0);
            CTexture * Texture = new CTexture();
            Texture->Load(atts(i).GetPszProperty("file",""));
            AddStageTexture(StageId, Texture);
        }
    }
}

CStagedTexturedRendererCommand::~CStagedTexturedRendererCommand()
{
    
}

void CStagedTexturedRendererCommand::ActivateTextures()
{

}

void CStagedTexturedRendererCommand::AddStageTexture(int StageId, CTexture *Texture)
{
    CKGStageTexture StageTexture = CKGStageTexture(StageId, Texture);
    m_StageTextures.push_back(StageTexture);
}