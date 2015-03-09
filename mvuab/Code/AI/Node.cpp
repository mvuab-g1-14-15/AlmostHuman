#include "Node.h"
#include "GraphicsManager.h"
#include "Core.h"
#include "Math\Matrix44.h"

void Node::Render()
{
  CGraphicsManager* l_GM = CCore::GetSingletonPtr()->GetGraphicsManager();
  Math::Mat44f m;
  m.Translate(posicion);
  l_GM->SetTransform(m);
  l_GM->DrawCube(0.3f);
  m.SetIdentity();
  l_GM->SetTransform(m);

  std::vector<Node*>::iterator it = vecinos.begin(),
                               it_end = vecinos.end();
  for ( ; it != it_end; ++it)
  {
    l_GM->DrawLine(posicion, (*it)->posicion);
  }
}