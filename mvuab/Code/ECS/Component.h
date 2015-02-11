#ifndef _COMPONENT_H
#define _COMPONENT_H

enum ComponentType
{
	TRANSFORM = 0
};

class CComponent
{
public:
	CComponent( ComponentType );
	virtual ~CComponent(void);

	const ComponentType GetComponentType(void) { return m_Type; }

private:
	const ComponentType m_Type;
};

#endif //_COMPONENT_H