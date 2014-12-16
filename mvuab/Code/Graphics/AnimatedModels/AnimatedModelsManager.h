#ifndef ANIMATED_MODEL_MANAGER_H
#define ANIMATED_MODEL_MANAGER_H
#pragma once

#include "Utils\MapManager.h"
#include "Utils\SingletonPattern.h"

class CAnimatedInstanceModel;
class CAnimatedCoreModel;


class CAnimatedModelsManager : public CMapManager<CAnimatedCoreModel>, public CSingleton<CAnimatedModelsManager>
{
public:
    CAnimatedModelsManager();
    ~CAnimatedModelsManager();
    CAnimatedCoreModel * GetCore(const std::string &Name, const std::string &Path);
    CAnimatedCoreModel * GetCore(const std::string &Name);
    CAnimatedInstanceModel * GetInstance(const std::string &Name);
    void Load(const std::string &Filename);
protected:
    std::string m_FileName;
    CAnimatedCoreModel *AddNewCore( const std::string &Name, const std::string &Path );
};

#endif //ANIMATED_MODEL_MANAGER_H