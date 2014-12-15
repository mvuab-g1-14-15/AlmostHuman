#include "AnimatedCoreModel.h"
#include "cal3d\cal3d.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "Texture\Texture.h"
#include "Texture\TextureManager.h"


CAnimatedCoreModel::CAnimatedCoreModel(const std::string &Name) : 
        m_Path(""),
        m_FileName(""),
        m_CalCoreModel( new CalCoreModel(Name)),
        CName()
{
}

CAnimatedCoreModel::~CAnimatedCoreModel()
{
    Destroy();
}

bool CAnimatedCoreModel::LoadMesh(const std::string &Filename)
{
    assert(m_CalCoreModel);
    uint16 err = m_CalCoreModel->loadCoreMesh( Filename );
    return ( err == -1 ) ? false : true;
}

bool CAnimatedCoreModel::LoadSkeleton(const std::string &Filename)
{
    assert(m_CalCoreModel);
    std::string SkeletonFullPath = m_Path + Filename;
    return m_CalCoreModel->loadCoreSkeleton(SkeletonFullPath);
}
bool CAnimatedCoreModel::LoadAnimation(const std::string &Name, const std::string &Filename)
{
    assert( m_CalCoreModel != NULL );
    std::string AnimationFullPath = m_Path + Filename;
    uint16 id = m_CalCoreModel->loadCoreAnimation(AnimationFullPath, Name);
    if( id == -1 )
        return false;

    m_AnimationsMap[Name] = id;
    return true;
}

const std::string & CAnimatedCoreModel::GetTextureName( size_t id )
{
    return m_TextureFilenameVector[id];
}

size_t CAnimatedCoreModel::GetNumTextures( ) const 
{
    return m_TextureFilenameVector.size();
}

bool CAnimatedCoreModel::Load(const std::string &Path)
{
    m_Path = Path;
    m_FileName = m_Path + defaultXML;
    return Load();
}

bool CAnimatedCoreModel::Load()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_FileName.c_str()))
	{
		CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CAnimatedCoreModel::Load No se puede abrir \"%s\"!", m_FileName.c_str());
		return false;
	}

	CXMLTreeNode node = newFile["animated_model"];
	if(!node.Exists())
	{
		CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CStaticMeshManager::Load Tag \"%s\" no existe",  "static_meshes");
		return false;
	}

    // Parse the animation stuff
	for(int i = 0; i < node.GetNumChildren(); ++i)
	{
		const std::string &TagName = node(i).GetName();

		if( TagName == "texture" )
		{
			const std::string &textureFilename = node.GetPszProperty("file", "no_file");
            m_TextureFilenameVector.push_back( textureFilename );
		}
		else if( TagName == "skeleton" )
		{
			const std::string &skeletonFilename = node.GetPszProperty("file", "no_file");
			if(!LoadSkeleton(skeletonFilename))
      {
          CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CAnimatedCoreModel::LoadSkeleton No se puede abrir \"%s\"!", m_FileName.c_str());
      }
		}
		else if( TagName == "mesh" )
		{
			const std::string &meshFilename = node.GetPszProperty("file", "no_file");
			if(!LoadMesh(meshFilename))
      {
          CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CAnimatedCoreModel::LoadMesh No se puede abrir \"%s\"!", m_FileName.c_str());
      }
		}
		else if( TagName == "animation" )
		{
			const std::string &animationFilename = node.GetPszProperty("file", "no_file");
			const std::string &name = node.GetPszProperty("name", "no_name");
      if(!LoadAnimation(name, animationFilename))
      {
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CAnimatedCoreModel::LoadAnimation No se puede abrir \"%s\"!", m_FileName.c_str());
      }
		}
	}

    // Load all the textures
    LoadTextures();

    // make one material thread for each material
    // NOTE: this is not the right way to do it, but this viewer can't do the right
    // mapping without further information on the model etc., so this is the only
    // thing we can do here.
    for( uint32 materialId = 0; materialId < m_CalCoreModel->getCoreMaterialCount(); ++materialId)
    {
        // create the a material thread
        m_CalCoreModel->createCoreMaterialThread(materialId);

        // initialize the material thread
        m_CalCoreModel->setCoreMaterialId(materialId, 0, materialId);
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

bool CAnimatedCoreModel::Reload()
{
    Destroy();
    return Load();
}

void CAnimatedCoreModel::Destroy()
{
    m_TextureFilenameVector.clear();
    m_AnimationsMap.clear();
    CHECKED_DELETE(m_CalCoreModel);
}

void CAnimatedCoreModel::LoadTextures()
{
    size_t l_TexCount = m_TextureFilenameVector.size();
    for(size_t i = 0; i < l_TexCount; ++i)
    {
        std::string l_Path = m_TextureFilenameVector[i];
    }
}