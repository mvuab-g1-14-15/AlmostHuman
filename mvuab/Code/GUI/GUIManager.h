//----------------------------------------------------------------------------------
// CWindows class
// Author: Enric Vergara
//
// Description:
// Esta clase gestiona el pintado y update de una ventana de la GUI. Esta ventana es simplemente una agrupacion
// de GuiElements que se lee al inicio de la aplicacion de un fichero .xml
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_GUI_MANAGER_H
#define INC_GUI_MANAGER_H

#include <string>
#include <map>
#include "Math/MathUtils.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Widgets\TextBox.h"
#include "Widgets\ConsoleGUI.h"
#include "Utils\Manager.h"

//---Forward Declarations---
class CPointerMouse;
class CGuiElement;
class CWindows;
class CImage;
//class CConsoleGUI;
class CMap;
//--------------------------

//----------Declaracion de nuevos tipos------------------------------------
typedef enum EtypeTransitionEffect {TE_SHADOW = 0};
struct TransitionEffect
{
  TransitionEffect::TransitionEffect(): m_bDoEffect( false ), m_fTransitionTime( 0.f ), m_eType( TE_SHADOW ),
    m_sWindowsName( "" ),
    m_fTimeCounter( 0.f ), m_bActiveWindows( false ) {}
  bool  m_bDoEffect;
  float  m_fTransitionTime;
  float  m_fTimeCounter;
  std::string  m_sWindowsName;
  bool  m_bActiveWindows;
  EtypeTransitionEffect m_eType;

};
//---------------------------------------------------------------------------

class CGUIManager : public CManager
{

public:
  // Init and End protocols
  CGUIManager( const Math::Vect2i& resolution );
  CGUIManager( const CXMLTreeNode& atts );
  virtual ~CGUIManager()
  {
    Done();
  }


  void Init();
  void Done();
  bool IsOk() const
  {
    return m_bIsOk;
  }
  void Render();
  void RenderPointerMouse();
  void Update();
  void SetScreenResolution( const Math::Vect2i& resolution );
  bool LoadGuiFiles( const std::string& pathWindows );
  bool ReloadGuiFiles();
  void SetMessageBox( const std::string& text );
  bool IsVisibleMessage() const
  {
    assert( m_pTextBox );
    return m_pTextBox->IsVisible();
  }
  void ActiveWindows( const std::string& inNameWindow );
  void PushWindows( const std::string& inNameWindow );
  void PopWindows();
  void ActiveWindowsWithEffect( const std::string& inNameWindow, EtypeTransitionEffect type, float transitionTime );
  void SetVisiblePointerMouse( bool flag )
  {
    m_bVisiblePointerMouse = flag;
  }
  void Reload();

  GET_SET( bool, RenderPointer );

  CWindows* GetWindow( const std::string& NameWindow );
  //----CScriptRegister interface--------------------------------------------
  //virtual void RegisterFunctions (CScriptManager* scriptManager);

  //-------Funciones para modificar los GuiElements--------------------------
  void SetActiveGuiElement( const std::string& inNameGuiElement, bool flag );
  void SetVisibleGuiElement( const std::string& inNameGuiElement, bool flag );

  bool GetProgressBarValue( const std::string& inNameGuiElement, float& outValue );
  bool SetProgressBarValue( const std::string& inNameGuiElement, float inValue );

  bool NextBlockInRadioBox( const std::string& inNameRadioBox );
  bool PrevBlockInRadioBox( const std::string& inNameRadioBox );

  std::string GetButtonCheckInRadioBox( const std::string& inNameRadioBox );
  void SetButtonCheckInRadioBox( const std::string& inNameRadioBox, const std::string& button );

  void SetStateCheckButton( const std::string& inCheckButtonName, bool state );
  bool GetStateCheckButton( const std::string& inCheckButtonName );

  void SetStateSlider( const std::string& inSliderName, float amount );
  float  GetStateSlider( const std::string& inSliderName );

  void SetLiteralInStaticText( const std::string& inStaticText, const std::string& lit );
  void ShowStaticText( const std::string& inStaticText );
  void ShowStaticText( const std::string& inStaticText, bool inActive);

  void ShowDialogText( const std::string& inDialogText );
  bool NextDialog();

  void SetEditableTextBox( const std::string& inEditableTextName, const std::string& text );
  std::string GetEditableTextBox( const std::string& inEditableTextName );

  void SetImage( const std::string& inImageName, const std::string& activeImage );
  std::string GetImage( const std::string& inImageName );
  void PlayImage( const std::string& inImageName, float timePerImage, bool loop );

  void ShowImage( bool bShow );
  GET_SET_PTR( CConsoleGUI, Console )
  GET_SET_PTR( CImage, PressButton )
  GET_SET_PTR( CTextBox, TextBox )

  void SetCurrentMap( CMap* aMap );
  void SetCurrentMap( const std::string& aName);
  CMap* GetCurrentMap();

  //-----------------------------------------------------------------------------------------------------------//

private:
  void Release();
  void RenderTransitionEffect();
  bool UpdateTransitionEffect();

private:
  Math::Vect2i   m_ScreenResolution;
  std::map<std::string, CWindows*>  m_WindowsMap;
  std::map<std::string, CGuiElement*> m_ElementsMap;
  std::string  m_sCurrentWindows;
  std::string  m_sLastLoadpathGUI_XML;
  std::vector<std::string> m_PrevWindows;
  bool  m_bIsOk;
  bool  m_bVisiblePointerMouse;
  bool  m_bRenderError;
  bool  m_bUpdateError;
  bool  m_bFirstUpdate;
  bool  m_RenderPointer;
  bool  m_bLoadedGuiFiles;
  CPointerMouse*  m_PointerMouse;
  CTextBox* m_pTextBox;
  CConsoleGUI*  m_pConsole;
  TransitionEffect m_sTransitionEffect;
  CImage* m_pPressButton;
  CDialogBox* m_CurrentDialogBox;
  CMap* m_pCurrentMap;
};

#endif //INC_GUI_MANAGER_H
