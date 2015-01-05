#include "LightManager.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "OmniLight.h"
#include "SpotLight.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"

CLightManager::CLightManager()
{
}

CLightManager::~CLightManager()
{
}

void CLightManager::Load(const std::string &FileName)
{
	CXMLTreeNode newFile;
    CXMLTreeNode m;

    if (!newFile.LoadFile(FileName.c_str()))
    {
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR,"CLightManager::Load --> Error loading XML %s.",FileName.c_str());
        return;
    }

    m = newFile["lights"];

    if (!m.Exists())
    {
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR,"CLightManager::Load --> Error reading %s, lights no existeix.",FileName.c_str());
        return;
    }

    for(int i=0;i<m.GetNumChildren();++i)
    {
        const std::string &l_TagName=m(i).GetName();
		if(l_TagName=="light")
		{
			std::string l_Type = m(i).GetPszProperty("type","");
			if(l_Type=="directional")
			{
				CDirectionalLight* l_Light = new CDirectionalLight();

				Math::Vect3f l_Direction = m(i).GetVect3fProperty("dir",Math::Vect3f(0,0,0));
				l_Light->SetDirection(l_Direction);

				std::string l_Name = m(i).GetPszProperty("name","");
				l_Light->SetName(l_Name);

				Math::CColor l_Color = Math::colWHITE;
				l_Light->SetColor(l_Color);

				Math::Vect3f l_Position = m(i).GetVect3fProperty("pos",Math::Vect3f(0,0,0));
				l_Light->SetPosition(l_Position);

				float l_Yaw = m(i).GetFloatProperty("yaw", 0);
				l_Light->SetYaw(l_Yaw);

				float l_Pitch = m(i).GetFloatProperty("pitch", 0);
				l_Light->SetPitch(l_Pitch);

				float l_Roll = m(i).GetFloatProperty("roll", 0);
				l_Light->SetRoll(l_Roll);

				Math::Vect3f l_Scale = m(i).GetVect3fProperty("scale",Math::Vect3f(0,0,0));
				l_Light->SetScale(l_Scale);

				float l_StartRangeAttenuation = m(i).GetFloatProperty("att_start_range", 0);
				l_Light->SetStartRangeAttenuation(l_StartRangeAttenuation);

				float l_EndRangeAttenuation = m(i).GetFloatProperty("att_end_range", 0);
				l_Light->SetEndRangeAttenuation(l_EndRangeAttenuation);
				l_Light->SetType(CLight::DIRECTIONAL);

				AddResource(l_Name, l_Light);
			}
			else if(l_Type=="omni")
			{
				COmniLight* l_Light = new COmniLight();

				std::string l_Name = m(i).GetPszProperty("name","");
				l_Light->SetName(l_Name);

				Math::CColor l_Color = Math::colWHITE;
				l_Light->SetColor(l_Color);

				Math::Vect3f l_Position = m(i).GetVect3fProperty("pos",Math::Vect3f(0,0,0));
				l_Light->SetPosition(l_Position);

				float l_Yaw = m(i).GetFloatProperty("yaw", 0);
				l_Light->SetYaw(l_Yaw);

				float l_Pitch = m(i).GetFloatProperty("pitch", 0);
				l_Light->SetPitch(l_Pitch);

				float l_Roll = m(i).GetFloatProperty("roll", 0);
				l_Light->SetRoll(l_Roll);

				Math::Vect3f l_Scale = m(i).GetVect3fProperty("scale",Math::Vect3f(0,0,0));
				l_Light->SetScale(l_Scale);

				float l_StartRangeAttenuation = m(i).GetFloatProperty("att_start_range", 0);
				l_Light->SetStartRangeAttenuation(l_StartRangeAttenuation);

				float l_EndRangeAttenuation = m(i).GetFloatProperty("att_end_range", 0);
				l_Light->SetEndRangeAttenuation(l_EndRangeAttenuation);
				l_Light->SetType(CLight::OMNI);

				AddResource(l_Name, l_Light);
			}
			else if(l_Type=="spot")
			{
				CSpotLight* l_Light = new CSpotLight();

				Math::Vect3f l_Direction = m(i).GetVect3fProperty("dir",Math::Vect3f(0,0,0));
				l_Light->SetDirection(l_Direction);

				float l_Angle = m(i).GetFloatProperty("angle", 0);
				l_Light->SetAngle(l_Angle);

				float l_FallOff = m(i).GetFloatProperty("falloff", 0);
				l_Light->SetFallOff(l_FallOff);

				std::string l_Name = m(i).GetPszProperty("name","");
				l_Light->SetName(l_Name);

				Math::CColor l_Color = Math::colWHITE;
				l_Light->SetColor(l_Color);

				Math::Vect3f l_Position = m(i).GetVect3fProperty("pos",Math::Vect3f(0,0,0));
				l_Light->SetPosition(l_Position);

				float l_Yaw = m(i).GetFloatProperty("yaw", 0);
				l_Light->SetYaw(l_Yaw);

				float l_Pitch = m(i).GetFloatProperty("pitch", 0);
				l_Light->SetPitch(l_Pitch);

				float l_Roll = m(i).GetFloatProperty("roll", 0);
				l_Light->SetRoll(l_Roll);

				Math::Vect3f l_Scale = m(i).GetVect3fProperty("scale",Math::Vect3f(0,0,0));
				l_Light->SetScale(l_Scale);

				float l_StartRangeAttenuation = m(i).GetFloatProperty("att_start_range", 0);
				l_Light->SetStartRangeAttenuation(l_StartRangeAttenuation);

				float l_EndRangeAttenuation = m(i).GetFloatProperty("att_end_range", 0);
				l_Light->SetEndRangeAttenuation(l_EndRangeAttenuation);
				l_Light->SetType(CLight::SPOT);

				AddResource(l_Name, l_Light);
			}
		}
    }
}

void CLightManager::Render()
{
}