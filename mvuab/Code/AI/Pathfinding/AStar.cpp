#include "AStar.h"
#include "Graph\Graph.h"

#include "GraphicsManager.h"

#include "EngineManagers.h"
#include <algorithm>

CAStar::CAStar()
{
}

CAStar::~CAStar()
{
    CHECKED_DELETE( m_Graph );
}

void CAStar::Init()
{
    m_Graph = new CGraph();
    m_Graph->Parse( "Data/graph.xml" );
}

void CAStar::Render()
{
    CGraphicsManager* l_GM = GraphicsInstance;

    if ( m_Graph->GetSize() == 0 )
    { return; }

    Math::Vect3f lCurrentPoint = m_Graph->GetNodeInfo( 0 );
    l_GM->DrawCube( lCurrentPoint, 0.1f, Math::colORANGE );

    for ( unsigned int i = 1; i < m_Graph->GetSize(); ++i )
    {
        Math::Vect3f pos( m_Graph->GetNodeInfo( i ) );
        l_GM->DrawCube( pos, 0.1f, Math::colORANGE );
        std::map<unsigned int, unsigned int> l_Arcs = m_Graph->GetArcs( i );
        std::map<unsigned int, unsigned int>::iterator it = l_Arcs.begin(),
                                                       it_end = l_Arcs.end();

        if ( i == 1 )
        {
            for ( ; it != it_end; ++it )
            { l_GM->DrawLine( pos, m_Graph->GetNodeInfo( it->first ), Math::colGREEN ); }
        }
    }
}

/*
    function A*(start,goal)
    closedset := the empty set    // The set of nodes already evaluated.
    openset := {start}    // The set of tentative nodes to be evaluated, initially containing the start node
    came_from := the empty map    // The map of navigated nodes.

    g_score[start] := 0    // Cost from start along best known path.
    // Estimated total cost from start to goal through y.
    f_score[start] := g_score[start] + heuristic_cost_estimate(start, goal)

    while openset is not empty
      current := the node in openset having the lowest f_score[] value
      if current = goal
        return reconstruct_path(came_from, goal)

      remove current from openset
      add current to closedset
      for each neighbor in neighbor_nodes(current)
        if neighbor in closedset
          continue
        tentative_g_score := g_score[current] + dist_between(current,neighbor)

        if neighbor not in openset or tentative_g_score < g_score[neighbor]
          came_from[neighbor] := current
          g_score[neighbor] := tentative_g_score
          f_score[neighbor] := g_score[neighbor] + heuristic_cost_estimate(neighbor, goal)
          if neighbor not in openset
            add neighbor to openset

    return failure

    function reconstruct_path(came_from,current)
    total_path := [current]
    while current in came_from:
      current := came_from[current]
      total_path.append(current)
    return total_path
*/

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
        { first_point = i; }

        l_ActualNodeDistance = final_pos.Distance( m_Graph->GetNodeInfo( i ) );

        if ( l_ActualNodeDistance < final_pos.Distance( m_Graph->GetNodeInfo( last_point ) ) )
        { last_point = i; }
    }

    //path.push_back( m_Graph->GetNodeInfo( first_point ) );
    unsigned int l_ActualNode = first_point;
    std::vector<unsigned int> l_ClosedSet;
    std::vector<unsigned int> l_OpenSet;
    l_OpenSet.push_back( l_ActualNode );
    std::map<unsigned int, unsigned int> l_CameFrom;
    std::map<unsigned int, float> l_GScore;
    std::map<unsigned int, float> l_FScore;
    l_GScore[l_ActualNode] = 0;
    l_FScore[l_ActualNode] = l_GScore[l_ActualNode] + m_Graph->GetNodeInfo( l_ActualNode ).Distance( final_pos );

    while ( l_OpenSet.size() > 0 )
    {
        l_ActualNode = l_OpenSet[0];

        for ( unsigned int i = 1; i < l_OpenSet.size(); ++i )
        {
            if ( l_FScore[l_OpenSet[i]] < l_FScore[l_ActualNode] )
            { l_ActualNode = l_OpenSet[i]; }
        }

        if ( l_ActualNode == last_point )
        {
            std::vector<Math::Vect3f> l_path;
            l_path.push_back( m_Graph->GetNodeInfo( l_ActualNode ) );

            while ( l_CameFrom.find( l_ActualNode ) != l_CameFrom.end() )
            {
                l_ActualNode = l_CameFrom[l_ActualNode];
                l_path.push_back( m_Graph->GetNodeInfo( l_ActualNode ) );
            }

            for ( int i = l_path.size() - 1; i >= 0; --i )
            { path.push_back( l_path[i] ); }

            //path.push_back( m_Graph->GetNodeInfo( last_point ) );
            path.push_back( final_pos );
            return path;
        }

        l_OpenSet.erase( std::remove( l_OpenSet.begin(), l_OpenSet.end(), l_ActualNode ), l_OpenSet.end() );
        l_ClosedSet.push_back( l_ActualNode );
        std::map<unsigned int, unsigned int> l_Neighbours = m_Graph->GetArcs( l_ActualNode );
        std::map<unsigned int, unsigned int>::iterator it = l_Neighbours.begin(),
                                                       it_end = l_Neighbours.end();

        for ( ; it != it_end; ++it )
        {
            unsigned int l_ActualNeighbour = it->first;

            if ( std::find( l_ClosedSet.begin(), l_ClosedSet.end(), l_ActualNeighbour ) != l_ClosedSet.end() )
            { continue; }

            float l_ActualGScore = l_GScore[l_ActualNode] + m_Graph->GetNodeInfo( l_ActualNode ).Distance( m_Graph->GetNodeInfo(
                                       l_ActualNeighbour ) );

            if ( std::find( l_OpenSet.begin(), l_OpenSet.end(), l_ActualNeighbour ) == l_OpenSet.end() ||
                    l_ActualGScore < l_GScore[l_ActualNeighbour] )
            {
                l_CameFrom[l_ActualNeighbour] = l_ActualNode;
                l_GScore[l_ActualNeighbour] = l_ActualGScore;
                l_FScore[l_ActualNeighbour] = l_GScore[l_ActualNeighbour] + m_Graph->GetNodeInfo( l_ActualNeighbour ).Distance(
                                                  final_pos );

                if ( std::find( l_OpenSet.begin(), l_OpenSet.end(), l_ActualNeighbour ) == l_OpenSet.end() )
                { l_OpenSet.push_back( l_ActualNeighbour ); }
            }
        }
    }

    //Path not found
    LOG_WARNING_APPLICATION( "Path not found in AStar" );
    return path;
}