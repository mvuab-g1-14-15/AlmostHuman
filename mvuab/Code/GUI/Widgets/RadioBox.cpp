#include "RadioBox.h"
#include "InputManager.h"
#include "Texture/Texture.h"
#include "GraphicsManager.h"
#include "Logger\Logger.h"
#include "ScriptManager.h"
#include "EngineManagers.h"

//---Constructor

CRadioBox::CRadioBox( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution )
    : CGuiElement( aNode, screenResolution )
    , m_pBackGroundTexture(aNode.GetAttribute<CTexture>( "texture_back"))
    , m_sDefaultButtonCheck(aNode.GetAttribute<std::string>( "default_checkButton", "" ))
    , m_uColumns( aNode.GetAttribute<int32>( "columns", 0 ) )
    , m_uRows( aNode.GetAttribute<int32>( "rows", 0 ) )
    , m_uLastButtonColumn(0)
    , m_uLastButtonRow(0)
    , m_uBlockCheckButton(0)
{
    m_CheckButtons.reserve(m_uColumns * m_uRows);
    SetCheckButtonActions
    (
        aNode.GetAttribute<std::string>( "OnCheckOn", "" ),
        aNode.GetAttribute<std::string>( "OnCheckOff", "" ),
        aNode.GetAttribute<std::string>( "OnOverButton", "" )
    );

    for ( uint32 j = 0, lCount = aNode.GetNumChildren(); j < lCount; ++j )
    {
        const CXMLTreeNode& pTexture = aNode( j );
        const std::string& tagName = pTexture.GetName();

        if( tagName == "texture" )
        {
            SetCheckButton
            (
                pTexture.GetAttribute<std::string>( "name" , ""),
                pTexture.GetAttribute<CTexture>("on"),
                pTexture.GetAttribute<CTexture>("off"),
                pTexture.GetAttribute<CTexture>("deactivated")
            );
        }
    }
}

//---------------CGuiElement Interface----------------------
void CRadioBox::Render ()
{
    if( CGuiElement::m_bIsVisible)
    {
        if (m_pBackGroundTexture)
        {
            //TODO RAUL
            GraphicsInstance->DrawQuad2D(CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight,
                                         CGraphicsManager::UPPER_LEFT, m_pBackGroundTexture);
        }

        uint32 cont_init = m_uBlockCheckButton * (m_uColumns * m_uRows);
        uint32 cont_end = m_uColumns * m_uRows;
        for( uint32 cont = cont_init; cont < cont_init + cont_end; cont++ )
        {
            m_CheckButtons[cont].Render();
        }

        //Finalmente renderizamos el texto:
        CGuiElement::RenderText();
    }
}
void CRadioBox::Update ()
{
    if( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
    {
        uint32 cont_init = m_uBlockCheckButton * (m_uColumns * m_uRows);
        uint32 cont_end = m_uColumns * m_uRows;

        for( uint32 cont = cont_init; cont < cont_init + cont_end; cont++ )
        {
            m_CheckButtons[cont].Update();
        }
    }
}

void CRadioBox::OnClickedChild( const std::string& inName )
{
    CGuiElement::m_bFocus = true;

    std::vector<CCheckButton>::iterator it(m_CheckButtons.begin());
    std::vector<CCheckButton>::iterator itEnd(m_CheckButtons.end());
    //[sp] Tenemos que recorrer todos los CheckButtons para ponerlos a todos en off menos el inName
    while (it != itEnd)
    {
        if( it->GetName().compare(inName) != 0 )
        {
            it->SetOn( false );
        }
        else
        {
            it->SetOn( true );
        }
        it++;
    }

    m_sDefaultButtonCheck = inName;
}

//---------------CCheckButton Interface----------------------
void CRadioBox::SetCheckButton (const std::string& name, CTexture* on, CTexture* off, CTexture* deactivated )
{
    /*
     float heightButton    = m_fHeightPercent / m_uRows;
     float widthButton        = m_fWidthPercent / m_uColumns;

     Math::Vect2f pos;
     pos.y = m_PositionPercent.y + m_uLastButtonRow * heightButton;
     pos.x = m_PositionPercent.x + m_uLastButtonColumn * widthButton;

     CCheckButton newCheckButton = CCheckButton(
                                       CGuiElement::m_uWindowsHeight,
                                       CGuiElement::m_uWindowsWidth,
                                       heightButton,
                                       widthButton,
                                       pos,
                                       false,
                                       "",
                                       0,
                                       0,
                                       false,
                                       true);
     if( name.compare(m_sDefaultButtonCheck) == 0)
     {
         newCheckButton.SetOn(true);
     }
     newCheckButton.SetTextures(on, off, deactivated);
     newCheckButton.SetParent(this);
     newCheckButton.SetName(name);
     m_CheckButtons.push_back(newCheckButton);

     m_uLastButtonColumn++;
     if( m_uLastButtonColumn == m_uColumns )
     {
         m_uLastButtonColumn = 0;
         m_uLastButtonRow++;
         if( m_uLastButtonRow == m_uRows )
         {
             m_uLastButtonRow = 0;
         }
     }
     */
}

void CRadioBox::NextBlock ()
{
    m_uBlockCheckButton++;
    if( m_uBlockCheckButton >= m_CheckButtons.size() / (m_uColumns * m_uRows) )
    {
        m_uBlockCheckButton--;
    }
}

void CRadioBox::PrevBlock ()
{
    if( m_uBlockCheckButton != 0 )
    {
        m_uBlockCheckButton--;
    }
}

void CRadioBox::SetOnCheckButton (const std::string& buttonCheck)
{
    if( buttonCheck.compare(m_sDefaultButtonCheck) != 0 )
    {
        CGuiElement::m_bFocus = true;

        m_sDefaultButtonCheck = buttonCheck;

        std::vector<CCheckButton>::iterator it(m_CheckButtons.begin());
        std::vector<CCheckButton>::iterator itEnd(m_CheckButtons.end());
        while (it != itEnd)
        {
            //[sp] Tenemos que recorrer todos los CheckButtons para ponerlos a todos en off menos el inName
            if( it->GetName().compare(buttonCheck) != 0 )
            {
                it->SetOn( false );
            }
            else
            {
                it->SetOn( true );
            }
            it++;
        }

    }//END if( buttonCheck.compare(m_sDefaultButtonCheck) != 0 )
}

void CRadioBox::SetCheckButtonActions (const std::string& onCheckOn, const std::string& onCheckOff,
                                       const std::string& onOverButton)
{
    std::vector<CCheckButton>::iterator it(m_CheckButtons.begin());
    std::vector<CCheckButton>::iterator itEnd(m_CheckButtons.end());
    while (it != itEnd)
    {
        it->SetOnCheckOffAction(onCheckOff);
        it->SetOnCheckOnAction(onCheckOn);
        it->SetOnOverAction(onOverButton);
        it++;
    }
}
