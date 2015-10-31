

#include "Widgets\DialogBox.h"
#include "InputManager.h"
#include "Texture/Texture.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Widgets\StaticText.h"

CDialogBox::CDialogBox( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution )
    : CGuiElement( aNode, screenResolution )
    , m_bDialogClicked(false)
    , m_ButtonClose( aNode, screenResolution)
    //, m_ButtonMove( aNode, screenResolution )
    , m_pBackGroundTexture(aNode.GetAttribute<CTexture>( "quad" ))
    , m_BackGroundColor(Math::colWHITE)
    , m_bStart_to_Move(false)
    , m_PreviousPosMouse(Math::v2iZERO)
    , m_fButtonHeight( 0.0f )
    , m_fButtonWidth( 0.0f )
{
    m_ButtonClose.SetParent(this);
    m_ButtonClose.SetName("buttonClose");
	m_ButtonClose.SetLiteral( "" );
    m_ButtonClose.SetTextures
    (
        aNode.GetAttribute<CTexture>( "buttonClose_normal" ),
        aNode.GetAttribute<CTexture>( "buttonClose_over" ),
        aNode.GetAttribute<CTexture>( "buttonClose_clicked" ),
        aNode.GetAttribute<CTexture>( "buttonClose_deactivated" )
    );
	m_ButtonClose.SetPositionPercent(m_PositionPercent + aNode.GetAttribute<Math::Vect2f>("pos_button", Math::Vect2f(0,0)));
	m_ButtonClose.SetWidthPercent(aNode.GetAttribute<float>("width_button",0.0f));
	m_ButtonClose.SetHeightPercent(aNode.GetAttribute<float>("height_button", 0.0f));

	for ( int i = 0, count = aNode.GetNumChildren(); i < count; ++i )
    {
		CXMLTreeNode  SubTreeNode = aNode( i );
        const std::string& TagName = SubTreeNode.GetName();
		if( TagName == "Dialog" )
		{
			CStaticText* lTexto1 = new CStaticText(SubTreeNode(0), screenResolution );
			CStaticText* lTexto2 = new CStaticText(SubTreeNode(1), screenResolution );
			CStaticText* lTexto3 = new CStaticText(SubTreeNode(2), screenResolution );
			lTexto1->SetPositionPercent(m_PositionPercent + lTexto1->GetPositionPercent());
			lTexto2->SetPositionPercent(m_PositionPercent + lTexto2->GetPositionPercent());
			lTexto3->SetPositionPercent(m_PositionPercent + lTexto3->GetPositionPercent());
			CDialog* lDialog = new CDialog(lTexto1, lTexto2, lTexto3);
			m_Dialogo.push_back(lDialog);
		}
	}
	m_CurrentDialog = m_Dialogo.begin();


}

//---------------Interfaz de GuiElement----------------------
void CDialogBox::Render    ()
{
    if (GetVisible())
    {
        if (m_pBackGroundTexture)
        {
            //TODO RAUL CREAR METODO DRAWQUAD2D CON TEXTURA
            GraphicsInstance->DrawQuad2D(CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight,
                                         CGraphicsManager::UPPER_LEFT, m_pBackGroundTexture);
        }
        else
        {
            GraphicsInstance->DrawQuad2D(CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight,
                                         CGraphicsManager::UPPER_LEFT, m_BackGroundColor);
        }
		
//        m_ButtonMove.Render();
        m_ButtonClose.Render();
    }
	
	(*m_CurrentDialog)->Render();
    //Finalmente renderizamos el texto:
    //CGuiElement::RenderText();
}

void CDialogBox::Update ()
{
    if (GetVisible() && CGuiElement::m_bIsActive)
    {
       // Math::Vect2i posMouse;
       // InputManagerInstance->GetPosition(IDV_MOUSE, posMouse);
       // if( m_bDialogClicked )
       // {
       //     if (!InputManagerInstance->IsDown(IDV_MOUSE, MOUSE_BUTTON_LEFT))
       //     {
       //         m_bDialogClicked = false;
       //         m_bStart_to_Move = false;
       //     }
       //     else
       //     {
       //         if( !m_bStart_to_Move )
       //         {
       //             m_bStart_to_Move = true;
       //             m_PreviousPosMouse = posMouse;
       //         }
       //         else
       //         {
       //             Math::Vect2i incPos = posMouse - m_PreviousPosMouse;
       //             m_PreviousPosMouse = posMouse;
       //             //m_ButtonMove.SetPosition( m_ButtonMove.GetPosition() + incPos );
       //             m_ButtonClose.SetPosition( m_ButtonClose.GetPosition() + incPos );
       //             SetPosition( GetPosition() + incPos);
       //         }
       //     }
       // }

       // CGuiElement::CalculatePosMouse(posMouse);
       // if (CGuiElement::IsInside())
       // {
       //     if (InputManagerInstance->IsDown(IDV_KEYBOARD, 0))
       //     {
       //         CGuiElement::m_bFocus = true;
       //     }
       // }

       // m_ButtonClose.Update();
       // //m_ButtonMove.Update();

       // CGuiElement::CalculatePosMouse(posMouse);
       ///* if( m_ButtonMove.IsClicking() )
       // {
       //     m_bDialogClicked = true;
       // }*/

    }// End if( GuiElement::mIsVisible && GuiElement::mIsActive )
}

void CDialogBox::OnClickedChild( const std::string& name )
{
    if( name.compare("buttonClose") == 0 )
    {
        CGuiElement::m_bIsActive = false;
        SetVisible( false );
    }
}

void CDialogBox::SetPosition(const Math::Vect2i& pos)
{
    //m_ButtonMove.SetPosition( pos );
    m_ButtonClose.SetPosition( pos + Math::Vect2i((uint32)(CGuiElement::m_uWidth - m_ButtonClose.GetWidth()), 0) );
    CGuiElement::SetPosition( pos );
}



//---------------Interfaz de Button---------------------------
void CDialogBox::SetCloseButtonTextures    (CTexture* normal, CTexture* over, CTexture* clicked, CTexture* deactivated)
{
    m_ButtonClose.SetTextures(normal, over, clicked, deactivated);
}

void CDialogBox::SetMoveButtonTextures (CTexture* normal, CTexture* over, CTexture* clicked, CTexture* deactivated)
{
//    m_ButtonMove.SetTextures(normal, over, clicked, deactivated);
}

void CDialogBox::SetDialogTexture (CTexture* background)
{
    assert(background);
    m_pBackGroundTexture = background;
}

void CDialogBox::SetCloseButtonColors (    const Math::CColor& normal, const Math::CColor& over,
        const Math::CColor& clicked,
        const Math::CColor& deactivated)
{
    m_ButtonClose.SetColors(normal, over, clicked, deactivated);
}
void CDialogBox::SetMoveButtonColors (const Math::CColor& normal, const Math::CColor& over, const Math::CColor& clicked,
                                      const Math::CColor& deactivated)
{
   // m_ButtonMove.SetColors(normal, over, clicked, deactivated);
}

void CDialogBox::SetDialogColors (const Math::CColor& background)
{
    m_BackGroundColor = background;
}

bool CDialogBox::NextDialog()
{
	
		(*m_CurrentDialog)->SetActive(false);
		++m_CurrentDialog;
	
	if( m_CurrentDialog == m_Dialogo.end() )
	{
		//m_CurrentDialog->SetActive(false);
		/*GetVisible() = false;
		CGuiElement::m_bIsActive = false;*/
		m_CurrentDialog = m_Dialogo.begin();
		return true;
	}
	else
	{	
		(*m_CurrentDialog)->SetActive(true);
	}
	return false;
}

void CDialogBox::StartDialog()
{
	EngineManagerInstance->SetbGamePause(true);
	(*m_CurrentDialog)->SetActive(true);
	SetActive(true);
	SetVisible(true);
	m_ButtonClose.SetActive(true);
    m_ButtonClose.SetVisible(true);
}

void CDialogBox::EndDialog()
{
	(*m_CurrentDialog)->SetActive(false);
	SetActive(false);
	SetVisible(false);
	m_ButtonClose.SetActive(false);
    m_ButtonClose.SetVisible(false);
	m_CurrentDialog = m_Dialogo.begin();
	EngineManagerInstance->SetbGamePause(false);
}

CDialogBox::CDialog::CDialog(CStaticText* aTexto1, CStaticText* aTexto2, CStaticText* aTexto3)
{
	m_Texto1 = aTexto1;
	m_Texto2 = aTexto2;
	m_Texto3 = aTexto3;
}
void CDialogBox::CDialog::Render()
{
	m_Texto1->Render();
	m_Texto2->Render();
	m_Texto3->Render();
}
void CDialogBox::CDialog::SetActive(bool flag)
{
	m_Texto1->SetActive(flag);
	m_Texto2->SetActive(flag);
	m_Texto3->SetActive(flag);
	m_Texto1->SetVisible(flag);
	m_Texto2->SetVisible(flag);
	m_Texto3->SetVisible(flag);
}