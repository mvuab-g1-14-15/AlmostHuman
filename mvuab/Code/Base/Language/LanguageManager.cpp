#include "Language\LanguageManager.h"
#include "xml/XMLTreeNode.h"
#include "Utils/Defines.h"

CLanguageManager::CLanguageManager()
    : m_sCurrentLanguage("")
{}


void CLanguageManager::LoadXMLs ()
{
    //Tener en cuenta que se puede hacer un reload!
    std::map<std::string, TLanguage>::iterator it_1            = m_Languages.begin();
    std::map<std::string, TLanguage>::iterator itEnd_1    = m_Languages.end();
    while (it_1 != itEnd_1)
    {
        it_1->second.clear();
        ++it_1;
    }
    m_Languages.clear();

    std::vector<std::string>::const_iterator it            = m_vXML_Files.begin();
    std::vector<std::string>::const_iterator itEnd    = m_vXML_Files.end();
    while (it != itEnd)
    {
        std::string xmlPath = *it;
        LoadXML(xmlPath);
        ++it;
    }
}

void CLanguageManager::LoadXML(const std::string& pathFile)
{
    CXMLTreeNode parser;
    if (!parser.LoadFile(pathFile.c_str()))
    {
        std::string msg_error = "LanguageManager::LoadXML->Error al intentar leer el archivo de lenguaje: " + pathFile;
        LOG_ERROR_APPLICATION(msg_error.c_str());
    }
    LOG_INFO_APPLICATION( "LanguageManager::LoadXML-> Parseando fichero de lenguaje: %s", pathFile.c_str());

    CXMLTreeNode  m = parser["Language"];
    std::string id_language    = m.GetPszProperty("id");
    TLanguage language;
    if (m.Exists())
    {
        int count = m.GetNumChildren();
        for (int i = 0; i < count; ++i)
        {
            //for each literal:
            SLiteral l_literal;

            std::string id            = m(i).GetPszProperty("id");
            l_literal.m_sFontId    = m(i).GetPszProperty("font");
            Math::Vect4f vecColor            = m(i).GetVect4fProperty("color", Math::Vect4f(0.f, 0.f, 0.f, 0.f));
            l_literal.m_value        = m(i).GetPszISOProperty("value", "nothing");
            l_literal.m_cColor    = Math::CColor(vecColor.x, vecColor.y, vecColor.z, vecColor.w);
            language.insert(std::pair<std::string, SLiteral>(id, l_literal));
            LOG_INFO_APPLICATION( "LanguageManager::LoadXML-> Añadido literal(%s,%s,[%f,%f,%f,%f],%s)", id.c_str(),
                                  l_literal.m_sFontId.c_str(), vecColor.x, vecColor.y, vecColor.z, vecColor.w, l_literal.m_value.c_str());
        }
    }
    if (m_Languages.find(id_language) != m_Languages.end())
    {
        LOG_WARNING_APPLICATION("LanguageManager::LoadXML-> EYa se ha registrado un language con identificador %s",
                                id_language.c_str());
    }
    else
    {
        m_Languages.insert(std::pair<std::string, TLanguage>(id_language, language));
    }

}

void CLanguageManager::SetXmlFile (const std::string& pathFile)
{
    std::vector<std::string>::iterator it            = m_vXML_Files.begin();
    std::vector<std::string>::iterator itEnd    = m_vXML_Files.end();
    bool exist = false;
    while(it != itEnd)
    {
        std::string xmlFile = *it;
        if (xmlFile == pathFile)
        {
            exist = true;
            break;
        }
        ++it;
    }

    if (!exist)
    {
        m_vXML_Files.push_back(pathFile);
        LOG_INFO_APPLICATION("LanguageManager::SetXmlFile-> Se ha añadido el xml: %s", pathFile.c_str());
    }

}

bool CLanguageManager::GetLiteral (const std::string& id, SLiteral& sliteral) const
{
    std::map<std::string, TLanguage>::const_iterator it = m_Languages.find(m_sCurrentLanguage);
    if (it != m_Languages.end())
    {
        TLanguage currentLanguage = it->second;
        TLanguage::const_iterator it_aux = currentLanguage.find(id);
        if (it_aux != currentLanguage.end())
        {
            sliteral = it_aux->second;
            return true;
        }
    }

    return false;
}

void CLanguageManager::SetCurrentLanguage (const std::string& id)
{
    std::map<std::string, TLanguage>::iterator it = m_Languages.find(id);
    if (it != m_Languages.end())
    {
        m_sCurrentLanguage = id;
        LOG_INFO_APPLICATION( "LanguageManager::SetCurrentLanguage-> Se ha seteado como current el language %s", id.c_str());
    }
    else
    {
        LOG_INFO_APPLICATION( "LanguageManager::SetCurrentLanguage-> El language %s no esta registrado", id.c_str());
    }
}

void CLanguageManager::SetXmlPaths(const std::vector<std::string>&    a_v_languages )
{
    m_vXML_Files.insert( m_vXML_Files.end(), a_v_languages.begin(), a_v_languages.end() );
}