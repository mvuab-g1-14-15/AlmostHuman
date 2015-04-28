//----------------------------------------------------------------------------------
// CSlider class
// Author: Enric Vergara
//
// Description:
// A Image..
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_MAP_H
#define INC_MAP_H

#include "GuiElement\GuiElement.h"

//---Forward Declarations---
class CTexture;
//--------------------------

class CMap: public CGuiElement
{
private:


public:
  CMap( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
        const Math::Vect2f position_percent, std::string lit = "", uint32 textHeightOffset = 0, uint32 textWidthOffset = 0,
        bool isVisible = true, bool isActive = true );

  virtual ~CMap();

  //---------------CGuiElement Interface----------------------
  virtual void  Render();
  virtual void  Update();
  virtual void  OnClickedChild( const std::string& name );
  Math::Vect2f  NormalizePlayerPos( float x, float z );


private:
  CTexture*           m_Map;  // Textura del mapa/radar del escenario
  CTexture*           m_Player;  // Textura del player en el mapa
  CTexture*           m_Camara;  // Textura del player en el mapa
  Math::Vect2f        m_posNPlayer;  // Posición del Player en el mapa/radar
  //Math::Vect2f        m_SizeWorld3D;  // Zona del mapa que estamos cubriendo
};

#endif //INC_IMAGE_H