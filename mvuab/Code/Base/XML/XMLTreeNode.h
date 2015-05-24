#pragma once

#ifndef _XML_TREE_NODE_H_
#define _XML_TREE_NODE_H_

#include <string>
#include "Math/Vector4.h"
#include "Math/Color.h"

#include "pugixml\pugixml.hpp"

class CXMLTreeNode
{
    public:
        CXMLTreeNode( const pugi::xml_node& aPugiNode );
        virtual ~CXMLTreeNode();

        const char*                 GetName() const;
        bool                        IsOk() const;
        template< typename T > bool GetAttribute( const char* aAttName, T& aValue) const;
        template< typename T > T    GetAttribute( const char* aAttName ) const;

        // Iterators of the tree node
        CXMLTreeNode FirstChild();
        CXMLTreeNode NextChild();
        CXMLTreeNode LastChild();
        bool         More();

    private:
        pugi::xml_node      mTreeNode;
};
#endif