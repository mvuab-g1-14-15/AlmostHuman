global VERTEX_TYPE_GEOMETRY 	= 0x0001
global VERTEX_TYPE_NORMAL 		= 0x0002
global VERTEX_TYPE_TANGENT 	 	= 0x0004
global VERTEX_TYPE_BINORMAL	 	= 0x0008
global VERTEX_TYPE_TEXTURE1 	= 0x0010
global VERTEX_TYPE_TEXTURE2 	= 0x0020
global VERTEX_TYPE_DIFFUSE 		= 0x0040

fileIn "GetAABB.ms"

function CreateVertex Coord Normal TUVMap =
(
	local NewVertex = #(Coord.x, Coord.y, Coord.z, Normal.x, Normal.y, Normal.z, TUVMap.x, 1.0-TUVMap.y)
	return NewVertex
)

fn AreVertexsEqual vtx1 vtx2 =
(
	--format "Comparing % with % \n" vtx1 vtx2
	result = true
	for i = 1 to vtx1.count do
		result = result and vtx1[i] == vtx2[i]
	return result
)

fn FindVertex vtx vtxs =
(
	--format "Finding Vertex: % in Vertex Buffer: %\n" vtx vtxs
	for i=1 to vtxs.count do
	(
		--format "Comparing % with % \n" vtxs[i] vtx
		if AreVertexsEqual vtxs[i] vtx then
		(
			--format "The vertex is equal in index % \n" i
			return ( i as integer )-1
		)
	)
	
	--format "Not in -1 \n"
	return -1
)

function ConvertVtx Vtx = 
(
	local l_Text = stringstream ""
	local l_Value=""
	for i=1 to Vtx.count do
	(   
		format "%," Vtx[i] to:l_Text
	)
	return (l_Text as string)
)

function DotNetAddVertex FullVtx VtxBuffers VtxMaps material_id =
(
	while material_id > VtxBuffers.count do
	(
		append VtxBuffers #()
	)
	VtxMap = VtxMaps[material_id]
	Vtxs = VtxBuffers[material_id]
	
	local l_VtxString=ConvertVtx FullVtx
	local l_Idx=1
	if (VtxMap.ContainsKey (l_VtxString))==false then
	(
		append vtxs FullVtx
		l_Idx=vtxs.count-1
		VtxMap.Add l_VtxString l_Idx
	)
	else
		l_Idx=VtxMap.Item[l_VtxString]
	return  l_Idx
)

fn AddVertex vertex_to_add vertex_buffers material_id =
(	
	--format "Material ID: %\n" material_id
	while material_id > vertex_buffers.count do
	(
		append vertex_buffers #()
	)
	
	--format "-- Vertex Buffers %\n" vertex_buffers
	--format "Adding new vertex % \n" vertex_to_add
	current_vertex_buffer = vertex_buffers[material_id]
	
	id = FindVertex vertex_to_add current_vertex_buffer
	
	--format "Id from find vertex % \n" id
	if id == -1 then
	(
		id = current_vertex_buffer.count;
		--format "Id Not Found Create new % \n" id
		append current_vertex_buffer vertex_to_add
	)
	else
	(
		--format "Id Found %\n" id
	)
	
	--format "Buffer Size % Elements % \n" vertex_buffers.count vertex_buffer
	return id
)

fn ExportMaterial objMaterial material_out=
(
	type = (bit.or VERTEX_TYPE_GEOMETRY VERTEX_TYPE_NORMAL)

	if objMaterial != undefined then
	(
		if objMaterial.diffuseMapEnable == true AND classOf(objMaterial.diffuseMap) == BitmapTexture then
		(
			type = (bit.or type VERTEX_TYPE_TEXTURE1)
			append material_out (filenameFromPath objMaterial.diffuseMap.filename)

			if objMaterial.selfIllumMapEnable == true AND classOf(objMaterial.selfIllumMap) == BitmapTexture then
			(
				type = (bit.or type VERTEX_TYPE_TEXTURE2)
				append material_out (filenameFromPath objMaterial.selfIllumMap.filename)
			)
			
			if objMaterial.bumpMapEnable == true AND classOf(objMaterial.bumpMap) == BitmapTexture then
			(
				type = (bit.or type (bit.or VERTEX_TYPE_BINORMAL VERTEX_TYPE_TANGENT) )
				append material_out (filenameFromPath objMaterial.bumpMap.filename)
			)
		)
		else
		(
			type = (bit.or type VERTEX_TYPE_DIFFUSE)
		)
	)

	return type
)

fn GetBuffers Obj VertexBuffer IndexBuffer =
(	
	l_VtxsMap=#(dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int16]")
	
	--format "Getting buffers...\n"
	local l_nVertexs = getNumVerts Obj
	local l_nFaces = getNumFaces Obj
	
	local InvTransform=inverse Obj.transform
	for iFaces = 1 to l_nFaces do
	(
		--format "Parsing face %...\n" iFaces
		-- Para coger los indices de la primera cara
		local IdxsFace=getFace Obj iFaces
		-- Para coger los vértices de la primera cara
		local Coord1=getVert Obj IdxsFace.x*InvTransform
		local Coord2=getVert Obj IdxsFace.y*InvTransform
		local Coord3=getVert Obj IdxsFace.z*InvTransform
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
		local MaterialID
		if classof(Obj.material) == Multimaterial then
		(
			MaterialID=getFaceMatID Obj iFaces
		)
		else
		(
			MaterialID = 1
		)
		
		local id = MaterialID
		while id > l_VtxsMap.count do
		(
			append l_VtxsMap dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int16]"
		)
		
		local Vertex1 = CreateVertex Coord1 Normal1 TUVMap1
		local Vertex2 = CreateVertex Coord2 Normal2 TUVMap2
		local Vertex3 = CreateVertex Coord3 Normal3 TUVMap3
		--format "%\n%\n%\n - Material ID: %\n" Vertex1 Vertex2 Vertex3 MaterialID
		
		local id1 = DotNetAddVertex Vertex1 VertexBuffer l_VtxsMap MaterialID
		local id2 = DotNetAddVertex Vertex2 VertexBuffer l_VtxsMap MaterialID
		local id3 = DotNetAddVertex Vertex3 VertexBuffer l_VtxsMap MaterialID
		--format "ID1 % ID2 % ID3 %\n" id1 id2 id3
		
		--format "Material ID: %\n" MaterialID
		while MaterialID > IndexBuffer.count do
		(
			append IndexBuffer #()
		)
		append IndexBuffer[MaterialID] id1
		append IndexBuffer[MaterialID] id3
		append IndexBuffer[MaterialID] id2
	)
)

fn WriteBuffers file VertexBuffer IndexBuffer =
(
	--format "Writing Buffers...\n"
	num_materials = VertexBuffer.count
	format "#Materials = %\n" num_materials
	for iMaterial = 1 to num_materials do
	(
		num_vertex = VertexBuffer[iMaterial].count
		format "#Vertex = %\n" num_vertex
		WriteShort file num_vertex #unsigned
		for iVertex = 1 to num_vertex do
		(
			current_vertex = VertexBuffer[iMaterial][iVertex]
			format "Vertex %:\n" iVertex
			
			format "\tPos: (%, %, %)\n" current_vertex[1] current_vertex[2] current_vertex[3]
			WriteFloat file current_vertex[1]
			WriteFloat file current_vertex[3]
			WriteFloat file current_vertex[2]
			
			format "\tNormal: (%, %, %)\n" current_vertex[4] current_vertex[5] current_vertex[6]
			WriteFloat file current_vertex[4]
			WriteFloat file current_vertex[6]
			WriteFloat file current_vertex[5]
			
			format "\tUV: (%, %)\n" current_vertex[7] current_vertex[8]
			WriteFloat file current_vertex[7]
			WriteFloat file current_vertex[8]
		)
		
		num_face = IndexBuffer[iMaterial].count
		format "#Faces = %\n" num_face
		WriteShort file num_face #unsigned
		for iFace = 1 to num_face do
		(
			current_face = IndexBuffer[iMaterial][iFace]
			format "Current Face: %\n" current_face
			WriteShort file current_face #unsigned
		)
	)
)

fn ExportMesh Obj filename =
(
	format "dir: % \n" filename
	
	maxOps.cloneNodes Obj cloneType:#instance newNodes:&newObj #nodialog
	
	try
	(
		convertToMesh newObj
		actualObj = newObj[1]
			
		local file = fopen filename "wb"
		
		format "0x55ff\n"
		WriteShort file 0x55ff #unsigned
		
		-- MATERIALS
		local l_Material = actualObj.material
		materials = #()
		types = #()
		if classof(l_Material) == Multimaterial then
		(
			--format "Saving multimaterial object...\n"
			local nFaces = getNumfaces actualObj
			local assigned_mats = #()
			for i=1 to nFaces do
			(
				local id = getFaceMatId actualObj i
				if findItem assigned_mats id == 0 then
					append assigned_mats id
			)
			
			for i=1 to assigned_mats.count do
			(
				local mat = assigned_mats[i]
				actual_mat = #()
				if l_Material[mat] != undefined then
				(
					append types ( ExportMaterial l_Material[mat] actual_mat )
					append materials (actual_mat)
				)
			)
			
			--format "Num mats: % v %\n" materials.count materials
		)
		else
		(
			--format "Saving standard material object...\n"
			actual_mat = #()
			append types ( ExportMaterial l_Material actual_mat)
			append materials (actual_mat)
		)
		
		-- Write into file
		WriteShort file materials.count #unsigned--Write number of materials
		format "#Materials: %\n" materials.count
		for iMaterials = 1 to materials.count do
		(
			current_material = materials[iMaterials]
			
			local type = types[iMaterials]
			WriteShort file type #unsigned
			format "Type: %\n" type
			for iTexture = 1 to current_material.count do
			(
				current_texture = current_material[iTexture]
				WriteShort file current_texture.count #unsigned--Write Lenght Name
				format "#Name: %\n" current_texture.count
				WriteString file current_texture --Write Name
				format "Name: %\n" current_texture
			)
		)
		-- MATERIALS END

		--GetBuffers Obj file
		local l_VertexBuffer = #()
		local l_IndexBuffer = #()
		GetBuffers actualObj l_VertexBuffer l_IndexBuffer
		WriteBuffers file l_VertexBuffer l_IndexBuffer
		
		local bb = GetAABB actualObj
		WriteFloat file bb[1].x
		WriteFloat file bb[1].y
		WriteFloat file bb[1].z
		WriteFloat file bb[2].x
		WriteFloat file bb[2].y
		WriteFloat file bb[2].z

		--format "Vertex Buffer: %\n#Vertex Buffer: %\nIndex Buffer: %\n#Index Buffer: %\n" l_VertexBuffer l_VertexBuffer.count l_IndexBuffer l_IndexBuffer.count
		
		WriteShort file 0xff55 #unsigned
		fclose file
		format "0xff55\n"
	)
	catch
	(
		format "Error while exporting object\n"
	)
	
	delete newObj
	return 0
)