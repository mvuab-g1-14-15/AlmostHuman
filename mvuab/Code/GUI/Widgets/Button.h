//----------------------------------------------------------------------------------
// CButton class
// Author: Enric Vergara
//
// Description:
// A button is typically used when you want an immediate action to occur when the user presses the button.
// An example might be a button to quit the application.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_BUTTON_H
#define INC_BUTTON_H

#include <string>
#include "GuiElement\GuiElement.h"
#include "Math/Color.h"

//---Forward Declarations---
class CTexture;
//--------------------------

class CButton: public CGuiElement
{
private:

	typedef enum EButtonState { BS_NORMAL, BS_OVER, BS_CLICKED };

public:

	CButton(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
						const Math::Vect2f position_percent, std::string lit="", uint32 textHeightOffset=0,
						uint32 textWidthOffsetbool=0, bool isVisible = true, bool isActive = true);

	virtual ~CButton() {/*NOTHING*/;}

	//---------------CGuiElement Interface----------------------
	virtual void	Render							();
	virtual void	Update							();
	virtual void	OnClickedChild			(const std::string& name) {/*NOTHING*/;}

	//---------------CButton Interface----------------------
	void					SetTextures					(CTexture* normal, CTexture* over, CTexture* clicked, CTexture* deactivated);
	void					SetLiteral					(const std::string& lit) {m_sLiteral = lit;}
	void					SetColors						(const Math::CColor& normal, const Math::CColor& over, const Math::CColor& clicked, const Math::CColor& deactivated, float alpha = 1.f);
	void					OnOverButton				();
	void					OnClickedButton			();
	bool					IsClicking					() const {return (m_eState == BS_CLICKED);}
	void					SetOnClickedAction	(std::string & inAction );
	void					SetOnOverAction			(std::string & inAction );

private:
	EButtonState	m_eState;
	std::string		m_sLuaCode_OnClicked;
	std::string		m_sLuaCode_OnOver;
	CTexture*			m_pNormalTexture;
	CTexture*			m_pOverTexture;
	CTexture*			m_pClickedTexture;
	CTexture*			m_pDeactivatedTexture;
	Math::CColor				m_NormalColor;
	Math::CColor				m_OverColor;
	Math::CColor				m_ClickedColor;
	Math::CColor				m_DeactivatedColor;
};

#endif //INC_BUTTON_H