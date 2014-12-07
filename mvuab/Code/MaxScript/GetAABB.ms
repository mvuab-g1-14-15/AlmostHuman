fn GetAABB Obj =
(
	local l_minPoint = point3 999999999999999999 999999999999999999 999999999999999999
	local l_maxPoint = point3 -999999999999999999 -999999999999999999 -999999999999999999
	local l_nFaces = getNumFaces Obj
	
	if ( classOf(Obj) ) != Editable_mesh then
	(
		format "Only Editable Meshes allowed...\n"
		return -1
	)
	
	for iFaces = 1 to l_nFaces do
	(
		format "FACE: %\n" iFaces
		local IdxsFace=getFace Obj iFaces
		local Coord1=getVert Obj IdxsFace.x
		local Coord3=getVert Obj IdxsFace.y
		local Coord2=getVert Obj IdxsFace.z
		
		-- Look for min point
		l_minPoint.x = GetMin l_minPoint.x Coord1.x
		l_minPoint.y = GetMin l_minPoint.y Coord1.y 
		l_minPoint.z = GetMin l_minPoint.z Coord1.z
		
		l_minPoint.x = GetMin l_minPoint.x Coord2.x
		l_minPoint.y = GetMin l_minPoint.y Coord2.y 
		l_minPoint.z = GetMin l_minPoint.z Coord2.z
		
		l_minPoint.x = GetMin l_minPoint.x Coord3.x
		l_minPoint.y = GetMin l_minPoint.y Coord3.y
		l_minPoint.z = GetMin l_minPoint.z Coord3.z
		
		-- Look for max point
		l_maxPoint.x = GetMax l_maxPoint.x Coord1.x
		l_maxPoint.y = GetMax l_maxPoint.y Coord1.y 
		l_maxPoint.z = GetMax l_maxPoint.z Coord1.z
		
		l_maxPoint.x = GetMax l_maxPoint.x Coord2.x
		l_maxPoint.y = GetMax l_maxPoint.y Coord2.y 
		l_maxPoint.z = GetMax l_maxPoint.z Coord2.z
		
		l_maxPoint.x = GetMax l_maxPoint.x Coord3.x
		l_maxPoint.y = GetMax l_maxPoint.y Coord3.y
		l_maxPoint.z = GetMax l_maxPoint.z Coord3.z
	)
	
	format "Points Min % and Max % and num faces %\n" l_minPoint l_maxPoint l_nFaces
)

fn GetMin a b =
(
	if a < b then 
	(
		return a
	)
	else
	(
		return b
	)
)

fn GetMax a b =
(
	if a < b then 
	(
		return b
	)
	else
	(
		return a
	)
)

GetAABB $