#ifndef MANAGER_H
#define MANAGER_H
#pragma once

#include <string>
#include "XML/XMLTreeNode.h"

class CManager
{
    protected:
        std::string mConfigPath;

    public:
        CManager() {}
        CManager(const CXMLTreeNode &atts) : mConfigPath(atts.GetAttribute<std::string>("file_config", "")) {}
        virtual ~CManager() {}

        void SetConfigPath( const std::string& aNewPath )
        {
            mConfigPath = aNewPath;
        }

        virtual void Init() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;
};

#endif
