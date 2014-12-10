#ifndef ANIMATED_CORE_MODEL_H
#define ANIMATED_CORE_MODEL_H
#pragma once

#include <string>
#include <vector>

//class CalCoreModel;

class CAnimatedCoreModel
{
private:
    //CalCoreModel* m_CalCoreModel;
    std::string m_Name;
    std::vector<std::string> m_TextureFilenameList;
    std::string m_Path;
    bool LoadMesh(const std::string &Filename);
    bool LoadSkeleton(const std::string &Filename);
    bool LoadAnimation(const std::string &Name, const std::string &Filename);
public:
    CAnimatedCoreModel();
    ~CAnimatedCoreModel();
    //CalCoreModel *GetCoreModel( );
    const std::string & GetTextureName( size_t id );
    size_t GetNumTextures( ) const;
    void Load(const std::string &Path);
    int GetAnimationId(const std::string &AnimationName) const;
};

#endif