#include "GraphicsManager.h"
#include "RenderableObject\RenderableObject.h"

#include <string>

class CStaticMesh;

class CAnimatedInstanceModel : public CRenderableObject
{
private:
	CStaticMesh *m_StaticMesh;
public:
	CAnimatedInstanceModel(const std::string &Name, const std::string &CoreName);
	~CAnimatedInstanceModel();
	void Render(CGraphicsManager *GM);
};

