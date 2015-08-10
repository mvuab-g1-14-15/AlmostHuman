//----------------------------------------------------------------------------------
// CStaticText class
// Author: Enric Vergara
//
// Description:
// Static text may be changed by the program, but not directly by the user. You might use this for labels or
// instructions to the user.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_STATIC_TEXT_H
#define INC_STATIC_TEXT_H

#include <string>
#include "GuiElement\GuiElement.h"
#include "Math/Color.h"

//---Forward Declarations---
//--------------------------


class CStaticText: public CGuiElement
{

    public:
        CStaticText( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution );

        virtual ~CStaticText()
        {
            /*NOTHING*/;
        }


        //---------------CGuiElement Interface----------------------
        virtual void    Render                            ();
        virtual void    Update                            ();
        virtual void    OnClickedChild            (const std::string& name)
        {
            /*NOTHING*/;
        }

        //---------------CStaticText Interface------------------
        void                    SetLiteral                    (const std::string& lit);

    private:
};

#endif //INC_STATIC_TEXT_H