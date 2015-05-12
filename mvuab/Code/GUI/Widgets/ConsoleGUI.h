//----------------------------------------------------------------------------------
// CEditableTextBox class
// Author: Enric Vergara
//
// Description:
// Editable text may be modified by the user by clicking the field and typing. An editable text field seems
// to be limited to one line. You could use an editable field to allow the user to enter a function to be plotted.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_CONSOLE_GUI_H
#define INC_CONSOLE_GUI_H

#include <string>
#include <list>
#include "Widgets\EditableTextBox.h"

#include "Math/Color.h"

//---Forward Declarations---
//--------------------------

class CConsoleGUI: public CEditableTextBox
{

    public:
        CConsoleGUI(    uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
                        const Math::Vect2f position_percent, Math::CColor textColor = Math::colBLACK, uint32 fontID = 0,
                        std::string lit = "", uint32 textHeightOffset = 0, uint32 textWidthOffset = 0,
                        bool isVisible = true, bool isActive = true);

        virtual ~CConsoleGUI();

        //---------------CGuiElement Interface----------------------
        virtual void                Render                                ();
        virtual void                Update                                ();

    private:
        std::vector<std::string>            m_vBuffer;
        std::vector<std::string>::iterator    m_itActual;
};

#endif //INC_CONSOLE_GUI_H