struct LightProbe
(
	mPos = point3 0 0 0,
	mVertex = #()
)

struct LPVertex
(
	mPos = point3 0 0 0,
	mUV = point2 0 0
)

fn GetUV Obj i =
(
	local TexFaces = (meshop.getFacesUsingVert Obj i) as array --Convert Bit Flags to an int array containing face indices
	--format "Textface: %\n" TexFaces
	local TexVert = 0
	local MeshFace = getFace Obj TexFaces[1]
	--format "MeshFace: %\n" MeshFace
	local MappingFace = meshop.getMapFace Obj 1 TexFaces[1]
	--format "MappingFace: %\n" MappingFace
	--Match up Tex Vert with the mesh vert
	if MeshFace.x == i then(
		TexVert = MappingFace.x
	) else if MeshFace.y == i then(
		TexVert = MappingFace.y
	) else (
		TexVert = MappingFace.z
	)
	--format "TexVert: %\n" TexVert
	--Retrieve UVW for the current face
	--The 1 in this function call refers to texture channel 1
	local VertUVW = meshop.getMapVert Obj 1 TexVert
	--format "VertUVW: %\n" VertUVW
	-- Finally return out the UV for this vertex
	return point2 VertUVW.x VertUVW.y
)

fn ExportLightProbes Obj =
(
	local lLightProbes = #()
	local lCounter = 0
	local lCenters = #()
	local lCenterVert = point3 0 0 0
	local lLightProbeVertex = #()
	local lNumVerts = getNumVerts Obj
	for i = 1 to lNumVerts do
	(
		if lCounter == 6 then
		(
			local lLightProbe = LightProbe()
			lLightProbe.mPos = lCenterVert
			lLightProbe.mVertex = lLightProbeVertex
			append lLightProbes lLightProbe
			lCounter = 0
			lCenterVert = point3 0 0 0
			lLightProbeVertex = #()
		)
		local lVert = meshOp.getVert Obj i
		local lUV = GetUV Obj i
		local lLPVertex = LPVertex()
		lLPVertex.mPos = lVert
		lLPVertex.mUV = lUV
		append lLightProbeVertex lLPVertex
		lCenterVert = lCenterVert + lVert / 6.0
		lCounter = lCounter + 1
	)
	format "%" lLightProbes
)

ExportLightProbes $