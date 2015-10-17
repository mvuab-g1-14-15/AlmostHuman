#include "Graph.h"

#include <algorithm>

CGraph::CGraph() : m_NoNode(0.0f, 0.0f, 0.0f)
{
}

CGraph::~CGraph()
{
    m_NodeInfo.clear();
    m_GraphGrid.clear();
}

unsigned int CGraph::AddNode(const Math::Vect3f &nodeInfo)
{
    unsigned int i = m_NodeInfo.size();
    m_NodeInfo[i] = nodeInfo;
    return m_NodeInfo.size() - 1;
}

const Math::Vect3f &CGraph::GetNodeInfo(unsigned int n)
{
    if (m_NodeInfo.find(n) != m_NodeInfo.end())
    {
        return m_NodeInfo[n];
    }
    return m_NoNode;
}

void CGraph::AddArcWeight(unsigned int n1, unsigned int n2, float v)
{
    m_GraphGrid[n1][n2] = v;
}

float CGraph::GetArcWeight(unsigned int n1, unsigned n2)
{
    std::map<unsigned int, std::map<unsigned int, float>>::iterator it1 = m_GraphGrid.find(n1);
    if(it1 == m_GraphGrid.end())
    {
        return 0;
    }

    std::map<unsigned int, float>::iterator it2 = it1->second.find(n2);
    return (it2 != it1->second.end()) ? it2->second : 0;
}


bool CGraph::Parse(const std::string &file)
{
    CXMLTreeNode l_NewFile;
    if (!l_NewFile.LoadFile(file.c_str()))
    {
        return false;
    }

    CXMLTreeNode l_TreeNode = l_NewFile["graph"];
    if (!l_TreeNode.Exists())
    {
        return false;
    }

    int l_ChildCount = l_TreeNode.GetNumChildren();
    for(int i = 0; i < l_ChildCount; ++i)
    {
        std::string l_TagName = l_TreeNode(i).GetName();

        if( l_TagName == "node" )
        {
            unsigned int l_Id = l_TreeNode(i).GetAttribute<uint32>("id", -1);
            Math::Vect3f l_Vect3f = l_TreeNode(i).GetAttribute<Math::Vect3f>("pos", Math::Vect3f());

            m_NodeInfo[l_Id] = l_Vect3f;
        }

        if( l_TagName == "weight" )
        {
            int l_Src = l_TreeNode(i).GetAttribute<int32>("src"  , 0);
            int l_Dst = l_TreeNode(i).GetAttribute<int32>("dst"  , 0);
            float l_Val = l_TreeNode(i).GetAttribute<float32>("value", 0.0f);

            m_GraphGrid[l_Src][l_Dst] = l_Val;
        }
    }

    l_TreeNode.Done();
    return true;
}