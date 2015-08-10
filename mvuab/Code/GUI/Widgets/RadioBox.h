//----------------------------------------------------------------------------------
// CRadioBox class
// Author: Enric Vergara
//
// Description:
// A button, similar to a check-box, except that only one item in a group can be selected. Its name
// comes from the mechanical push-button group on a car radio receiver. Selecting a new item from
// the group's buttons also deselects the previously selected button.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_RADIO_BOX_H
#define INC_RADIO_BOX_H

#include "GuiElement\GuiElement.h"
#include "Widgets\CheckButton.h"
#include "Math/Color.h"


//---Forward Declarations---
class CTexture;
//--------------------------

class CRadioBox: public CGuiElement
{

    public:
        CRadioBox( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution );

        virtual ~CRadioBox()
        {
            /*NOTHING*/;
        }

        //---------------CGuiElement Interface----------------------
        virtual void                    Render                                    ();
        virtual void                    Update                                    ();
        virtual void                    OnClickedChild                    (const std::string& name);

        //---------------CRadioBox Interface----------------------
        void                                    SetTextureBack                    (CTexture* background)
        {
            m_pBackGroundTexture = background;
        }
        void                                    SetCheckButton                    (const std::string& name, CTexture* on,
                CTexture* off, CTexture* deactivated );
        void                                    NextBlock                                ();
        void                                    PrevBlock                                ();
        void                                    SetOnCheckButton                (const std::string& buttonCheck);
        void                                    SetCheckButtonActions        (    const std::string& onCheckOn,
                const std::string& onCheckOff,
                const std::string& onOverButton);
        const std::string&        GetDefaultCheckButton        () const
        {
            return m_sDefaultButtonCheck;
        }
        void                                    SetDefaultCheckButton        (const std::string& checkB)
        {
            m_sDefaultButtonCheck = checkB;
        }

    private:
        CTexture*                                    m_pBackGroundTexture;
        std::vector<CCheckButton> m_CheckButtons;
        std::string                                   m_sDefaultButtonCheck;
        uint32                                        m_uColumns;
        uint32                                        m_uRows;
        uint32                                        m_uLastButtonColumn;
        uint32                                        m_uLastButtonRow;
        uint32                                        m_uBlockCheckButton;
};

#endif //INC_RADIO_BOX_H