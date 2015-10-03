#ifndef GRAPH_H
#define GRAPH_H
#pragma once

#include "XML\XMLTreeNode.h"
#include "Math\Vector3.h"

#include <vector>
#include <map>

class CGraph
{
private:
  Math::Vect3f m_NoNode;
  std::vector<Math::Vect3f> m_NodeInfo;
  std::map<unsigned int, std::map<unsigned int, unsigned int>> m_GraphGrid;

public:
  CGraph();
  ~CGraph();

  bool Parse( const std::string& file );

  unsigned int AddNode( const Math::Vect3f& nodeInfo );
  const Math::Vect3f& GetNodeInfo( unsigned int n );

  unsigned int GetArcWeight( unsigned int n1, unsigned n2 );
  void AddArcWeight( unsigned int n1, unsigned int n2, unsigned int v );

  unsigned int GetSize()
  {
    return ( unsigned int) m_NodeInfo.size();
  };

  std::map<unsigned int, unsigned int> GetArcs( unsigned int node )
  {
    return m_GraphGrid[node];
  };
};

#endif
