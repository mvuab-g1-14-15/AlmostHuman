//----------------------------------------------------------------------------------
// CEditableTextBox class
// Author: Enric Vergara
//
// Description:
// Editable text may be modified by the user by clicking the field and typing. An editable text field seems 
// to be limited to one line. You could use an editable field to allow the user to enter a function to be plotted.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_EDITABLE_TEXT_BOX_H
#define INC_EDITABLE_TEXT_BOX_H

#include <string>
#include <list>
#include "GuiElement\GuiElement.h"

#include "Math/Color.h"

//---Forward Declarations---
class CTexture;
//--------------------------

class CEditableTextBox: public CGuiElement
{

public:
	CEditableTextBox(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
										const Math::Vect2f position_percent, Math::CColor textColor = Math::colBLACK, uint32 fontID = 0, 
										std::string lit="", uint32 textHeightOffset=0, uint32 textWidthOffset=0, 
										bool isVisible = true, bool isActive = true);

	virtual ~CEditableTextBox();

	//---------------CGuiElement Interface----------------------
	virtual void				Render								();
	virtual void				Update								();
	virtual void				OnClickedChild				(const std::string& name) {/*NOTHING*/;}


	//---------------CEditableTextBox Interface----------------------
	void								SetFont								(Math::CColor textColor = Math::colBLACK, uint32 fontID = 0);
	void								SetBackGroundTexture	(CTexture* background)			{m_pBackGroundTexture = background;}
	void								SetBackGroundColor		(const Math::CColor& color)				{m_BackGroundColor = color;}
	const std::string&	GetBuffer							()const											{return m_sBuffer;}
	void								SetBuffer							(const std::string& buffer) {m_sBuffer = buffer; }
	void								AddBuffer							(const std::string& buffer) {m_sBuffer += buffer; }
	bool								IsReturnPress					();

private:
	void								ProcessNewEntries			();
	void								Remove1character			();

private:
	uint32				m_uFontID;
	Math::CColor				m_TextColor;
	CTexture*			m_pBackGroundTexture;
	Math::CColor				m_BackGroundColor;
	std::string		m_sBuffer;
	bool					m_bShift;
	float					m_fTime;
	float					m_fTimeCount;
	std::string		m_sFocusObject;
	bool					m_bFlagDiks[256];
	float					m_fTimeWriteFast;
	float					m_fTimeWriteFastCount;
	float					m_fSpeed;
	float					m_fSpeedCount;
	bool					m_bReturnPress;
	float					m_fWidthMargin;
	float					m_fHeightMargin;
	uint16*				m_Result;

	float					m_BackTime1;
	float					m_BackTime2;
	uint8					m_uCursorPos;
};

#endif //INC_EDITABLE_TEXT_BOX_H