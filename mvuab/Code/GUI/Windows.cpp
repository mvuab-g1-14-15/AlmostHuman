#include "__PCH_GUI.h"

#include "Windows.h"
#include "InputManager.h"
#include "Texture/Texture.h"
#include "Texture/TextureManager.h"
#include "GraphicsManager.h"
#include "Logger/Logger.h"
#include "Core.h"
#include "xml/XMLTreeNode.h"
#include "EngineManagers.h"
#include "ScriptManager.h"


//---Elementos que puede contener una Windows--
#include "Button.h"
#include "CheckButton.h"
#include "Slider.h"
#include "EditableTextBox.h"
#include "DialogBox.h"
#include "RadioBox.h"
#include "Image.h"
#include "StaticText.h"
#include "ProgressBar.h"
//---------------------------------------------

CWindows::~CWindows()
{
	std::vector<CGuiElement*>::iterator it(m_GuiElementsVector.begin());
	std::vector<CGuiElement*>::iterator itEnd(m_GuiElementsVector.end());	
	while (it != itEnd)
	{
		delete (*it);
		it++;
	}
	m_GuiElementsVector.clear();
}

bool CWindows::AddGuiElement( CGuiElement * inGuiElement )
{
	//Primero comprobamos que no exista ya el guiElement a introducir
	std::vector<CGuiElement*>::iterator it(m_GuiElementsVector.begin());
	std::vector<CGuiElement*>::iterator itEnd(m_GuiElementsVector.end());
	while (it != itEnd)
	{
		CGuiElement* guiElement = *it;
		if (guiElement == inGuiElement)
		{
			LOG_ERROR_APPLICATION("Windows:: El GuiElement %s ya se ha introducido en la windows %s", inGuiElement->GetName().c_str(),m_sWindowsName.c_str());
			return false;
		}
		it++;
	}

	//Finalmente lo insertamos en el vector de GuiElements
	m_GuiElementsVector.push_back(inGuiElement);
	return true;
}

bool CWindows::ReleaseGuiElement( const std::string & name )
{
	std::vector<CGuiElement*>::iterator it(m_GuiElementsVector.begin());
	std::vector<CGuiElement*>::iterator itEnd(m_GuiElementsVector.end());

	while (it != itEnd)
	{
		if( (*it)->GetName().compare(name) == 0 )
		{
			m_GuiElementsVector.erase(it);
			return true;
		}
		it++;
	}

	LOG_WARNING_APPLICATION("Windows:: Se ha intentado eliminar el GuiElement %s pero no existe en la windows %s", name.c_str(),m_sWindowsName.c_str());
	return false;
}

void CWindows::Render ()
{
	std::vector<CGuiElement*>::iterator it(m_GuiElementsVector.begin());
	std::vector<CGuiElement*>::iterator itEnd(m_GuiElementsVector.end());

	while (it != itEnd)
	{
		(*it)->Render ();
		it++;
	}
}


void CWindows::Update ()
{

	if (m_sLuaCode_OnUpdateWindows.compare(""))
	{
		//Lanzar acción en Lua:
		//TODO RAUL SCRIPT
		//CScriptManager* scriptManager = CORE->GetScriptManager();
		ScriptMInstance->RunCode(m_sLuaCode_OnUpdateWindows);
	}

	std::vector<CGuiElement*>::reverse_iterator revIt;
	std::vector<CGuiElement*>::reverse_iterator revItEnd(m_GuiElementsVector.rend());
	std::vector<CGuiElement*>::reverse_iterator revIt_aux;
	bool focus_aux = false;
	//Primero de todo quitamos el focus a todos los GuiElements 
	for( revIt = m_GuiElementsVector.rbegin(); revIt != revItEnd; ++revIt )
	{
		// De haber alguno que tenga focus nos lo guardamos, ya que al recalcular el focus si no lo tiene nadie
		// como mínimo se lo damos al que lo tenía.
		if( (*revIt)->HasFocus() )
		{
			focus_aux = true;
			revIt_aux = revIt;
		}

		(*revIt)->LoseFocus();
	}

	bool elemWithFocus = false;
	for( revIt = m_GuiElementsVector.rbegin(); revIt != revItEnd; ++revIt )
	{
		(*revIt)->Update();
		if( (*revIt)->HasFocus() )
		{
			elemWithFocus = true;
			break;
		}
	}

	if( elemWithFocus )
	{
		CGuiElement* guiElement = (*revIt);
		m_GuiElementsVector.erase(revIt.base() - 1);
		m_GuiElementsVector.push_back( guiElement );
	}
	else if( focus_aux )
	{
		(*revIt_aux)->GainFocus();
		(*revIt_aux)->Update();
	}

	IsKeyDown(InputManagerInstance);
}


void CWindows::RegisterElements( std::map<std::string,CGuiElement*>& elements )
{
	std::map<std::string,CGuiElement*>::const_iterator it_aux;

	std::vector<CGuiElement*>::iterator it(m_GuiElementsVector.begin());
	std::vector<CGuiElement*>::iterator itEnd(m_GuiElementsVector.end());
	while (it != itEnd)
	{
		CGuiElement* element = (*it);
		//Antes de insertarlo en el mapa debemos comprobar que no este registrado ya un GuiElement con el mismo identificado(mismo string):
		if (elements.find(element->GetName()) != elements.end())
		{
			LOG_ERROR_APPLICATION("Windows:: El GuiElement %s ya se ha registrado en el GuiManager", element->GetName().c_str());
		}
		else
		{
			elements.insert(std::pair<std::string,CGuiElement*>(element->GetName(), element));
		}
		it++;
	}
}

void CWindows::LoadWindows( void )
{
	std::vector<CGuiElement*>::iterator it;
	std::vector<CGuiElement*>::iterator itEnd(m_GuiElementsVector.end());
	for( it = m_GuiElementsVector.begin(); it != itEnd; ++it )
	{
		CGuiElement * guiElement = (*it);
		guiElement->OnLoadValue();
	}
	
	if (m_sLuaCode_OnLoadWindows.compare(""))
	{
		//Lanzar acción en Lua:
		//TODO RAUL SCRIPT
		//CScriptManager* scriptManager = CORE->GetScriptManager();
		ScriptMInstance->RunCode(m_sLuaCode_OnLoadWindows);
	}
	
}

void CWindows::SaveWindows( void )
{

	std::vector<CGuiElement*>::iterator it;
	std::vector<CGuiElement*>::iterator itEnd(m_GuiElementsVector.end());
	for( it = m_GuiElementsVector.begin(); it != itEnd; ++it )
	{
		CGuiElement * guiElement = (*it);
		guiElement->OnSaveValue();
	}

	if (m_sLuaCode_OnSaveWindows.compare(""))
	{
		//Lanzar acción en Lua:
		//TODO RAUL SCRIPT
		//CScriptManager* scriptManager = CORE->GetScriptManager();
		ScriptMInstance->RunScript(m_sLuaCode_OnSaveWindows);
	}
}

void CWindows::IsKeyDown( CInputManager* InputManagerInstance )
{
	if (m_sLuaCode_OnKeyDown.compare(""))
	{
		//Comprobar si el input se cumple
		if (InputManagerInstance->IsUpDown(IDV_KEYBOARD,m_uInputKeyDown))
		{
			//Lanzar acción en Lua:
			//TODO RAUL SCRIPT
			//CScriptManager* scriptManager = CORE->GetScriptManager();
			ScriptMInstance->RunScript(m_sLuaCode_OnKeyDown);
		}
	}
}


bool CWindows::LoadXML( const std::string &xmlGuiFile, const Math::Vect2i& screenResolution)
{
	//Read the xml gui file
	LOG_INFO_APPLICATION("CWindows:: Iniciando el parseo del fichero %s", xmlGuiFile.c_str());
	bool isOK = false;

	CXMLTreeNode newFile;
	if (!newFile.LoadFile(xmlGuiFile.c_str()))
	{
		LOG_ERROR_APPLICATION("Windows:: No se ha podido leer correctamente el fichero ->%s", xmlGuiFile.c_str());
		isOK = false;
	}
	else
	{
		CTextureManager* textureM = TextureMInstance;
		CXMLTreeNode  windows = newFile["Windows"];

		m_sLuaCode_OnLoadWindows		= windows.GetPszProperty("OnLoadWindows");
		m_sLuaCode_OnSaveWindows		= windows.GetPszProperty("OnSaveWindows");
		m_sLuaCode_OnUpdateWindows	= windows.GetPszProperty("OnUpdateWindows");


		if (windows.Exists())
		{
			int count = windows.GetNumChildren();
			for (int i = 0; i < count; ++i)
			{
				CXMLTreeNode pNewNode = windows(i);

				//Para cada guielement leemos su informacion comun-->
				std::string name	= pNewNode.GetPszProperty("name", "defaultGuiElement");
				float posx				= pNewNode.GetFloatProperty("posx", 0.f);
				float posy				= pNewNode.GetFloatProperty("posy", 0.f);
				float w						= pNewNode.GetFloatProperty("width", 50.f);
				float h						= pNewNode.GetFloatProperty("height", 50.f);
				bool visible			= pNewNode.GetBoolProperty("visible", true);
				bool activated		= pNewNode.GetBoolProperty("active", true);

				std::string tagName = pNewNode.GetName();
				if (tagName.compare("Button")==0)
				{
					CButton* newButton = NULL;
					LoadButton(newButton,pNewNode,screenResolution, textureM);
					AddGuiElement(newButton);
				}
				else if (tagName.compare("CheckButton")==0)
				{
					CCheckButton* new_checkButton = NULL;
					LoadCheckButton(new_checkButton,pNewNode,screenResolution, textureM);									
					AddGuiElement(new_checkButton);
				}
				else if (tagName.compare("Slider")==0)
				{
					CSlider* new_slider = NULL;
					LoadSlider(new_slider,pNewNode,screenResolution, textureM);					
					AddGuiElement(new_slider);
				}
				else if (tagName.compare("DialogBox")==0)
				{
					CDialogBox* new_dialogBox = NULL;
					LoadDialogBox(new_dialogBox,pNewNode,screenResolution, textureM);					
					AddGuiElement(new_dialogBox);
				}
				else if (tagName.compare("EditableTextBox")==0)
				{
					CEditableTextBox* new_editableTextBox = NULL;
					LoadEditableTextBox(new_editableTextBox,pNewNode,screenResolution, textureM);					
					AddGuiElement(new_editableTextBox);
				}
				else if (tagName.compare("RadioBox")==0)
				{
					CRadioBox* new_radioBox = NULL;
					LoadRadioBox(new_radioBox,pNewNode,screenResolution, textureM);		
					AddGuiElement(new_radioBox);
				}
				else if (tagName.compare("Image")==0)
				{
					CImage* new_image = NULL;
					_LoadImage(new_image,pNewNode,screenResolution, textureM);		
					AddGuiElement(new_image);
				}
				else if (tagName.compare("ProgressBar")==0)
				{
					CProgressBar* new_progressBar = NULL;
					LoadProgressBar(new_progressBar,pNewNode,screenResolution, textureM);		
					AddGuiElement(new_progressBar);
				} 
				else if (tagName.compare("StaticText")==0)
				{
					CStaticText* new_staticText = NULL;
					LoadStaticText(new_staticText,pNewNode,screenResolution, textureM);		
					AddGuiElement(new_staticText);
				}
				else if (tagName.compare("KeyBoard_Back")==0)
				{
					//<KeyBoard_Back input="DIK_A" OnKeyDown="blablaLua"/>
					m_sLuaCode_OnKeyDown	= pNewNode.GetPszProperty("OnKeyDown", "");
					m_uInputKeyDown				= pNewNode.GetIntProperty("input", 0);
				}
				else
				{
					//Warning
					LOG_WARNING_APPLICATION("Windows:: No se reconoce el tag %s del fichero %s", tagName.c_str(), xmlGuiFile.c_str());
				}

			}
			isOK = true;
		}
		else
		{
			LOG_ERROR_APPLICATION("Windows:: No se ha podido leer el tag Windows del fichero ->%s", xmlGuiFile.c_str());
			isOK = false;
		}

	}//END else de if (!newFile.LoadFile(xmlGuiFile.c_str()))


	if (!isOK)
	{
		LOG_ERROR_APPLICATION("Windows:: No se ha podido leer correctamente el fichero -->%s", xmlGuiFile.c_str());
		isOK =  false;
	}
	else
	{
		LOG_INFO_APPLICATION("CWindows:: Finalizado correctamente el parseo el fichero %s", xmlGuiFile.c_str());

	}

	return isOK;
}


void CWindows::LoadButton (CButton* button, CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution, CTextureManager* tm)
{
	//<Button	name="play"	posx="0" posy="0" height="10" width="10" visible="true" activated="true" 
	// texture_normal="blabla" texture_over="bla" texture_clicked="bla" texture_deactivated="bla"
	// OnClickedAction="blabla" OnOverAction="blabla" Literal="blabla" widthOffset="" heightOffset=""/>

	std::string name								= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float 			posx								= pNewNode.GetFloatProperty("posx", 0.f);
	float 			posy								= pNewNode.GetFloatProperty("posy", 0.f);
	float 			w										= pNewNode.GetFloatProperty("width", 50.f);
	float 			h										= pNewNode.GetFloatProperty("height", 50.f);
	bool 				visible							= pNewNode.GetBoolProperty("visible", true);
	bool 				activated						= pNewNode.GetBoolProperty("active", true);
	std::string texture_normal			= pNewNode.GetPszProperty("texture_normal", "");
	std::string texture_over				= pNewNode.GetPszProperty("texture_over", "");
	std::string texture_clicked			= pNewNode.GetPszProperty("texture_clicked", "");
	std::string texture_deactivated	= pNewNode.GetPszProperty("texture_deactivated", "");
	std::string OnClickedAction			= pNewNode.GetPszProperty("OnClickedAction", "");
	std::string OnOverAction				= pNewNode.GetPszProperty("OnOverAction", "");
	std::string l_literal						= pNewNode.GetPszProperty("Literal", "");
	float				widthOffsetPercent	= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float				heightOffsetPercent	= pNewNode.GetFloatProperty("heightOffset", 0.f);

	CTexture* normal			= tm->GetTexture(texture_normal);
	CTexture* over				= tm->GetTexture(texture_over);
	CTexture* clicked			= tm->GetTexture(texture_clicked);
	CTexture* deactivated	= tm->GetTexture(texture_deactivated);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

	button	= new CButton(screenResolution.y,screenResolution.x, h, w, Math::Vect2f(posx,posy), l_literal, heightOffset, widthOffset, visible, activated);
	button->SetName(name);
	button->SetTextures(normal,over,clicked,deactivated);
	button->SetOnClickedAction(OnClickedAction);
	button->SetOnOverAction(OnOverAction);
}

void CWindows::LoadCheckButton (CCheckButton* checkButton, CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution, CTextureManager* tm)
{
	//<CheckButton name="debug_thps" posx="0" posy="2" height="10" width="10" visible="true" active="true" isOn="true"
	// texture_on="blabla" texture_off="bla" texture_deactivated"bla"
	// OnCheckOn="blabla" OnCheckOff="blabla" OnOverButton="blabla"
	// OnSaveValue="blabl" OnLoadValue="" Literal="blabla" widthOffset="" heightOffset=""/>

	std::string name									= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float 			posx									= pNewNode.GetFloatProperty("posx", 0.f);
	float 			posy									= pNewNode.GetFloatProperty("posy", 0.f);
	float 			w											= pNewNode.GetFloatProperty("width", 50.f);
	float 			h											= pNewNode.GetFloatProperty("height", 50.f);
	bool 				visible								= pNewNode.GetBoolProperty("visible", true);
	bool 				activated							= pNewNode.GetBoolProperty("active", true);
	bool 				isOn									= pNewNode.GetBoolProperty("isOn", "");
	std::string texture_on						= pNewNode.GetPszProperty("texture_on", "");
	std::string texture_off						= pNewNode.GetPszProperty("texture_off", "");
	std::string texture_deactivated		= pNewNode.GetPszProperty("texture_deactivated", "");
	std::string OnCheckOn							= pNewNode.GetPszProperty("OnCheckOn", "");
	std::string OnCheckOff						= pNewNode.GetPszProperty("OnCheckOff", "");
	std::string OnOverButton					= pNewNode.GetPszProperty("OnOverButton", "");
	std::string OnSaveValue						= pNewNode.GetPszProperty("OnSaveValue", "");
	std::string OnLoadValue						= pNewNode.GetPszProperty("OnLoadValue", "");
	std::string l_literal							= pNewNode.GetPszProperty("Literal", "");
	float				widthOffsetPercent		= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float				heightOffsetPercent		= pNewNode.GetFloatProperty("heightOffset", 0.f);


	CTexture* on					= tm->GetTexture(texture_on);
	CTexture* off					= tm->GetTexture(texture_off);
	CTexture* deactivated	= tm->GetTexture(texture_deactivated);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

	checkButton = new CCheckButton(screenResolution.y,screenResolution.x, h, w, Math::Vect2f(posx,posy), isOn, l_literal, heightOffset, widthOffset, visible, activated);
	checkButton->SetName(name);
	checkButton->SetTextures(on, off, deactivated);
	checkButton->SetOnCheckOnAction(OnCheckOn);
	checkButton->SetOnCheckOffAction(OnCheckOff);
	checkButton->SetOnLoadValueAction(OnLoadValue);
	checkButton->SetOnSaveValueAction(OnSaveValue);
	checkButton->SetOnOverAction(OnOverButton);
}

void CWindows::LoadSlider (CSlider* slider, CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution, CTextureManager* tm)
{
	//<Slider	name="debug_fps" posx="0" posy="2" height="10" width="10" visible="true" active="true" value="0.5" buttonH="10" buttonW="10"
	// button_normal="blabla" button_over="bla" button_clicked"bla" button_deactivated="bla" quad="bla"
	// OnOverAction="" OnClickedAction="" OnChangeValue="" OnSaveValue="blabl" OnLoadValue="" Literal="blabla" widthOffset="" heightOffset=""/>

	std::string name								= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float 			posx								= pNewNode.GetFloatProperty("posx", 0.f);
	float 			posy								= pNewNode.GetFloatProperty("posy", 0.f);
	float 			w										= pNewNode.GetFloatProperty("width", 50.f);
	float 			h										= pNewNode.GetFloatProperty("height", 50.f);
	bool 				visible							= pNewNode.GetBoolProperty("visible", true);
	bool 				activated						= pNewNode.GetBoolProperty("active", true);
	float 			value								= pNewNode.GetBoolProperty("value", 0.f);
	float 			buttonH							= pNewNode.GetFloatProperty("buttonH", 10.f);
	float 			buttonW							= pNewNode.GetFloatProperty("buttonW", 10.f);
	std::string button_normal				= pNewNode.GetPszProperty("button_normal", "");
	std::string button_over					= pNewNode.GetPszProperty("button_over", "");
	std::string button_clicked			= pNewNode.GetPszProperty("button_clicked", "");
	std::string button_deactivated	= pNewNode.GetPszProperty("button_deactivated", "");
	std::string quad								= pNewNode.GetPszProperty("quad", "");
	std::string OnChangeValue				= pNewNode.GetPszProperty("OnChangeValue", "");
	std::string OnClickedAction			= pNewNode.GetPszProperty("OnClickedAction", "");
	std::string OnOverAction				= pNewNode.GetPszProperty("OnOverAction", "");
	std::string OnSaveValue					= pNewNode.GetPszProperty("OnSaveValue", "");
	std::string OnLoadValue					= pNewNode.GetPszProperty("OnLoadValue", "");
	std::string l_literal						= pNewNode.GetPszProperty("Literal", "");
	float				widthOffsetPercent	= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float				heightOffsetPercent	= pNewNode.GetFloatProperty("heightOffset", 0.f);

	CTexture* normal	= tm->GetTexture(button_normal);
	CTexture* over		= tm->GetTexture(button_over);
	CTexture* deac		= tm->GetTexture(button_deactivated);
	CTexture* clicked	= tm->GetTexture(button_clicked);
	CTexture* back		= tm->GetTexture(quad);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

	slider = new CSlider(	screenResolution.y, screenResolution.x, h, w, Math::Vect2f(posx,posy), buttonW, buttonH, value, 
												l_literal, heightOffset, widthOffset, visible, activated);
	slider->SetName(name);
	slider->SetButtonTextures(normal, over, clicked, deac);
	slider->SetBackGroundTexture(back);
	slider->SetOnChangeValueAction(OnChangeValue);
	slider->SetOnClickedAction(OnClickedAction);
	slider->SetOnOverAction(OnOverAction);
	slider->SetOnLoadValueAction(OnLoadValue);
	slider->SetOnSaveValueAction(OnSaveValue);
}

void	CWindows::LoadDialogBox			(CDialogBox* dialogBox, CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution, CTextureManager* tm)
{
	//<DialogBox name="debug_fps" posx="0" posy="2" height="10" width="10" visible="true" active="true" buttonH="10" buttonW="10"
	// buttonClose_normal="blabla" buttonClose_over="bla" buttonClose_clicked"bla"  buttonClose_deactivated"bla" buttonMove_normal="blabla" buttonMove_over="bla"
	// buttonMove_clicked"bla" buttonMove_deactivated"bla" quad="bla" Literal="blabla" widthOffset="" heightOffset=""/>

	std::string name										= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float 			posx										= pNewNode.GetFloatProperty("posx", 0.f);
	float 			posy										= pNewNode.GetFloatProperty("posy", 0.f);
	float 			w												= pNewNode.GetFloatProperty("width", 50.f);
	float 			h												= pNewNode.GetFloatProperty("height", 50.f);
	bool 				visible									= pNewNode.GetBoolProperty("visible", true);
	bool 				activated								= pNewNode.GetBoolProperty("active", true);
	float 			buttonH									= pNewNode.GetFloatProperty("buttonH", 10.f);
	float 			buttonW									= pNewNode.GetFloatProperty("buttonW", 10.f);
	std::string buttonClose_normal			= pNewNode.GetPszProperty("buttonClose_normal", "");
	std::string buttonClose_over				= pNewNode.GetPszProperty("buttonClose_over", "");
	std::string buttonClose_clicked			= pNewNode.GetPszProperty("buttonClose_clicked", "");
	std::string buttonClose_deactivated	= pNewNode.GetPszProperty("buttonClose_deactivated", "");
	std::string buttonMove_normal				= pNewNode.GetPszProperty("buttonMove_normal", "");
	std::string buttonMove_over					= pNewNode.GetPszProperty("buttonMove_over", "");
	std::string buttonMove_clicked			= pNewNode.GetPszProperty("buttonMove_clicked", "");
	std::string buttonMove_deactivated	= pNewNode.GetPszProperty("buttonMove_deactivated", "");
	std::string quad										= pNewNode.GetPszProperty("quad");
	std::string l_literal								= pNewNode.GetPszProperty("Literal", "");
	float				widthOffsetPercent			= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float				heightOffsetPercent			= pNewNode.GetFloatProperty("heightOffset", 0.f);

	CTexture* Close_normal			= tm->GetTexture(buttonClose_normal);
	CTexture* Close_over				= tm->GetTexture(buttonClose_over);
	CTexture* Close_clicked			= tm->GetTexture(buttonClose_clicked);
	CTexture* Close_deactivated	= tm->GetTexture(buttonClose_deactivated);
	CTexture* Move_normal				= tm->GetTexture(buttonMove_normal);
	CTexture* Move_over					= tm->GetTexture(buttonMove_over);
	CTexture* Move_clicked			= tm->GetTexture(buttonMove_clicked);
	CTexture* Move_deactivated	= tm->GetTexture(buttonMove_deactivated);
	CTexture* back							= tm->GetTexture(quad);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

	dialogBox = new CDialogBox(	screenResolution.y,screenResolution.x, h, w, Math::Vect2f(posx,posy), buttonW, buttonH, l_literal, 
															heightOffset, widthOffset, visible, activated);
	dialogBox->SetName(name);
	dialogBox->SetCloseButtonTextures(Close_normal, Close_over, Close_clicked, Close_deactivated);
	dialogBox->SetMoveButtonTextures(Move_normal, Move_over, Move_clicked, Move_deactivated);
	dialogBox->SetDialogTexture(back);
}

void	CWindows::LoadEditableTextBox (CEditableTextBox* editableTextBox, CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution, CTextureManager* tm)
{
	//<EditableTextBox name="debug_fps" posx="0" posy="2" height="10" width="10" visible="true" active="true" 
	// texture_quad="blabla" id_font="0" color_font_r="0" color_font_g="0" color_font_b="0"
	// buffer="" OnSaveValue="blabla" OnLoadValue="blabla" Literal="blabla" widthOffset="" heightOffset=""
	// Literal="blabla" widthOffset="" heightOffset=""/>

	std::string name								= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float 			posx								= pNewNode.GetFloatProperty("posx", 0.f);
	float 			posy								= pNewNode.GetFloatProperty("posy", 0.f);
	float 			w										= pNewNode.GetFloatProperty("width", 50.f);
	float 			h										= pNewNode.GetFloatProperty("height", 50.f);
	bool 				visible							= pNewNode.GetBoolProperty("visible", true);
	bool 				activated						= pNewNode.GetBoolProperty("active", true);
	std::string quad								= pNewNode.GetPszProperty("texture_quad", "");
	std::string OnSaveValue					= pNewNode.GetPszProperty("OnSaveValue", "");
	std::string OnLoadValue					= pNewNode.GetPszProperty("OnLoadValue", "");
	uint32			idFont							= pNewNode.GetIntProperty("id_font", 0);
	float				color_font_r				= pNewNode.GetFloatProperty("color_font_r", 0.f);
	float				color_font_g				= pNewNode.GetFloatProperty("color_font_g", 0.f);
	float				color_font_b				= pNewNode.GetFloatProperty("color_font_b", 0.f);
	std::string buffer							= pNewNode.GetPszProperty("buffer", "");
	std::string l_literal						= pNewNode.GetPszProperty("Literal", "");
	float				widthOffsetPercent	= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float				heightOffsetPercent	= pNewNode.GetFloatProperty("heightOffset", 0.f);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

	CTexture* quad_texture	= tm->GetTexture(quad);

	editableTextBox = new CEditableTextBox(	screenResolution.y,screenResolution.x,h, w, Math::Vect2f(posx,posy),
																					Math::CColor(color_font_r,color_font_g,color_font_b),idFont, l_literal,
																					heightOffset, widthOffset, visible, activated);
	editableTextBox->SetName(name);
	editableTextBox->SetBackGroundTexture(quad_texture);
	editableTextBox->SetBuffer(buffer);
	editableTextBox->SetOnLoadValueAction(OnSaveValue);
	editableTextBox->SetOnSaveValueAction(OnLoadValue);
}


void	CWindows::LoadRadioBox(CRadioBox* radioBox, CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution, CTextureManager* tm)
{
	//<RadioBox name="pepito" posx="0" posy="2" height="10" width="10" default_checkButton="bla" visible="true" active="true" texture_back=""
	// columns="3" rows="3" OnCheckOn="blabla" OnCheckOff="blabla" OnOverButton="blabla"
	// OnSaveValue="blabl" OnLoadValue=""	Literal="blabla" widthOffset="" heightOffset=""/>
	//	<texture name="q3dm1" on="gui_q3dm1_on.jpg" off="gui_q3dm1_off.jpg"  deactivated="gui_q3dm1_on.jpg" />
	//	<texture name="q3dm2" on="gui_q3dm2_on.jpg" off="gui_q3dm2_off.jpg" deactivated="gui_q3dm2_on.jpg" />
	//  ...
	//</RadioBox>


	std::string name								= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float 			posx								= pNewNode.GetFloatProperty("posx", 0.f);
	float 			posy								= pNewNode.GetFloatProperty("posy", 0.f);
	float 			w										= pNewNode.GetFloatProperty("width", 50.f);
	float 			h										= pNewNode.GetFloatProperty("height", 50.f);
	bool 				visible							= pNewNode.GetBoolProperty("visible", true);
	bool 				activated						= pNewNode.GetBoolProperty("active", true);
	std::string default_checkButton = pNewNode.GetPszProperty("default_checkButton", "");
	std::string texture_back				= pNewNode.GetPszProperty("texture_back", "");
	uint32			columns							= pNewNode.GetIntProperty("columns", 0);
	uint32			rows								= pNewNode.GetIntProperty("rows", 0);
	std::string OnCheckOn						= pNewNode.GetPszProperty("OnCheckOn", "");
	std::string OnCheckOff					= pNewNode.GetPszProperty("OnCheckOff", "");
	std::string OnOverButton				= pNewNode.GetPszProperty("OnOverButton", "");
	std::string OnSaveValue					= pNewNode.GetPszProperty("OnSaveValue", "");
	std::string OnLoadValue					= pNewNode.GetPszProperty("OnLoadValue", "");
	std::string l_literal						= pNewNode.GetPszProperty("Literal", "");
	float				widthOffsetPercent	= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float				heightOffsetPercent	= pNewNode.GetFloatProperty("heightOffset", 0.f);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

	radioBox = new CRadioBox(	screenResolution.y,screenResolution.x,h, w, Math::Vect2f(posx,posy), columns, rows, default_checkButton, 
														l_literal, heightOffset, widthOffset, visible, activated);
	if (texture_back!="")
	{
		CTexture* quad	= tm->GetTexture(texture_back);
		radioBox->SetTextureBack(quad);
	}
	radioBox->SetName(name);
	radioBox->SetOnLoadValueAction(OnLoadValue);
	radioBox->SetOnSaveValueAction(OnSaveValue);
	radioBox->SetCheckButtonActions(OnCheckOn, OnCheckOff, OnOverButton);
	int count = pNewNode.GetNumChildren();
	for (int j = 0; j < count; ++j)
	{
		CXMLTreeNode pTexture = pNewNode(j);
		std::string tagName = pTexture.GetName();
		if (tagName.compare("texture")==0)
		{
			std::string name				= pTexture.GetPszProperty("name");
			std::string on					= pTexture.GetPszProperty("on");
			std::string off					= pTexture.GetPszProperty("off");
			std::string deactivated = pTexture.GetPszProperty("deactivated");

			CTexture* texture_on					= tm->GetTexture(on);
			CTexture* texture_off					= tm->GetTexture(off);
			CTexture* texture_deactivated	= tm->GetTexture(deactivated);
			radioBox->SetCheckButton(name, texture_on, texture_off, texture_deactivated);
		}
	}
}

void	CWindows::_LoadImage(CImage* image, CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution, CTextureManager* tm)
{
	//<Image	name="imageRoomSelected_gameserver2" posx="35" posy="20" height="40" width="30" visible="true" active="true" 
	//default="q3dm1"	isAnimated="true", time="0" loop="true" OnSaveValue="blabla"  OnLoadValue="blabla"  Literal="blabla"
	//widthOffset="" heightOffset="" flip="" Literal="blabla" widthOffset="" heightOffset=""/>
	//	<texture name="q3dm1" texture="gui_q3dm1_off.jpg"/>
	//	<texture name="q3dm2" texture="gui_q3dm2_off.jpg"/>
	//	...
	//</Image>
	std::string name								= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float 			posx								= pNewNode.GetFloatProperty("posx", 0.f);
	float 			posy								= pNewNode.GetFloatProperty("posy", 0.f);
	float 			w										= pNewNode.GetFloatProperty("width", 50.f);
	float 			h										= pNewNode.GetFloatProperty("height", 50.f);
	bool 				visible							= pNewNode.GetBoolProperty("visible", true);
	bool 				activated						= pNewNode.GetBoolProperty("active", true);
	std::string default_image				= pNewNode.GetPszProperty("default", "");
	std::string OnSaveValue					= pNewNode.GetPszProperty("OnSaveValue", "");
	std::string OnLoadValue					= pNewNode.GetPszProperty("OnLoadValue", "");
	std::string flip								= pNewNode.GetPszProperty("flip", "");
	bool				backGround					= pNewNode.GetBoolProperty("backGround", false);
	std::string l_literal						= pNewNode.GetPszProperty("Literal", "");
	float				widthOffsetPercent	= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float				heightOffsetPercent	= pNewNode.GetFloatProperty("heightOffset", 0.f);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

	image = new CImage(	screenResolution.y,screenResolution.x, h, w, Math::Vect2f(posx,posy),
											l_literal, heightOffset, widthOffset, visible, activated);
	image->SetName(name);
	image->SetActiveTexture(default_image);

	image->SetOnLoadValueAction(OnLoadValue);
	image->SetOnSaveValueAction(OnSaveValue);
	image->SetBackGround(backGround);
	if( flip.compare("FLIP_X") == 0)
	{
		image->SetFlip(FLIP_X);
	}
	else if (flip.compare("FLIP_Y") == 0)
	{
		image->SetFlip(FLIP_Y);
	}
	else
	{
		image->SetFlip(NONE_FLIP);
	}
	int count = pNewNode.GetNumChildren();
	for (int j = 0; j < count; ++j)
	{
		CXMLTreeNode pTexture = pNewNode(j);
		std::string tagName = pTexture.GetName();
		if (tagName.compare("texture")==0)
		{	
			std::string name				=  pNewNode(j).GetPszProperty("name");
			std::string texture			=  pNewNode(j).GetPszProperty("name_texture");

			CTexture* texture_image		= tm->GetTexture(texture);
			image->SetTexture(texture_image, name);
		}
	}
}


void	CWindows::LoadProgressBar(CProgressBar* progressBar, CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution, CTextureManager* tm)
{
	//<ProgressBar name="progressBar1" posx="200" posy="600" height="10" width="100" visible="true" active="true"
	//	texture_back="hola" texture_bar="hola2"  id_font="0" color_font_r="0" color_font_g="0" color_font_b="0"
	//	Literal="blabla" widthOffset="" heightOffset="" OnComplete="blabla"/>

	std::string name								= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float				posx								= pNewNode.GetFloatProperty("posx", 0.f);
	float				posy								= pNewNode.GetFloatProperty("posy", 0.f);
	float				w										= pNewNode.GetFloatProperty("width", 50.f);
	float				h										= pNewNode.GetFloatProperty("height", 50.f);
	bool				visible							= pNewNode.GetBoolProperty("visible", true);
	bool				activated						= pNewNode.GetBoolProperty("active", true);
	std::string texture_bar					= pNewNode.GetPszProperty("texture_bar", "");
	std::string texture_back				= pNewNode.GetPszProperty("texture_back", "");
	std::string OnComplete					= pNewNode.GetPszProperty("OnComplete", "");
	uint32			idFont							= pNewNode.GetIntProperty("id_font", 0);
	float				color_font_r				= pNewNode.GetFloatProperty("color_font_r", 0.f);
	float				color_font_g				= pNewNode.GetFloatProperty("color_font_g", 0.f);
	float				color_font_b				= pNewNode.GetFloatProperty("color_font_b", 0.f);
	std::string l_literal						= pNewNode.GetPszProperty("Literal", "");
	float				widthOffsetPercent	= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float				heightOffsetPercent	= pNewNode.GetFloatProperty("heightOffset", 0.f);

	CTexture* bar					= tm->GetTexture(texture_bar);
	CTexture* back				= tm->GetTexture(texture_back);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

	progressBar = new CProgressBar(	screenResolution.y,screenResolution.x, h, w, Math::Vect2f(posx, posy), 
																	l_literal, heightOffset, widthOffset, visible, activated);
	progressBar->SetName(name);
	progressBar->SetTextures(back, bar);
	progressBar->SetFont(idFont, Math::CColor(color_font_r,color_font_g,color_font_b));
	progressBar->SetOnComplete(OnComplete);
}

void	CWindows::LoadStaticText(CStaticText* staticText, CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution, CTextureManager* tm)
{
	//<StaticText name="pepito" posx="0" posy="2" literal="blabla" visible="true" active="true"/>
	
	std::string name						= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float 			posx						= pNewNode.GetFloatProperty("posx", 0.f);
	float 			posy						= pNewNode.GetFloatProperty("posy", 0.f);
	float 			w								= pNewNode.GetFloatProperty("width", 50.f);
	float 			h								= pNewNode.GetFloatProperty("height", 50.f);
	bool 				visible					= pNewNode.GetBoolProperty("visible", true);
	bool 				activated				= pNewNode.GetBoolProperty("active", true);
	std::string l_literal				= pNewNode.GetPszProperty("Literal", "");
	
	staticText = new CStaticText(screenResolution.y, screenResolution.x, h, w, Math::Vect2f(posx,posy), l_literal, visible, activated);
	staticText->SetName(name);
}