#ifndef LIGHT_H
#define LIGHT_H

#include "Object3D.h"
#include "Math/Color.h"

#include <string>

class CLight : public CObject3D
{
public:
	enum TLightType
	{
		OMNI=0,
		DIRECTIONAL,
		SPOT
	};
protected:
	Math::CColor m_Color;
	TLightType m_Type;
	std::string m_Name;
	bool m_RenderShadows;
	float m_Intensity;
	float m_StartRangeAttenuation;
	float m_EndRangeAttenuation;
	static TLightType GetLightTypeByName(const std::string &StrLightType);
public:
	CLight();
	virtual ~CLight();
	void SetName(const std::string &Name);
	const std::string &GetName();
	void SetIntensity(const float intensity);
	float GetIntensity() const;
	void SetColor(const Math::CColor &Color);
	const Math::CColor & GetColor() const;
	void SetStartRangeAttenuation(const float StartRangeAttenuation);
	float GetStartRangeAttenuation() const;
	void SetEndRangeAttenuation(const float EndRangeAttenuation);
	float GetEndRangeAttenuation() const;
	bool RenderShadows() const;
	void SetType(const TLightType Type);
	TLightType GetType() const;
	virtual void Render();
};

#endif // LIGHT_H
