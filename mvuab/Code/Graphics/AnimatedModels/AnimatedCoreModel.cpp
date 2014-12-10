#include "AnimatedCoreModel.h"
#include "cal3d\cal3d.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"


CAnimatedCoreModel::CAnimatedCoreModel() : m_Path(""), m_CalCoreModel(0), CName()
{
}

CAnimatedCoreModel::~CAnimatedCoreModel()
{
}

bool CAnimatedCoreModel::LoadMesh(const std::string &Filename)
{
    std::string MeshFullPath = m_Path + Filename;
    return true;
}

bool CAnimatedCoreModel::LoadSkeleton(const std::string &Filename)
{
    std::string SkeletonFullPath = m_Path + Filename;
    return true;
}
bool CAnimatedCoreModel::LoadAnimation(const std::string &Name, const std::string &Filename)
{
    std::string AnimationFullPath = m_Path + Filename;
    return true;
}

const std::string & CAnimatedCoreModel::GetTextureName( size_t id )
{
    std::string textureName = "ANDREI";
    //TODO:Look inside the texture
    return textureName;
}

size_t CAnimatedCoreModel::GetNumTextures( ) const 
{
    return m_TextureFilenameList.size();
}

bool CAnimatedCoreModel::Load(const std::string &Path)
{
    m_Path = Path;
	
	std::string filename = m_Path + defaultXML;
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(filename.c_str()))
	{
		CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CAnimatedCoreModel::Load No se puede abrir \"%s\"!", filename.c_str());
		return false;
	}

	CXMLTreeNode node = newFile["animated_model"];
	if(!node.Exists())
	{
		CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CStaticMeshManager::Load Tag \"%s\" no existe",  "static_meshes");
		return false;
	}

	const std::string &name = node.GetPszProperty("name", "no_name");
	SetName(name);

	for(int i = 0; i < node.GetNumChildren(); ++i)
	{
		const std::string &TagName = node(i).GetName();

		if( TagName == "texture" )
		{
			const std::string &textureFilename = node.GetPszProperty("file", "no_file");
		}
		else if( TagName == "skeleton" )
		{
			const std::string &skeletonFilename = node.GetPszProperty("file", "no_file");
			LoadSkeleton(skeletonFilename);
		}
		else if( TagName == "mesh" )
		{
			const std::string &meshFilename = node.GetPszProperty("file", "no_file");
			LoadMesh(meshFilename);
		}
		else if( TagName == "animation" )
		{
			const std::string &animationFilename = node.GetPszProperty("file", "no_file");
			const std::string &name = node.GetPszProperty("name", "no_name");
			LoadAnimation(name, animationFilename);
		}
	}
	
	return true;

}

int CAnimatedCoreModel::GetAnimationId(const std::string &AnimationName) const
{
    return 0;
}

CalCoreModel *CAnimatedCoreModel::GetCoreModel( )
{
	return m_CalCoreModel;
}