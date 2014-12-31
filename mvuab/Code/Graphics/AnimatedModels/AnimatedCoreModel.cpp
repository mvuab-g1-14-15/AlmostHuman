#include "AnimatedCoreModel.h"
#include "cal3d\cal3d.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"

#include "Texture\Texture.h"
#include "Texture\TextureManager.h"


CAnimatedCoreModel::CAnimatedCoreModel(const std::string &Name) : 
        CName(),
        m_Path(""),
        m_FileName(""),
        m_RenderableVertexs(0),
        m_CalCoreModel(new CalCoreModel(Name)),
        m_CalHardwareModel(new CalHardwareModel(m_CalCoreModel))
{
}

CAnimatedCoreModel::~CAnimatedCoreModel()
{
    Destroy();
}

bool CAnimatedCoreModel::LoadMesh(const std::string &Filename)
{
    assert(m_CalCoreModel);
    std::string &MeshFullPath = m_Path + Filename;
    uint16 err = m_CalCoreModel->loadCoreMesh( MeshFullPath );
    return ( err == -1 ) ? false : true;
}

bool CAnimatedCoreModel::LoadSkeleton(const std::string &Filename)
{
    assert(m_CalCoreModel);
    std::string &SkeletonFullPath = m_Path + Filename;
    return m_CalCoreModel->loadCoreSkeleton(SkeletonFullPath);
}
bool CAnimatedCoreModel::LoadAnimation(const std::string &Name, const std::string &Filename)
{
    assert( m_CalCoreModel != NULL );
    std::string &AnimationFullPath = m_Path + Filename;
    uint16 id = m_CalCoreModel->loadCoreAnimation(AnimationFullPath, Name);
    if( id == -1 )
        return false;

    m_AnimationsMap[Name] = id;
    return true;
}

bool CAnimatedCoreModel::LoadTexture(const std::string &Filename)
{
    // Get the texture from the texture manager
    CTexture *t = CTextureManager::GetSingletonPtr()->GetTexture(Filename);
    if(t)
    {
        m_TextureVector.push_back(t);
        return true;
    }

    return false;
}

const std::string & CAnimatedCoreModel::GetTextureName( size_t id )
{
    return m_TextureVector[id]->GetFileName();
}

size_t CAnimatedCoreModel::GetNumTextures() 
{
    return m_TextureVector.size();
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
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CAnimatedCoreModel::Load Tag \"%s\" no existe",  "static_meshes");
        return false;
    }

    // Parse the animation stuff
    for(int i = 0; i < node.GetNumChildren(); ++i)
    {
        const std::string &TagName = node(i).GetName();

        if( TagName == "texture" )
        {
            const std::string &textureFilename = node(i).GetPszProperty("file", "no_file");
            if(!LoadTexture(textureFilename))
            {
                CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CAnimatedCoreModel::LoadTexture No se puede abrir \"%s\"!", m_FileName.c_str());
            }
        }
        else if( TagName == "skeleton" )
        {
            const std::string &skeletonFilename = node(i).GetPszProperty("file", "no_file");
            if(!LoadSkeleton(skeletonFilename))
            {
                CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CAnimatedCoreModel::LoadSkeleton No se puede abrir \"%s\"!", m_FileName.c_str());
            }
        }
        else if( TagName == "mesh" )
        {
            const std::string &meshFilename = node(i).GetPszProperty("file", "no_file");
            if(!LoadMesh(meshFilename))
            {
                CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CAnimatedCoreModel::LoadMesh No se puede abrir \"%s\"!", m_FileName.c_str());
            }
        }
        else if( TagName == "animation" )
        {
            const std::string &animationFilename = node(i).GetPszProperty("file", "no_file");
            const std::string &name = node(i).GetPszProperty("name", "no_name");
            if(!LoadAnimation(name, animationFilename))
            {
                CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CAnimatedCoreModel::LoadAnimation No se puede abrir \"%s\"!", m_FileName.c_str());
            }
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

CalHardwareModel* CAnimatedCoreModel::GetCalHardwareModel()
{
    return m_CalHardwareModel;
}

CRenderableVertexs* CAnimatedCoreModel::GetRenderableVertexs()
{
    return m_RenderableVertexs;
}

bool CAnimatedCoreModel::Reload()
{
    Destroy();
    return Load();
}

void CAnimatedCoreModel::Destroy()
{
    m_TextureVector.clear();
    m_AnimationsMap.clear();

    CHECKED_DELETE(m_CalCoreModel);
    CHECKED_DELETE(m_CalHardwareModel);
    CHECKED_DELETE(m_RenderableVertexs);
}

void CAnimatedCoreModel::ActivateTextures()
{
	TTextureVector::iterator itb = m_TextureVector.begin(),
							 ite = m_TextureVector.end();
	for( size_t i = 0; itb != ite ; ++itb, ++i )
	{
		(*itb)->Activate(i);
	}
}
