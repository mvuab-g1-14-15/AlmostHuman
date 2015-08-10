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
        CConsoleGUI( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution );

        virtual ~CConsoleGUI();

        //---------------CGuiElement Interface----------------------
        virtual void                Render                                ();
        virtual void                Update                                ();

    private:
        std::vector<std::string>            m_vBuffer;
        std::vector<std::string>::iterator    m_itActual;
};

#endif //INC_CONSOLE_GUI_H