#ifndef _ENEMY_H
#define _ENEMY_H

#pragma once

#include "Math\Vector3.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"
#include "Characters\Character.h"

class CEnemy : public CCharacter
{
protected:
    
public:
    CEnemy(CXMLTreeNode &Node);
    virtual ~CEnemy();
    virtual void Update();
	virtual void Render();
};

#endif

