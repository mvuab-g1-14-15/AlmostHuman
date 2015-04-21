#ifndef MAV_MESH_MANAGER_H
#define MAV_MESH_MANAGER_H

#include "XML\XMLTreeNode.h"

#include "Utils\Manager.h"

/// Specifies a configuration to use when performing Recast builds.
/// @ingroup recast
struct rcConfig
{
	/// The width of the field along the x-axis. [Limit: >= 0] [Units: vx]
	int width;
	/// The height of the field along the z-axis. [Limit: >= 0] [Units: vx]
	int height;
	/// The width/height size of tile's on the xz-plane. [Limit: >= 0] [Units: vx]
	int tileSize;
	/// The size of the non-navigable border around the heightfield. [Limit: >=0] [Units: vx]
	int borderSize;
	/// The xz-plane cell size to use for fields. [Limit: > 0] [Units: wu]
	float cs;
	/// The y-axis cell size to use for fields. [Limit: > 0] [Units: wu]
	float ch;
	/// The minimum bounds of the field's AABB. [(x, y, z)] [Units: wu]
	float bmin[3];
	/// The maximum bounds of the field's AABB. [(x, y, z)] [Units: wu]
	float bmax[3];
	/// The maximum slope that is considered walkable. [Limits: 0 <= value < 90] [Units: Degrees]
	float walkableSlopeAngle;
	/// Minimum floor to 'ceiling' height that will still allow the floor area to
	/// be considered walkable. [Limit: >= 3] [Units: vx]
	int walkableHeight;
	/// Maximum ledge height that is considered to still be traversable. [Limit: >=0] [Units: vx]
	int walkableClimb;
	/// The distance to erode/shrink the walkable area of the heightfield away from
	/// obstructions. [Limit: >=0] [Units: vx]
	int walkableRadius;
	/// The maximum allowed length for contour edges along the border of the mesh. [Limit: >=0] [Units: vx]
	int maxEdgeLen;
	/// The maximum distance a simplfied contour's border edges should deviate
	/// the original raw contour. [Limit: >=0] [Units: vx]
	float maxSimplificationError;
	/// The minimum number of cells allowed to form isolated island areas. [Limit: >=0] [Units: vx]
	int minRegionArea;
	/// Any regions with a span count smaller than this value will, if possible,
	/// be merged with larger regions. [Limit: >=0] [Units: vx]
	int mergeRegionArea;
	/// The maximum number of vertices allowed for polygons generated during the
	/// contour to polygon conversion process. [Limit: >= 3]
	int maxVertsPerPoly;
	/// Sets the sampling distance to use when generating the detail mesh.
	/// (For height detail only.) [Limits: 0 or >= 0.9] [Units: wu]
	float detailSampleDist;
	/// The maximum distance the detail mesh surface should deviate from heightfield
	/// data. (For height detail only.) [Limit: >=0] [Units: wu]
	float detailSampleMaxError;
};

class CNavMesh;

class CNavMeshManager: public CManager
{
public:
	CNavMeshManager(const CXMLTreeNode& atts);
	virtual ~CNavMeshManager();

	virtual void Init();
	virtual void Update();
	virtual void Render();

private:
	CNavMesh* m_NavMesh;

};

#endif // MAV_MESH_MANAGER_H
