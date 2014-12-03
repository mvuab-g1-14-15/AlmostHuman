global VERTEX_TYPE_GEOMETRY 	= 0x0001
global VERTEX_TYPE_NORMAL 		= 0x0002
global VERTEX_TYPE_TANGENT 	 	= 0x0004
global VERTEX_TYPE_BINORMAL	 	= 0x0008
global VERTEX_TYPE_TEXTURE1 	= 0x0010
global VERTEX_TYPE_TEXTURE2 	= 0x0020
global VERTEX_TYPE_DIFFUSE 		= 0x0040

struct Vertex
(
	Coord,
	Normal,
	TUVMap1
)

function CreateVertex Coord Normal TUVMap1 =
(
	local NewVertex = Vertex()
	NewVertex.Coord = (Coord as point3)
	NewVertex.Normal = (Normal as point3)
	NewVertex.TUVMap1 = (TUVMap1 as point3)
	return NewVertex
)

fn AreVertexsEqual vtx1 vtx2 =
(
	return (vtx1 as string) == (vtx2 as string)
)

fn FindVertex vtxs vtx =
(	
	for i=1 to vtxs.count do
	(
		format "Comparing % with % \n" vtxs[i] vtx
		if AreVertexsEqual vtxs[i] vtx then return i as integer
	)
	return -1
)

fn AddVertex vertex_to_add vertex_buffers material_id =
(
	format "Adding new vertex % \n" vertex_to_add
	current_vertex_buffer = vertex_buffers[material_id]
	
	id = FindVertex vertex_to_add current_vertex_buffer
	if id == -1 then
	(
		id = current_vertex_buffer[2].count + 1;
		format "Id Not Found Create new % \n" id
		append current_vertex_buffer[2] vertex_to_add
	)
	
	format "Buffer Size % Elements % \n" vertex_buffer.count vertex_buffer
	return id
)

fn ExportMaterial material =
(
	local material_out = #()
	if material.diffuseMapEnable then 
	(
		append material_out (filenameFromPath Material.diffuseMap.filename)
	)
	
	if material.selfIllumMapEnable then 
	(
		append material_out (filenameFromPath Material.selfIllumMap.filename)
	)
	
	if material.bumpMapEnable then 
	(
		append material_out (filenameFromPath Material.bumpMap.filename)
	)
	
	if material.reflectionMapEnable then 
	(
		append material_out (filenameFromPath Material.reflectionMap.filename)
	)
	
	format "%\n" material_out
	return material_out
)

fn GetBuffers Obj VertexBuffer IndexBuffer =
(
	format "Getting buffers...\n"
	local l_nVertexs = getNumVerts Obj
	local l_nFaces = getNumFaces Obj
	
	for iFaces = 1 to l_nFaces do
	(
		format "Parsing face %...\n" iFaces
		-- Para coger los indices de la primera cara
		local IdxsFace=getFace Obj iFaces
		
		-- Para coger los vértices de la primera cara
		local Coord1=getVert Obj IdxsFace.x
		local Coord3=getVert Obj IdxsFace.y
		local Coord2=getVert Obj IdxsFace.z
		
		-- Para coger las normales de los vértices de la primera cara
		local Normal1=getNormal Obj IdxsFace.x
		local Normal2=getNormal Obj IdxsFace.y
		local Normal3=getNormal Obj IdxsFace.z
		
		/* Para coger los índices de las coordenadas de textura de la primera cara, el 1 señala al primer
		conjunto de coordenadas aplicadas sobre la cara */
		local IdxsMap = meshop.getMapFace Obj 1 iFaces
		
		-- Para coger las coordenadas de los vértices de la primera cara
		local TUVMap1= meshop.getMapVert Obj 1 IdxsMap.x
		local TUVMap2= meshop.getMapVert Obj 1 IdxsMap.y
		local TUVMap3= meshop.getMapVert Obj 1 IdxsMap.z
		
		/* Por ultimo para saber el Id del material que tiene aplicada esta cara en caso de estar utilizando
		un multimaterial sobre el objeto */
		local MaterialID=getFaceMatID Obj iFaces
		
		local Vertex1 = CreateVertex Coord1 Normal1 TUVMap1
		local Vertex2 = CreateVertex Coord2 Normal2 TUVMap2
		local Vertex3 = CreateVertex Coord3 Normal3 TUVMap3
		
		format "%\n%\n%\n - Material ID: %\n" Vertex1 Vertex2 Vertex3 MaterialID
		--local id1 = AddVertex Vertex1 VertexBuffer MaterialID
		--local id2 = AddVertex Vertex2 VertexBuffer MaterialID
		--local id3 = AddVertex Vertex3 VertexBuffer MaterialID
		format "ID1 % ID2 % ID3 %\n" id1 id2 id3
	)
)

fn ExportMesh Obj filename =
(
	if ( classOf(Obj) ) != Editable_mesh then
	(
		format "Only Editable Meshes allowed...\n"
		return -1
	)
	
	local file = fopen filename "wb"
	
	WriteShort file 0x55ff #unsigned
	
	-- MATERIALS
	local l_Material = Obj.material
	materials = #()
	if classof(l_Material) == Multimaterial then
	(
		format "Saving multimaterial object...\n"
		for i=1 to l_Material.numsubs do
		(
			if l_Material[i] != undefined then
				append materials ( ExportMaterial l_Material[i] )
		)
		
		format "Num mats: % v %\n" materials.count materials
	)
	else
	(
		format "Saving standard material object...\n"
		append materials ( ExportMaterial l_Material )
	)
	
	-- Write into file
	WriteShort file materials.count #unsigned--Write number of materials
	for iMaterials = 1 to materials.count do
	(
		current_material = materials[iMaterials]
		
		WriteShort file current_material.count #unsigned--Write number of textures per material
		for iTexture = 1 to current_material.count do
		(
			current_texture = current_material[iTexture]
			WriteShort file current_texture.count #unsigned--Write Type
			WriteString file current_texture --Write Type
		)
	)
	-- MATERIALS END
	
	local l_VertexBuffer = #()
	local l_IndexBuffer = #()
	GetBuffers Obj l_VertexBuffer l_IndexBuffer
	
	WriteShort file 0xff55 #unsigned
	fclose file
	
	return 0
)

ExportMesh $ "C:\prueba3.mesh"