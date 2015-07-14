#pragma once

#ifndef _XML_TREE_NODE_H_
#define _XML_TREE_NODE_H_

// Includes
#include "libxml/parser.h"
#include "libxml/xmlwriter.h"
#include <string>
#include "Math/Vector4.h"
#include "Math/Color.h"

//Class
class CXMLTreeNode
{
public:
  CXMLTreeNode();
  virtual ~CXMLTreeNode();

  void Done();
  bool IsOk() const
  {
    return m_bIsOk;
  }

  // reading methods
  bool                          LoadFile( const char* _pszFileName );
  bool                          LoadAndFindNode( const char* aFilename, const char* aNodeTag, CXMLTreeNode& aNode );
  const char*                   GetName() const;
  template<typename T> const T  GetAttribute( const char* aAttName, const T& aDefaultAttValue ) const;
  template<typename T> T*       GetAttribute( const char* aAttName ) const;

  // writing methods
  template<typename T> const bool     AddAttribute( const char* aAttName, const T& aValue )const;

  bool Exists()
  {
    return m_pNode != NULL;
  }

  bool ExistsKey( const char* _pszKey );


  uint32 GetNumChildren() const;

  CXMLTreeNode operator[]( const char* _pszKey ) const;
  CXMLTreeNode operator()( int _iIndex ) const;

  CXMLTreeNode GetChildren( int _iIndex ) const;
  CXMLTreeNode GetNode( const char* _pszKey ) const;

  // -----------------------
  // Write functions
  // -----------------------
  bool                                StartNewFile( const char* _pszFileName );
  void                                EndNewFile();
  bool                                WriteComment( const char* _pszComment );
  bool                                StartElement( const char* _pszKey );
  bool                                EndElement();

  // To write keywords to xml file
  bool                                WritePszKeyword( const char* _pszKey, const char* _pszValue );
  bool                                WriteIntKeyword( const char* _pszKey, int _iValue );
  bool                                WriteFloatKeyword( const char* _pszKey, float32 _fValue );
  bool                                WriteBoolKeyword( const char* _pszKey, bool _bValue );


private:
  void                        Release();
  CXMLTreeNode                GetSubTree( const char* _pszKey ) const;
  bool                        _FindSubTree( xmlNodePtr _pNode, const char* _pszKey, CXMLTreeNode& _TreeFound ) const;

  xmlChar*                    GetProperty( const char* _pszKey ) const;
  xmlChar*                    GetKeyword( const char* _pszKey ) const;
  bool                        AddProperty( const char* _pszKey, const char* _Value ) const;
  xmlChar*                    ConvertInput( const char* _pszIn, const char* _pszEncoding );

  // member variables
  bool                m_bIsOk;          // Initialization boolean control
  xmlDocPtr           m_pDoc;           // Pointer to the doc libxml structure
  xmlNodePtr          m_pNode;          // Pointer to the root node libxml structure
  xmlTextWriterPtr    m_pWriter;        // Pointer to the writer libxml structure
  const char*         m_pszFileName;    // Name of the new file to be created
};

#endif