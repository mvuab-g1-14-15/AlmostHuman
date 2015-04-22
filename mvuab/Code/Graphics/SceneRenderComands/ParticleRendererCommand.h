#ifndef _PARTICLE_RENDER_COMMAND_
#define _PARTICLE_RENDER_COMMAND_

#include "SceneRenderComands\SceneRendererCommand.h"
#include "XML\XMLTreeNode.h"

class CGraphicsManager;

class CParticleRenderCommand : public CSceneRendererCommand
{
    protected:

    public:
        CParticleRenderCommand(CXMLTreeNode& atts);
        virtual void Execute(CGraphicsManager& GM);
};

#endif