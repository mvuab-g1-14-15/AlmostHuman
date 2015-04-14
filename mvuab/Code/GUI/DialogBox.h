//----------------------------------------------------------------------------------
// CDialogBox class
// Author: Enric Vergara
//
// Description:
// Dialog boxes may be defined to help the user navigate complicated choices or to give the user information. The second
// example below implements several dialog boxes. Possible dialog boxes include:
//	* error
//	* help
//	* input
//	* question
//	* file read/write
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_DIALOG_BOX_H
#define INC_DIALOG_BOX_H


#include "gui/GuiElement.h"
#include "Button.h"
#include "Math/Color.h"

//---Forward Declarations---
class CTexture;
//--------------------------
class CDialogBox: public CGuiElement
{

public:
	CDialogBox(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
							const Math::Vect2f position_percent,  float buttonWidthPercent, float buttonHeightPercent,
							std::string lit="", uint32 textHeightOffset=0, uint32 textWidthOffset=0, 
							bool isVisible = true, bool isActive = true);
	
	virtual ~CDialogBox() {/*NOTHING*/;}

	//---------------CGuiElement Interface----------------------
	virtual void	Render									();
	virtual void	Update									();
	virtual void	OnClickedChild					(const std::string& name);
	virtual void	SetPosition							(const Math::Vect2i& pos);

	//---------------CDialogBox Interface----------------------
	void					SetCloseButtonTextures	(CTexture* normal, CTexture* over, CTexture* clicked, CTexture* deactivated);
	void					SetMoveButtonTextures		(CTexture* normal, CTexture* over, CTexture* clicked, CTexture* deactivated);
	void					SetDialogTexture				(CTexture* background);	
	void					SetCloseButtonColors		(const Math::CColor& normal, const Math::CColor& over, const Math::CColor& clicked, const Math::CColor& deactivated);
	void					SetMoveButtonColors			(const Math::CColor& normal, const Math::CColor& over, const Math::CColor& clicked, const Math::CColor& deactivated);
	void					SetDialogColors					(const Math::CColor& background);	

private:
	bool			m_bDialogClicked;
	CButton		m_ButtonClose;
	CButton		m_ButtonMove;
	CTexture*	m_pBackGroundTexture;
	Math::CColor		m_BackGroundColor;
	bool			m_bStart_to_Move;
	Math::Vect2i		m_PreviousPosMouse;
	
protected:
	float			m_fButtonHeight;	// Es el % del mHeight
	float			m_fButtonWidth;		// Es el % del mWidth;

};

#endif //INC_DIALOG_BOX_H