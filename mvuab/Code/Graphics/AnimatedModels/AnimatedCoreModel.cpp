#include "AnimatedCoreModel.h"

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

CAnimatedCoreModel::CAnimatedCoreModel() : m_Path(""), m_Name("")
{

}

CAnimatedCoreModel::~CAnimatedCoreModel()
{
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

void CAnimatedCoreModel::Load(const std::string &Path)
{
    m_Path = Path;
}

int CAnimatedCoreModel::GetAnimationId(const std::string &AnimationName) const
{
    return 0;
}