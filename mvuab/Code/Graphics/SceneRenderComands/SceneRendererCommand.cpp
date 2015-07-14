#include "SceneRenderComands\SceneRendererCommand.h"

CSceneRendererCommand::CSceneRendererCommand( CXMLTreeNode& atts )
{
    if ( atts.Exists() )
    {
        SetName( atts.GetAttribute<std::string>( "name", atts.GetName() ) );
        SetVisible( atts.GetAttribute<bool>( "active", true ) );
    }
}

CSceneRendererCommand::~CSceneRendererCommand()
{
}