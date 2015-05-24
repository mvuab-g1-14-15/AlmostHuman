#pragma once

#ifndef XML_DOC_H_
#define XML_DOC_H_

#include <string>
#include "pugixml\pugixml.hpp"
#include "XMLTreeNode.h"

class CXMLDocument
{
    public:
        CXMLDocument( const std::string& aFile );
        virtual ~CXMLDocument();
        bool Load();
        CXMLTreeNode GetNode( const std::string aNode ) const;

    private:
        bool                mIsOk;
        std::string         mFile;
        pugi::xml_document  mDocument;
};
#endif