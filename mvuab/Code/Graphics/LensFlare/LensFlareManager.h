#ifndef LENS_FLARE_MANAGER_H
#define LENS_FLARE_MANAGER_H
#pragma once

#include "Utils/TemplatedVectorMapManager.h"
#include "Utils/Manager.h"

class CLensFlare;

class CLensFlareManager: public CTemplatedVectorMapManager<CLensFlare>, public CManager
{
    public:
        CLensFlareManager() {}
        CLensFlareManager(const CXMLTreeNode &atts);
        virtual ~CLensFlareManager();

        virtual void Init();
        virtual void Update();
        virtual void Render();
};

#endif // LENS_FLARE_MANAGER_H