//----------------------------------------------------------------------------------
// CSlider class
// Author: Enric Vergara
//
// Description:
// A Image..
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_MAP_H
#define INC_MAP_H

#include "Widgets\Image.h"

//---Forward Declarations---
//--------------------------

class CMap: public CImage
{
private:


public:
	CMap( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
					const Math::Vect2f position_percent, std::string lit="", uint32 textHeightOffset=0, uint32 textWidthOffset=0, 
					bool isVisible = true, bool isActive = true);

	virtual ~CMap();

	//---------------CGuiElement Interface----------------------
	virtual void	Render									();
	virtual void	Update									();
	


private:
	CImage*		m_Character;
};

#endif //INC_IMAGE_H