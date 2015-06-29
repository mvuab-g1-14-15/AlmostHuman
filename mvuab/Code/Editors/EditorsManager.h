#ifndef EDITORS_MANAGER_H
#define EDITORS_MANAGER_H
#pragma once

#include "Utils\Defines.h"
#include "Utils\Manager.h"
#include "XML/XMLTreeNode.h"

class CUiBar;

class CEditorsManager : public CManager
{
    public:
        CEditorsManager( const CXMLTreeNode& atts );
        ~CEditorsManager();

        virtual void Init();
        void Update();
        void Render();

    private: // Members
        std::vector<CUiBar*> mBars;
};

#endif // EDITORS_MANAGER_H