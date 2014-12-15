#ifndef ANIMATED_CORE_MODEL_H
#define ANIMATED_CORE_MODEL_H
#pragma once

#include <string>
#include <vector>
#include <map>
#include "Utils\Name.h"

// Define the default xml file that will be searched inside the path
const std::string defaultXML = "actor.xml";

class CalCoreModel;
class CTexture;

class CAnimatedCoreModel : public CName
{
protected:
    typedef std::vector<CTexture *> TTextureVector;
    CalCoreModel*                   m_CalCoreModel;
    std::vector<std::string>        m_TextureFilenameVector;
    TTextureVector                  m_TextureVector;
    std::map<std::string, uint16>   m_AnimationsMap;
    std::string                     m_Path;
    std::string                     m_FileName;

private:
    void Destroy();
    bool Reload();
    bool Load();
    bool LoadMesh(const std::string &Filename);
    bool LoadSkeleton(const std::string &Filename);
    bool LoadAnimation(const std::string &Name, const std::string &Filename);
    void LoadTextures();

public:
    CAnimatedCoreModel(const std::string &Name);
    ~CAnimatedCoreModel();
    CalCoreModel *GetCoreModel( );
    const std::string & GetTextureName( size_t id );
    size_t GetNumTextures( ) const;
    bool Load(const std::string &Path);
    int GetAnimationId(const std::string &AnimationName) const;
};

#endif