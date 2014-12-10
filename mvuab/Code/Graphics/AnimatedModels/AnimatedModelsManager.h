#ifndef ANIMATED_MODEL_MANAGER_H
#define ANIMATED_MODEL_MANAGER_H
#pragma once

#include "Utils\MapManager.h"

class CAnimatedInstanceModel;
class CAnimatedCoreModel;

class CAnimatedModelManager : public CMapManager<CAnimatedCoreModel>
{
public:
	CAnimatedModelManager();
	~CAnimatedModelManager();
	CAnimatedCoreModel * GetCore(const std::string &Name, const std::string &Path);
	CAnimatedInstanceModel * GetInstance(const std::string &Name);
	void Load(const std::string &Filename);
protected:
	std::string m_FileName;
};

#endif //ANIMATED_MODEL_MANAGER_H