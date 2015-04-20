#include "NavMesh.h"

#include "Utils\Defines.h"
#include "StaticMeshes\StaticMesh.h"

CNavMesh::CNavMesh()
{
}

CNavMesh::~CNavMesh()
{

}

void CNavMesh::AddMesh( const CStaticMesh* aStaticMesh )
{
	ASSERT( aStaticMesh, "Adding a null static mesh to the nav mesh" );

	const std::vector<Math::Vect3f> & l_VB = aStaticMesh->GetVB();
	const std::vector<uint32> & l_IB = aStaticMesh->GetIB();

	{
		std::vector<Math::Vect3f>::const_iterator it = l_VB.begin(),
												  it_end = l_VB.end();
		for (; it!=it_end; ++it)
		{
			const Math::Vect3f & point = *it;
			m_VB.push_back(point.x);
			m_VB.push_back(point.y);
			m_VB.push_back(point.z);
		}
	}

	{
		std::vector<uint32>::const_iterator it = l_IB.begin(),
											it_end = l_IB.end();
		for (; it!=it_end; ++it)
		{
			const int & index = (int)*it;
			m_IB.push_back(index);
		}
	}
}