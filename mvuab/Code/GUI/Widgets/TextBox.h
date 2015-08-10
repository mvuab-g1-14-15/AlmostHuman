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
        CTextBox( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution );

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