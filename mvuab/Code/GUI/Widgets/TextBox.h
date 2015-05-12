//----------------------------------------------------------------------------------
// CTextBox class
// Author: Enric Vergara
//
// Description:
// A TextBox...
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_TEXT_BOX_H
#define INC_TEXT_BOX_H

#include "Widgets\DialogBox.h"
#include "Math/Color.h"
#include <string>

//---Forward Declarations---
class CGraphicsInstance;
//--------------------------


class CTextBox: public CDialogBox
{
    public:
        CTextBox(    uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
                     const Math::Vect2f position_percent, float buttonWidthPercent, float buttonHeightPercent,
                     std::string lit = "", uint32 textHeightOffset = 0, uint32 textWidthOffset = 0,
                     bool isVisible = true, bool isActive = true);

        virtual ~CTextBox()
        {
            /*NOTHING*/;
        }

        //---------------CDialogBox Interface----------------------
        virtual void    Render                    ();

        //---------------CTextBox Interface---------------------------
        void                    SetMessage            (const std::string& message)
        {
            m_sMessage = message;
        }
        void                    SetFont                    (Math::CColor textColor = Math::colBLACK, uint32 fontID = 0);

    private:
        std::string        m_sMessage;
        uint32                m_uFontID;
        Math::CColor                m_TextColor;
};

#endif //INC_TEXT_BOX_H