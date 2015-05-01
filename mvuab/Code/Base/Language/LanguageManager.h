#pragma once
#ifndef INC_LANGUAGE_MANAGER_H_
#define INC_LANGUAGE_MANAGER_H_

#include "Math/Color.h"
#include "Utils/SingletonPattern.h"
#include "Utils\Manager.h"
#include <vector>
#include <map>

struct SLiteral
{
  std::string    m_sFontId;
  Math::CColor   m_cColor;
  std::string    m_value;
};

class CLanguageManager : public CManager
{
public:
  CLanguageManager( CXMLTreeNode& atts );
  virtual ~CLanguageManager( void ) {}
  void Init();
  void Update() {}
  void Render() {}
  void LoadXMLs();
  void SetXmlFile( const std::string& pathFile );
  bool GetLiteral( const std::string& id, SLiteral& sliteral ) const;
  void SetCurrentLanguage( const std::string& id );

private:
  void LoadXML( const std::string& pathFile );
  typedef std::map<std::string, SLiteral>      TLanguage;
  std::vector<std::string>            m_vXML_Files;
  std::map<std::string, TLanguage>    m_Languages;
  std::string                         m_sCurrentLanguage;
};

#endif //INC_LANGUAGE_MANAGER_H_