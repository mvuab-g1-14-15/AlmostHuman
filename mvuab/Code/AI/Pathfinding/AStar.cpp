#include "AStar.h"
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
  m_Graph = new CGraph();
  m_Graph->Parse( "Data/graph1.xml" );
}

void CAStar::Render()
{
  CGraphicsManager* l_GM = CCore::GetSingletonPtr()->GetGraphicsManager();

  for ( unsigned int i = 0; i < m_Graph->GetSize(); ++i )
  {
    Math::Vect3f pos = m_Graph->GetNodeInfo( i );

    Math::Mat44f m;
    m.Translate( pos );
    l_GM->SetTransform( m );
    l_GM->DrawCube( 0.3f );
    m.SetIdentity();
    l_GM->SetTransform( m );

    std::map<unsigned int, unsigned int> l_Arcs = m_Graph->GetArcs( i );
    std::map<unsigned int, unsigned int>::iterator it = l_Arcs.begin(),
                                                   it_end = l_Arcs.end();

    for ( ; it != it_end; ++it )
      l_GM->DrawLine( pos, m_Graph->GetNodeInfo( it->first ) );
  }
}

std::vector<Math::Vect3f>
CAStar::GetPath( Math::Vect3f init_pos, Math::Vect3f final_pos )
{
  std::vector<Math::Vect3f> path;
  path.push_back( init_pos );

  Math::Vect3f first_point = m_Graph->GetNodeInfo( 0 );
  Math::Vect3f last_point = m_Graph->GetNodeInfo( 0 );

  Math::Vect3f l_ActualNode;
  float l_ActualNodeDistance;

  for ( unsigned int i = 1; i < m_Graph->GetSize(); ++i )
  {
    l_ActualNode = m_Graph->GetNodeInfo( i );

    l_ActualNodeDistance = init_pos.Distance( l_ActualNode );

    if ( l_ActualNodeDistance < init_pos.Distance( first_point ) )
      first_point = l_ActualNode;

    l_ActualNodeDistance = final_pos.Distance( l_ActualNode );

    if ( l_ActualNodeDistance < final_pos.Distance( last_point ) )
      last_point = l_ActualNode;
  }

  path.push_back( first_point );

  path.push_back( last_point );
  path.push_back( final_pos );
  return path;
}