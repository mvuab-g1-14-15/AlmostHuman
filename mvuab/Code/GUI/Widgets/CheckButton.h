//----------------------------------------------------------------------------------
// CheckButton class
// Author: Enric Vergara
//
// Description:
// A checkbutton is used when you need to specify a persistent binary condition. An example might be to turn on/off
// the grid associated with the axes of a plot.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_CHECK_BUTTON_H
#define INC_CHECK_BUTTON_H

#include <string>
#include "GuiElement\GuiElement.h"
#include "Math/Color.h"


//---Forward Declarations---
class CTexture;
//--------------------------

class CCheckButton: public CGuiElement
{

    private:
        typedef enum ECheckButtonState { CBS_ON, CBS_OFF };

    public:
        CCheckButton( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution );

        virtual ~CCheckButton()
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


        //---------------CCheckButton Interface----------------------
        void                    SetTextures                    (CTexture* on, CTexture* off, CTexture* deactivated );
        void                    SetLiteral                    (const std::string& lit)
        {
            m_sLiteral = lit;
        }
        void                    SetColors                        (const Math::CColor& on, const Math::CColor& off,
                const Math::CColor& deactivated, float alpha = 1.f);
        void                    OnCheckOn                        ();
        void                    OnCheckOff                    ();
        void                    OnOverButton                ();
        void                    SetOnCheckOnAction    (const std::string& inAction);
        void                    SetOnCheckOffAction    (const std::string& inAction);
        void                    SetOnOverAction            (const std::string& inAction);
        void                    SetOn                                (bool On)
        {
            if (On)
            {
                m_eState = CBS_ON;
            }
            else
            {
                m_eState = CBS_OFF;
            }
        }
        bool                    GetState                        () const
        {
            return m_eState == CBS_ON;
        }

    private:
        ECheckButtonState    m_eState;
        CTexture*                    m_pOnTexture;
        CTexture*                    m_pOffTexture;
        CTexture*                    m_pDeactivatedTexture;
        std::string                m_sLuaCode_OnCheckOn;
        std::string                m_sLuaCode_OnCheckOff;
        std::string                m_sLuaCode_OnOver;
        Math::CColor                        m_OnColor;
        Math::CColor                        m_OffColor;
        Math::CColor                        m_DeactivatedColor;
};

#endif //INC_CHECK_BUTTON_H