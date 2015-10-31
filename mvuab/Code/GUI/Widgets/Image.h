//----------------------------------------------------------------------------------
// CSlider class
// Author: Enric Vergara
//
// Description:
// A Image..
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_IMAGE_H
#define INC_IMAGE_H

#include "GraphicsManager.h"
#include "GuiElement\GuiElement.h"
#include "Math/Color.h"
#include "Utils/TemplatedVectorMapManager.h"
#include "Texture/Texture.h"
#include <map>

class CImage: public CGuiElement
{
private:
  typedef CTemplatedVectorMapManager<CTexture>                        TTexturesContainer;
  typedef std::vector<CTexture*>::iterator                            TTexturesIterator;

public:
  CImage( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution );

  virtual ~CImage()
  {
    m_Textures.Clear();
  }

  //---------------CGuiElement Interface----------------------
  virtual void    Render();
  virtual void    Update();
  virtual void    OnClickedChild( const std::string& name )
  {
    /*NOTHING*/;
  }


  //---------------CImage Interface---------------------------
  void                    AddTexture( const CXMLTreeNode& aNode );
  void                    SetActiveTexture( const std::string& inName )
  {
    m_sActiveTexture = inName;
  }
  std::string&    GetActiveTexture()
  {
    return m_sActiveTexture;
  }
  void                    PlayAnimation( float timePerImage, bool loop );
  void                    SetFlip( CGraphicsManager::ETypeFlip flip )
  {
    m_eFlip = flip;
  }
  CGraphicsManager::ETypeFlip            GetFlip() const
  {
    return m_eFlip;
  }

  bool                    IsQuadrant() const
  {
    return m_bIsQuadrant;
  }

  void                    SetQuadrant( bool flag )
  {
    m_bIsQuadrant = flag;
  }

  void                    SetBackGround( bool flag )
  {
    m_bIsBackGround = flag;
  }

  void SetCurrentTextureIdx( uint32 aIdx );
private:
  TTexturesContainer          m_Textures;
  TTexturesIterator           m_itVecTextures;
  Math::CColor                m_Color;
  std::string                 m_sActiveTexture;
  bool                        m_bAnimated;
  uint32                      mCurrentTextureId;
  bool                        m_bLoop;
  float                       m_fTimePerImage;
  float                       m_fCounter;
  CGraphicsManager::ETypeFlip m_eFlip;
  bool                        m_bIsQuadrant;
  bool                        m_bIsBackGround;
  bool                        mUseTextureSize;
};

#endif //INC_IMAGE_H