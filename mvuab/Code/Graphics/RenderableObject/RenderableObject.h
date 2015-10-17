#pragma once
#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include "Object3D.h"
#include "Utils\Name.h"

class CXMLTreeNode;

class CRenderableObject : public CObject3D, public CName
{
    public:
        CRenderableObject();
        CRenderableObject( const CXMLTreeNode &atts);
        virtual ~CRenderableObject();

        GET_SET(bool, Active);

        virtual void Update() {}
        virtual void Render() = 0;

        std::string GetRoomName();
        void SetRoomName( std::string );

    private:
        bool m_Active;

        std::string m_RoomName;
};

#endif