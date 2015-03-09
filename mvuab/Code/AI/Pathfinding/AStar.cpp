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

  unsigned int first_point = 0,
               last_point = 0;

  float l_ActualNodeDistance;

  for ( unsigned int i = 1; i < m_Graph->GetSize(); ++i )
  {
    l_ActualNodeDistance = init_pos.Distance( m_Graph->GetNodeInfo( i ) );

    if ( l_ActualNodeDistance < init_pos.Distance( m_Graph->GetNodeInfo( first_point ) ) )
      first_point = i;

    l_ActualNodeDistance = final_pos.Distance( m_Graph->GetNodeInfo( i ) );

    if ( l_ActualNodeDistance < final_pos.Distance( m_Graph->GetNodeInfo( last_point ) ) )
      last_point = i;
  }

  path.push_back( m_Graph->GetNodeInfo( first_point ) );

  std::vector<unsigned int> l_Visited;
  unsigned int l_ActualNode = first_point;

  bool l_PathFound = false;

  float h = 0;
  float g = 0;

  while ( l_Visited.size() < m_Graph->GetSize() && !l_PathFound )
  {
    std::map<unsigned int, unsigned int> l_Neighbours;
    l_Neighbours = m_Graph->GetArcs( l_ActualNode );

    std::map<unsigned int, unsigned int>::iterator it = l_Neighbours.begin(),
                                                   it_end = l_Neighbours.end();
    float l_F = 9999999999;
    unsigned int l_NextNode;

    for ( ; it != it_end; ++it )
    {
      float l_H = h + m_Graph->GetNodeInfo( l_ActualNode ).Distance( m_Graph->GetNodeInfo( it->first ) );
      float l_G = m_Graph->GetNodeInfo( l_ActualNode ).Distance( final_pos );

      if ( l_H + l_G < l_F )
      {
        l_F = l_H + l_G;
        l_NextNode = it->first;
      }
    }

    l_Visited.push_back( l_ActualNode );
    l_ActualNode = l_NextNode;

    if ( l_ActualNode == last_point )
      l_PathFound = true;
  }

  for ( unsigned int i = 1; i < l_Visited.size() - 1; ++i )
    path.push_back( m_Graph->GetNodeInfo( i ) );

  path.push_back( m_Graph->GetNodeInfo( last_point ) );

  path.push_back( final_pos );
  return path;
}