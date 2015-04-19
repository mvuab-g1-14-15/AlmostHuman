//----------------------------------------------------------------------------------
// CLanguageManager class
// Author: Enric Vergara
//
// Description:
// Esta clase será la que se encargue de dar un string en el lenguage que este seteado actualmente
//----------------------------------------------------------------------------------
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
    Math::CColor            m_cColor;
    std::string    m_value;
};


class CLanguageManager : public CManager
{
public:
    CLanguageManager();
	CLanguageManager( CXMLTreeNode& atts );
    virtual ~CLanguageManager(void) {/*Nothing*/;}
	void Init();
    void LoadXMLs();
    void SetXmlFile(const std::string& pathFile);
    void SetXmlPaths(const std::vector<std::string>&    a_v_languages );
    bool GetLiteral(const std::string& id, SLiteral& sliteral) const;
    void SetCurrentLanguage(const std::string& id);
	void Update(){}
	void Render(){}
    
private:
    void                        LoadXML                            (const std::string& pathFile);
    
private:
    typedef std::map<std::string,SLiteral>      TLanguage;

    std::vector<std::string>                    m_vXML_Files;
    std::map<std::string, TLanguage>    m_Languages;
    std::string                                                m_sCurrentLanguage;


};

#endif //INC_LANGUAGE_MANAGER_H_