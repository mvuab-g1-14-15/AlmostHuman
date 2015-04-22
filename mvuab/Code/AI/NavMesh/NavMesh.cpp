#include "NavMesh.h"

#include "Utils\Defines.h"
#include "StaticMeshes\StaticMesh.h"

#include "recastnavigation\Recast\Recast.h"
#include "recastnavigation\RecastDemo\ChunkyTriMesh.h"

#include <limits>

CNavMesh::CNavMesh()
	: m_MaxIndex( -1 )
	, m_BBMin( std::numeric_limits<float>::infinity() )
	, m_BBMax( -std::numeric_limits<float>::infinity() )
{
}

CNavMesh::~CNavMesh()
{

}

void CNavMesh::AddMesh( Math::Vect3f aPos, const CStaticMesh* aStaticMesh )
{
	ASSERT( aStaticMesh, "Adding a null static mesh to the nav mesh" );

	const std::vector<Math::Vect3f> & l_VB = aStaticMesh->GetVB();
	const std::vector<uint32> & l_IB = aStaticMesh->GetIB();

	{
		std::vector<Math::Vect3f>::const_iterator it = l_VB.begin(),
												  it_end = l_VB.end();
		for (; it!=it_end; ++it)
		{
			const Math::Vect3f & point = *it + aPos;
			m_VB.push_back(point.x);
			m_VB.push_back(point.y);
			m_VB.push_back(point.z);

			RecalculateBB(point);
		}
	}

	{
		int l_OffsetIndex( m_MaxIndex + 1 );

		std::vector<uint32>::const_iterator it = l_IB.begin(),
											it_end = l_IB.end();
		for (; it!=it_end; ++it)
		{
			const int & index = (int)*it + l_OffsetIndex;
			m_IB.push_back(index);
			if ( index > m_MaxIndex )
				m_MaxIndex = index;
		}
	}
}

void CNavMesh::RecalculateBB( Math::Vect3f point )
{
	if ( point.x > m_BBMax.x )
		m_BBMax.x = point.x;
	if ( point.y > m_BBMax.y )
		m_BBMax.y = point.y;
	if ( point.z > m_BBMax.z )
		m_BBMax.z = point.z;
	if ( point.x < m_BBMin.x )
		m_BBMin.x = point.x;
	if ( point.y < m_BBMin.y )
		m_BBMin.y = point.y;
	if ( point.z < m_BBMin.z )
		m_BBMin.z = point.z;
}

void CNavMesh::Calculate()
{
	/*
bool Sample_SoloMesh::handleBuild()
{
	if (!m_geom || !m_geom->getMesh())
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Input mesh is not specified.");
		return false;
	}
	
	cleanup();
	
	const float* bmin = m_geom->getMeshBoundsMin();
	const float* bmax = m_geom->getMeshBoundsMax();
	const float* verts = m_geom->getMesh()->getVerts();
	const int nverts = m_geom->getMesh()->getVertCount();
	const int* tris = m_geom->getMesh()->getTris();
	const int ntris = m_geom->getMesh()->getTriCount();
	
	//
	// Step 1. Initialize build config.
	//
	
	// Init build configuration from GUI
	memset(&m_cfg, 0, sizeof(m_cfg));
	m_cfg.cs = m_cellSize;
	m_cfg.ch = m_cellHeight;
	m_cfg.walkableSlopeAngle = m_agentMaxSlope;
	m_cfg.walkableHeight = (int)ceilf(m_agentHeight / m_cfg.ch);
	m_cfg.walkableClimb = (int)floorf(m_agentMaxClimb / m_cfg.ch);
	m_cfg.walkableRadius = (int)ceilf(m_agentRadius / m_cfg.cs);
	m_cfg.maxEdgeLen = (int)(m_edgeMaxLen / m_cellSize);
	m_cfg.maxSimplificationError = m_edgeMaxError;
	m_cfg.minRegionArea = (int)rcSqr(m_regionMinSize);		// Note: area = size*size
	m_cfg.mergeRegionArea = (int)rcSqr(m_regionMergeSize);	// Note: area = size*size
	m_cfg.maxVertsPerPoly = (int)m_vertsPerPoly;
	m_cfg.detailSampleDist = m_detailSampleDist < 0.9f ? 0 : m_cellSize * m_detailSampleDist;
	m_cfg.detailSampleMaxError = m_cellHeight * m_detailSampleMaxError;
	
	// Set the area where the navigation will be build.
	// Here the bounds of the input mesh are used, but the
	// area could be specified by an user defined box, etc.
	rcVcopy(m_cfg.bmin, bmin);
	rcVcopy(m_cfg.bmax, bmax);
	rcCalcGridSize(m_cfg.bmin, m_cfg.bmax, m_cfg.cs, &m_cfg.width, &m_cfg.height);

	// Reset build times gathering.
	m_ctx->resetTimers();

	// Start the build process.	
	m_//ctx->startTimer(RC_TIMER_TOTAL);
	
	m_ctx->log(RC_LOG_PROGRESS, "Building navigation:");
	m_ctx->log(RC_LOG_PROGRESS, " - %d x %d cells", m_cfg.width, m_cfg.height);
	m_ctx->log(RC_LOG_PROGRESS, " - %.1fK verts, %.1fK tris", nverts/1000.0f, ntris/1000.0f);
	
	//
	// Step 2. Rasterize input polygon soup.
	//
	
	// Allocate voxel heightfield where we rasterize our input data to.
	m_solid = rcAllocHeightfield();
	if (!m_solid)
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'solid'.");
		return false;
	}
	if (!rcCreateHeightfield(m_ctx, *m_solid, m_cfg.width, m_cfg.height, m_cfg.bmin, m_cfg.bmax, m_cfg.cs, m_cfg.ch))
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create solid heightfield.");
		return false;
	}
	
	// Allocate array that can hold triangle area types.
	// If you have multiple meshes you need to process, allocate
	// and array which can hold the max number of triangles you need to process.
	m_triareas = new unsigned char[ntris];
	if (!m_triareas)
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'm_triareas' (%d).", ntris);
		return false;
	}
	
	// Find triangles which are walkable based on their slope and rasterize them.
	// If your input data is multiple meshes, you can transform them here, calculate
	// the are type for each of the meshes and rasterize them.
	memset(m_triareas, 0, ntris*sizeof(unsigned char));
	rcMarkWalkableTriangles(m_ctx, m_cfg.walkableSlopeAngle, verts, nverts, tris, ntris, m_triareas);
	rcRasterizeTriangles(m_ctx, verts, nverts, tris, m_triareas, ntris, *m_solid, m_cfg.walkableClimb);

	if (!m_keepInterResults)
	{
		delete [] m_triareas;
		m_triareas = 0;
	}
	
	//
	// Step 3. Filter walkables surfaces.
	//
	
	// Once all geoemtry is rasterized, we do initial pass of filtering to
	// remove unwanted overhangs caused by the conservative rasterization
	// as well as filter spans where the character cannot possibly stand.
	rcFilterLowHangingWalkableObstacles(m_ctx, m_cfg.walkableClimb, *m_solid);
	rcFilterLedgeSpans(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid);
	rcFilterWalkableLowHeightSpans(m_ctx, m_cfg.walkableHeight, *m_solid);


	//
	// Step 4. Partition walkable surface to simple regions.
	//

	// Compact the heightfield so that it is faster to handle from now on.
	// This will result more cache coherent data as well as the neighbours
	// between walkable cells will be calculated.
	m_chf = rcAllocCompactHeightfield();
	if (!m_chf)
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'chf'.");
		return false;
	}
	if (!rcBuildCompactHeightfield(m_ctx, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid, *m_chf))
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build compact data.");
		return false;
	}
	
	if (!m_keepInterResults)
	{
		rcFreeHeightField(m_solid);
		m_solid = 0;
	}
		
	// Erode the walkable area by agent radius.
	if (!rcErodeWalkableArea(m_ctx, m_cfg.walkableRadius, *m_chf))
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
		return false;
	}

	// (Optional) Mark areas.
	const ConvexVolume* vols = m_geom->getConvexVolumes();
	for (int i  = 0; i < m_geom->getConvexVolumeCount(); ++i)
		rcMarkConvexPolyArea(m_ctx, vols[i].verts, vols[i].nverts, vols[i].hmin, vols[i].hmax, (unsigned char)vols[i].area, *m_chf);

	
	// Partition the heightfield so that we can use simple algorithm later to triangulate the walkable areas.
	// There are 3 martitioning methods, each with some pros and cons:
	// 1) Watershed partitioning
	//   - the classic Recast partitioning
	//   - creates the nicest tessellation
	//   - usually slowest
	//   - partitions the heightfield into nice regions without holes or overlaps
	//   - the are some corner cases where this method creates produces holes and overlaps
	//      - holes may appear when a small obstacles is close to large open area (triangulation can handle this)
	//      - overlaps may occur if you have narrow spiral corridors (i.e stairs), this make triangulation to fail
	//   * generally the best choice if you precompute the nacmesh, use this if you have large open areas
	// 2) Monotone partioning
	//   - fastest
	//   - partitions the heightfield into regions without holes and overlaps (guaranteed)
	//   - creates long thin polygons, which sometimes causes paths with detours
	//   * use this if you want fast navmesh generation
	// 3) Layer partitoining
	//   - quite fast
	//   - partitions the heighfield into non-overlapping regions
	//   - relies on the triangulation code to cope with holes (thus slower than monotone partitioning)
	//   - produces better triangles than monotone partitioning
	//   - does not have the corner cases of watershed partitioning
	//   - can be slow and create a bit ugly tessellation (still better than monotone)
	//     if you have large open areas with small obstacles (not a problem if you use tiles)
	//   * good choice to use for tiled navmesh with medium and small sized tiles
	
	if (m_partitionType == SAMPLE_PARTITION_WATERSHED)
	{
		// Prepare for region partitioning, by calculating distance field along the walkable surface.
		if (!rcBuildDistanceField(m_ctx, *m_chf))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build distance field.");
			return false;
		}
		
		// Partition the walkable surface into simple regions without holes.
		if (!rcBuildRegions(m_ctx, *m_chf, 0, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build watershed regions.");
			return false;
		}
	}
	else if (m_partitionType == SAMPLE_PARTITION_MONOTONE)
	{
		// Partition the walkable surface into simple regions without holes.
		// Monotone partitioning does not need distancefield.
		if (!rcBuildRegionsMonotone(m_ctx, *m_chf, 0, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build monotone regions.");
			return false;
		}
	}
	else // SAMPLE_PARTITION_LAYERS
	{
		// Partition the walkable surface into simple regions without holes.
		if (!rcBuildLayerRegions(m_ctx, *m_chf, 0, m_cfg.minRegionArea))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build layer regions.");
			return false;
		}
	}
	
	//
	// Step 5. Trace and simplify region contours.
	//
	
	// Create contours.
	m_cset = rcAllocContourSet();
	if (!m_cset)
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'cset'.");
		return false;
	}
	if (!rcBuildContours(m_ctx, *m_chf, m_cfg.maxSimplificationError, m_cfg.maxEdgeLen, *m_cset))
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create contours.");
		return false;
	}
	
	//
	// Step 6. Build polygons mesh from contours.
	//
	
	// Build polygon navmesh from the contours.
	m_pmesh = rcAllocPolyMesh();
	if (!m_pmesh)
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmesh'.");
		return false;
	}
	if (!rcBuildPolyMesh(m_ctx, *m_cset, m_cfg.maxVertsPerPoly, *m_pmesh))
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not triangulate contours.");
		return false;
	}
	
	//
	// Step 7. Create detail mesh which allows to access approximate height on each polygon.
	//
	
	m_dmesh = rcAllocPolyMeshDetail();
	if (!m_dmesh)
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'pmdtl'.");
		return false;
	}

	if (!rcBuildPolyMeshDetail(m_ctx, *m_pmesh, *m_chf, m_cfg.detailSampleDist, m_cfg.detailSampleMaxError, *m_dmesh))
	{
		m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build detail mesh.");
		return false;
	}

	if (!m_keepInterResults)
	{
		rcFreeCompactHeightfield(m_chf);
		m_chf = 0;
		rcFreeContourSet(m_cset);
		m_cset = 0;
	}

	// At this point the navigation mesh data is ready, you can access it from m_pmesh.
	// See duDebugDrawPolyMesh or dtCreateNavMeshData as examples how to access the data.
	
	//
	// (Optional) Step 8. Create Detour data from Recast poly mesh.
	//
	
	// The GUI may allow more max points per polygon than Detour can handle.
	// Only build the detour navmesh if we do not exceed the limit.
	if (m_cfg.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
	{
		unsigned char* navData = 0;
		int navDataSize = 0;

		// Update poly flags from areas.
		for (int i = 0; i < m_pmesh->npolys; ++i)
		{
			if (m_pmesh->areas[i] == RC_WALKABLE_AREA)
				m_pmesh->areas[i] = SAMPLE_POLYAREA_GROUND;
				
			if (m_pmesh->areas[i] == SAMPLE_POLYAREA_GROUND ||
				m_pmesh->areas[i] == SAMPLE_POLYAREA_GRASS ||
				m_pmesh->areas[i] == SAMPLE_POLYAREA_ROAD)
			{
				m_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK;
			}
			else if (m_pmesh->areas[i] == SAMPLE_POLYAREA_WATER)
			{
				m_pmesh->flags[i] = SAMPLE_POLYFLAGS_SWIM;
			}
			else if (m_pmesh->areas[i] == SAMPLE_POLYAREA_DOOR)
			{
				m_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
			}
		}


		dtNavMeshCreateParams params;
		memset(&params, 0, sizeof(params));
		params.verts = m_pmesh->verts;
		params.vertCount = m_pmesh->nverts;
		params.polys = m_pmesh->polys;
		params.polyAreas = m_pmesh->areas;
		params.polyFlags = m_pmesh->flags;
		params.polyCount = m_pmesh->npolys;
		params.nvp = m_pmesh->nvp;
		params.detailMeshes = m_dmesh->meshes;
		params.detailVerts = m_dmesh->verts;
		params.detailVertsCount = m_dmesh->nverts;
		params.detailTris = m_dmesh->tris;
		params.detailTriCount = m_dmesh->ntris;
		params.offMeshConVerts = m_geom->getOffMeshConnectionVerts();
		params.offMeshConRad = m_geom->getOffMeshConnectionRads();
		params.offMeshConDir = m_geom->getOffMeshConnectionDirs();
		params.offMeshConAreas = m_geom->getOffMeshConnectionAreas();
		params.offMeshConFlags = m_geom->getOffMeshConnectionFlags();
		params.offMeshConUserID = m_geom->getOffMeshConnectionId();
		params.offMeshConCount = m_geom->getOffMeshConnectionCount();
		params.walkableHeight = m_agentHeight;
		params.walkableRadius = m_agentRadius;
		params.walkableClimb = m_agentMaxClimb;
		rcVcopy(params.bmin, m_pmesh->bmin);
		rcVcopy(params.bmax, m_pmesh->bmax);
		params.cs = m_cfg.cs;
		params.ch = m_cfg.ch;
		params.buildBvTree = true;
		
		if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
		{
			m_ctx->log(RC_LOG_ERROR, "Could not build Detour navmesh.");
			return false;
		}
		
		m_navMesh = dtAllocNavMesh();
		if (!m_navMesh)
		{
			dtFree(navData);
			m_ctx->log(RC_LOG_ERROR, "Could not create Detour navmesh");
			return false;
		}
		
		dtStatus status;
		
		status = m_navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
		if (dtStatusFailed(status))
		{
			dtFree(navData);
			m_ctx->log(RC_LOG_ERROR, "Could not init Detour navmesh");
			return false;
		}
		
		status = m_navQuery->init(m_navMesh, 2048);
		if (dtStatusFailed(status))
		{
			m_ctx->log(RC_LOG_ERROR, "Could not init Detour navmesh query");
			return false;
		}
	}
	
	m_//ctx->stopTimer(RC_TIMER_TOTAL);

	// Show performance stats.
	duLogBuildTimes(*m_ctx, m_ctx->getAccumulatedTime(RC_TIMER_TOTAL));
	m_ctx->log(RC_LOG_PROGRESS, ">> Polymesh: %d vertices  %d polygons", m_pmesh->nverts, m_pmesh->npolys);
	
	m_totalBuildTimeMs = m_ctx->getAccumulatedTime(RC_TIMER_TOTAL)/1000.0f;
	
	if (m_tool)
		m_tool->init(this);
	initToolStates(this);

	return true;
}
	*/

	int l_NumVerts = m_VB.size();
	int l_NumIdxs = m_IB.size()/3;
	float* l_Verts = &m_VB[0];
	int* l_Idxs = &m_IB[0];
	float l_BBMin[3] = { m_BBMin.x, m_BBMin.y, m_BBMin.z };
	float l_BBMax[3] = { m_BBMax.x, m_BBMax.y, m_BBMax.z };
	
	//
	// Step 1. Initialize build config.
	//
	
	// Init build configuration from GUI
	rcConfig m_cfg;
	m_cfg.cs = 1;
	m_cfg.ch = 1;
	m_cfg.walkableSlopeAngle = 0.5f;
	m_cfg.walkableHeight = (int)ceilf(2 / m_cfg.ch);
	m_cfg.walkableClimb = (int)floorf(2 / m_cfg.ch);
	m_cfg.walkableRadius = (int)ceilf(0.4f / m_cfg.cs);
	m_cfg.maxEdgeLen = (int)(2 / 1);
	m_cfg.maxSimplificationError = 1;
	m_cfg.minRegionArea = (int)rcSqr(512);		// Note: area = size*size
	m_cfg.mergeRegionArea = (int)rcSqr(512);	// Note: area = size*size
	m_cfg.maxVertsPerPoly = (int)3;
	m_cfg.detailSampleDist = 1 < 0.9f ? 0 : 1 * 1;
	m_cfg.detailSampleMaxError = 1 * 1;
	
	// Set the area where the navigation will be build.
	// Here the bounds of the input mesh are used, but the
	// area could be specified by an user defined box, etc.
	rcVcopy(m_cfg.bmin, l_BBMin);
	rcVcopy(m_cfg.bmax, l_BBMax);
	rcCalcGridSize(m_cfg.bmin, m_cfg.bmax, m_cfg.cs, &m_cfg.width, &m_cfg.height);
	
	//
	// Step 2. Rasterize input polygon soup.
	//
	
	// Allocate voxel heightfield where we rasterize our input data to.
	rcHeightfield* m_solid = rcAllocHeightfield();
	if (!m_solid)
	{
	}
	if (!rcCreateHeightfield(0, *m_solid, m_cfg.width, m_cfg.height, m_cfg.bmin, m_cfg.bmax, m_cfg.cs, m_cfg.ch))
	{
	}
	
	// Allocate array that can hold triangle area types.
	// If you have multiple meshes you need to process, allocate
	// and array which can hold the max number of triangles you need to process.
	unsigned char* m_triareas = new unsigned char[l_NumIdxs];
	if (!m_triareas)
	{
	}
	
	// Find triangles which are walkable based on their slope and rasterize them.
	// If your input data is multiple meshes, you can transform them here, calculate
	// the are type for each of the meshes and rasterize them.
	memset(m_triareas, 0, l_NumIdxs*sizeof(unsigned char));
	rcMarkWalkableTriangles(0, m_cfg.walkableSlopeAngle, l_Verts, l_NumVerts, l_Idxs, l_NumIdxs, m_triareas);
	rcRasterizeTriangles(0, l_Verts, l_NumVerts, l_Idxs, m_triareas, l_NumIdxs, *m_solid, m_cfg.walkableClimb);

	delete [] m_triareas;
	m_triareas = 0;

	//
	// Step 3. Filter walkables surfaces.
	//
	
	// Once all geoemtry is rasterized, we do initial pass of filtering to
	// remove unwanted overhangs caused by the conservative rasterization
	// as well as filter spans where the character cannot possibly stand.
	rcFilterLowHangingWalkableObstacles(0, m_cfg.walkableClimb, *m_solid);
	rcFilterLedgeSpans(0, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid);
	rcFilterWalkableLowHeightSpans(0, m_cfg.walkableHeight, *m_solid);


	//
	// Step 4. Partition walkable surface to simple regions.
	//

	// Compact the heightfield so that it is faster to handle from now on.
	// This will result more cache coherent data as well as the neighbours
	// between walkable cells will be calculated.
	rcCompactHeightfield* m_chf = rcAllocCompactHeightfield();
	if (!m_chf)
	{
	}
	if (!rcBuildCompactHeightfield(0, m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid, *m_chf))
	{
	}

	rcFreeHeightField(m_solid);
	m_solid = 0;
		
	// Erode the walkable area by agent radius.
	if (!rcErodeWalkableArea(0, m_cfg.walkableRadius, *m_chf))
	{
	}

	// (Optional) Mark areas.
	/*
	const ConvexVolume* vols = m_geom->getConvexVolumes();
	for (int i  = 0; i < m_geom->getConvexVolumeCount(); ++i)
		rcMarkConvexPolyArea(m_ctx, vols[i].verts, vols[i].nverts, vols[i].hmin, vols[i].hmax, (unsigned char)vols[i].area, *m_chf);
	*/

	
	// Partition the heightfield so that we can use simple algorithm later to triangulate the walkable areas.
	// There are 3 martitioning methods, each with some pros and cons:
	// 1) Watershed partitioning
	//   - the classic Recast partitioning
	//   - creates the nicest tessellation
	//   - usually slowest
	//   - partitions the heightfield into nice regions without holes or overlaps
	//   - the are some corner cases where this method creates produces holes and overlaps
	//      - holes may appear when a small obstacles is close to large open area (triangulation can handle this)
	//      - overlaps may occur if you have narrow spiral corridors (i.e stairs), this make triangulation to fail
	//   * generally the best choice if you precompute the nacmesh, use this if you have large open areas
	// 2) Monotone partioning
	//   - fastest
	//   - partitions the heightfield into regions without holes and overlaps (guaranteed)
	//   - creates long thin polygons, which sometimes causes paths with detours
	//   * use this if you want fast navmesh generation
	// 3) Layer partitoining
	//   - quite fast
	//   - partitions the heighfield into non-overlapping regions
	//   - relies on the triangulation code to cope with holes (thus slower than monotone partitioning)
	//   - produces better triangles than monotone partitioning
	//   - does not have the corner cases of watershed partitioning
	//   - can be slow and create a bit ugly tessellation (still better than monotone)
	//     if you have large open areas with small obstacles (not a problem if you use tiles)
	//   * good choice to use for tiled navmesh with medium and small sized tiles
	
	//if (m_partitionType == SAMPLE_PARTITION_WATERSHED)
	{
		// Prepare for region partitioning, by calculating distance field along the walkable surface.
		if (!rcBuildDistanceField(0, *m_chf))
		{
		}
		
		// Partition the walkable surface into simple regions without holes.
		if (!rcBuildRegions(0, *m_chf, 0, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
		{
		}
	}
	/*
	else if (m_partitionType == SAMPLE_PARTITION_MONOTONE)
	{
		// Partition the walkable surface into simple regions without holes.
		// Monotone partitioning does not need distancefield.
		if (!rcBuildRegionsMonotone(m_ctx, *m_chf, 0, m_cfg.minRegionArea, m_cfg.mergeRegionArea))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build monotone regions.");
			return false;
		}
	}
	else // SAMPLE_PARTITION_LAYERS
	{
		// Partition the walkable surface into simple regions without holes.
		if (!rcBuildLayerRegions(m_ctx, *m_chf, 0, m_cfg.minRegionArea))
		{
			m_ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build layer regions.");
			return false;
		}
	}
	*/

	//
	// Step 5. Trace and simplify region contours.
	//
	
	// Create contours.
	rcContourSet* m_cset = rcAllocContourSet();
	if (!m_cset)
	{
	}
	if (!rcBuildContours(0, *m_chf, m_cfg.maxSimplificationError, m_cfg.maxEdgeLen, *m_cset))
	{
	}
	
	//
	// Step 6. Build polygons mesh from contours.
	//
	
	// Build polygon navmesh from the contours.
	rcPolyMesh* m_pmesh = rcAllocPolyMesh();
	if (!m_pmesh)
	{
	}
	if (!rcBuildPolyMesh(0, *m_cset, m_cfg.maxVertsPerPoly, *m_pmesh))
	{
	}
	
	//
	// Step 7. Create detail mesh which allows to access approximate height on each polygon.
	//
	
	rcPolyMeshDetail* m_dmesh = rcAllocPolyMeshDetail();
	if (!m_dmesh)
	{
	}

	if (!rcBuildPolyMeshDetail(0, *m_pmesh, *m_chf, m_cfg.detailSampleDist, m_cfg.detailSampleMaxError, *m_dmesh))
	{
	}

	rcFreeCompactHeightfield(m_chf);
	m_chf = 0;
	rcFreeContourSet(m_cset);
	m_cset = 0;

	m_sizeX = m_pmesh->bmax[0] - m_pmesh->bmin[0];
	m_sizeY = m_pmesh->bmax[1] - m_pmesh->bmin[1];
	m_sizeZ = m_pmesh->bmax[2] - m_pmesh->bmin[2];

	m_Pos = Math::Vect3f( 0.0f );
	for (int i = 0; i<m_pmesh->nverts*3; i+=3)
	{
		m_Pos.x += m_pmesh->verts[i] / m_pmesh->nverts;
		m_Pos.y += m_pmesh->verts[i+1] / m_pmesh->nverts;
		m_Pos.z += m_pmesh->verts[i+2] / m_pmesh->nverts;
	}
}