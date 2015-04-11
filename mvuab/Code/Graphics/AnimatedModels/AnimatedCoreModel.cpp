#include "AnimatedCoreModel.h"
#include "cal3d\cal3d.h"

#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"

#include "Texture\Texture.h"
#include "Texture\TextureManager.h"

#include "GraphicsManager.h"
#include "RenderableVertex\VertexTypes.h"
#include "RenderableVertex\IndexedVertexs.h"

#include "Core.h"
#include "EngineManagers.h"
#define MAXBONES 29

CAnimatedCoreModel::CAnimatedCoreModel(const std::string &Name) : 
        CName(),
        m_Path(""),
        m_FileName(""),
        m_CalHardwareModel(0),
        m_RenderableVertexs(0),
        m_CalCoreModel(new CalCoreModel(Name)),
		m_NumFaces(0), 
		m_NumVtxs(0)
{
}

CAnimatedCoreModel::~CAnimatedCoreModel()
{
    Destroy();
}

void CAnimatedCoreModel::Destroy()
{
    m_TextureVector.clear();
    m_AnimationsMap.clear();

    CHECKED_DELETE(m_CalCoreModel);
    CHECKED_DELETE(m_CalHardwareModel);
    CHECKED_DELETE(m_RenderableVertexs);
}

bool CAnimatedCoreModel::LoadMesh(const std::string &Filename)
{
    ASSERT(m_CalCoreModel, "Cal Core Model not found");
    std::string &MeshFullPath = m_Path + Filename;
    int err = m_CalCoreModel->loadCoreMesh( MeshFullPath );
    return ( err == -1 ) ? false : true;
}

bool CAnimatedCoreModel::LoadSkeleton(const std::string &Filename)
{
    ASSERT(m_CalCoreModel, "Cal Core Model not found");
    return m_CalCoreModel->loadCoreSkeleton(m_Path + Filename);
}
bool CAnimatedCoreModel::LoadAnimation(const std::string &Name, const std::string &Filename)
{
    ASSERT(m_CalCoreModel, "Cal Core Model not found");
    int id = m_CalCoreModel->loadCoreAnimation(m_Path + Filename, Name);
    if( id == -1 )
        return false;

    m_AnimationsMap[Name] = id;
    return true;
}

bool CAnimatedCoreModel::LoadVertexBuffer(CGraphicsManager *GM)
{
	m_NumVtxs=0;
	m_NumFaces=0;
	for(int i=0;i<m_CalCoreModel->getCoreMeshCount();++i)
	{
		CalCoreMesh *l_CalCoreMesh=m_CalCoreModel->getCoreMesh(i);
		for(int j=0;j<l_CalCoreMesh->getCoreSubmeshCount();++j)
		{
			CalCoreSubmesh *l_CalCoreSubmesh=l_CalCoreMesh->getCoreSubmesh(j);
			if(l_CalCoreSubmesh!=NULL)
			{
				m_NumVtxs+=l_CalCoreSubmesh->getVertexCount();
				m_NumFaces+=l_CalCoreSubmesh->getFaceCount();
			}
		}
	}

    CHECKED_DELETE(m_CalHardwareModel);
	m_CalHardwareModel = new CalHardwareModel(m_CalCoreModel);
	unsigned short *l_Idxs = new unsigned short[m_NumFaces*3];
	CAL3D_HW_VERTEX *l_Vtxs = new CAL3D_HW_VERTEX[m_NumFaces*3];
    
    m_CalHardwareModel->setVertexBuffer((char*) l_Vtxs, sizeof(CAL3D_HW_VERTEX));
    m_CalHardwareModel->setWeightBuffer(((char*) l_Vtxs) + 12, sizeof(CAL3D_HW_VERTEX));
    m_CalHardwareModel->setMatrixIndexBuffer(((char*) l_Vtxs) + 28, sizeof(CAL3D_HW_VERTEX));

    m_CalHardwareModel->setNormalBuffer(((char*) l_Vtxs) + 44, sizeof(CAL3D_HW_VERTEX));

    m_CalHardwareModel->setTextureCoordNum(1);
    m_CalHardwareModel->setTextureCoordBuffer(0,((char*)l_Vtxs)+92,sizeof(CAL3D_HW_VERTEX));
    
    m_CalHardwareModel->setIndexBuffer(l_Idxs);
    m_CalHardwareModel->load( 0, 0, MAXBONES);
    
    m_NumVtxs = m_CalHardwareModel->getTotalVertexCount();
    m_NumFaces = m_CalHardwareModel->getTotalFaceCount();

    //En caso de utilizar NormalMap
    CalcTangentsAndBinormals(l_Vtxs, (unsigned short *) l_Idxs, m_NumVtxs, m_NumFaces*3, sizeof(CAL3D_HW_VERTEX), 0, 44, 60, 76, 92);
    
    CHECKED_DELETE(m_RenderableVertexs);
    m_RenderableVertexs = new CIndexedVertexs<CAL3D_HW_VERTEX>(GM, l_Vtxs, l_Idxs, m_NumVtxs, m_NumFaces * 3);
    delete []l_Vtxs; 
    delete []l_Idxs;

    return true;
}

bool CAnimatedCoreModel::LoadTexture(const std::string &Filename)
{
    // Get the texture from the texture manager
    CTexture *t = TextureMInstance->GetTexture( m_Path + Filename);
    if(t)
        m_TextureVector.push_back(t);
    return (t != 0);
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
        LOG_ERROR_APPLICATION( "CAnimatedCoreModel::Load No se puede abrir \"%s\"!", m_FileName.c_str());
        return false;
    }

    CXMLTreeNode node = newFile["animated_model"];
    if(!node.Exists())
    {
        LOG_ERROR_APPLICATION( "CAnimatedCoreModel::Load Tag \"%s\" no existe",  "static_meshes");
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
                LOG_ERROR_APPLICATION( "CAnimatedCoreModel::LoadTexture No se puede abrir \"%s\"!", m_FileName.c_str());
            }
        }
        else if( TagName == "skeleton" )
        {
            const std::string &skeletonFilename = node(i).GetPszProperty("file", "no_file");
            if(!LoadSkeleton(skeletonFilename))
            {
                LOG_ERROR_APPLICATION( "CAnimatedCoreModel::LoadSkeleton No se puede abrir \"%s\"!", m_FileName.c_str());
            }
        }
        else if( TagName == "mesh" )
        {
            const std::string &meshFilename = node(i).GetPszProperty("file", "no_file");
            if(!LoadMesh(meshFilename))
            {
                LOG_ERROR_APPLICATION( "CAnimatedCoreModel::LoadMesh No se puede abrir \"%s\"!", m_FileName.c_str());
            }
        }
        else if( TagName == "animation" )
        {
            const std::string &animationFilename = node(i).GetPszProperty("file", "no_file");
            const std::string &name = node(i).GetPszProperty("name", "no_name");
            if(!LoadAnimation(name, animationFilename))
            {
                LOG_ERROR_APPLICATION( "CAnimatedCoreModel::LoadAnimation No se puede abrir \"%s\"!", m_FileName.c_str());
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

void CAnimatedCoreModel::ActivateTextures()
{
	TTextureVector::iterator itb = m_TextureVector.begin(),
							 ite = m_TextureVector.end();
	for( size_t i = 0; itb != ite ; ++itb, ++i )
	{
		(*itb)->Activate(i);
	}
}
