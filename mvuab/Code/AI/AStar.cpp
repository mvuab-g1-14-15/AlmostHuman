#include "AStar.h"
#include "Node.h"
#include "Graph\Graph.h"

#include "GraphicsManager.h"
#include "Core.h"

CAStar::CAStar()
{
}
  
CAStar::~CAStar()
{
}

void CAStar::Init()
{
  grafo = new CGraph();
  grafo->Parse("Data/graph1.xml");
}

void CAStar::Render()
{
  CGraphicsManager* l_GM = CCore::GetSingletonPtr()->GetGraphicsManager();
  for(unsigned int i=0; i<grafo->GetSize(); ++i)
  {
    Math::Vect3f pos = grafo->GetNodeInfo(i);

    Math::Mat44f m;
    m.Translate(pos);
    l_GM->SetTransform(m);
    l_GM->DrawCube(0.3f);
    m.SetIdentity();
    l_GM->SetTransform(m);

    /*std::vector<Node*>::iterator it = vecinos.begin(),
                                 it_end = vecinos.end();
    for ( ; it != it_end; ++it)
    {
      l_GM->DrawLine(posicion, (*it)->posicion);
    }*/
  }
}

std::vector<Node*>
CAStar::GetPath(Math::Vect3f posicion_inicial, Math::Vect3f posicion_final)
{
  std::vector<Node*> path;
  return path;
}