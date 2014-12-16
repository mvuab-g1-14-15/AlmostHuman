--export de la class
fn ExportStaticMeshXMLFile filenameXML MeshDir =
(
	local ostream=createfile filenameXML
	
	local UniqueInstanceArray = GetUniqueInstances()
	
	if UniqueInstanceArrary.count == 0 then 
	(
		format "Empty selection. Select any object \n"
		return -1
	)

	
	format "<static_meshes>\n" to:ostream

	for Obj in UniqueInstanceArray do
	(
			filename = "\\meshes\\" + Obj.name +".mesh"
			tag1 = "\t<static_mesh name=\"" + Obj.name + "\" filename=\"" + filename + "\"/>\n"
			format tag1 to:ostream
	)
	
	format "</static_meshes>\n" to:ostream
	close ostream
)

--export de la class
fn ExportRenderableObjectXMLFile AllObj filenameXML =
(
	local ostream=createfile filenameXML

	local unique_instances = GetUniqueInstances()
	
	if AllObj.count == 0 then 
	(
		format "Empty selection. Select any object \n"
		return -1
	)
	
	format "<RenderableObjects>\n" to:ostream
	for Obj in AllObj do
	(
		local instance_obj;
		if findItem unique_instances Obj != 0 then
			instance_obj = Obj
		else
		(
			local references = GetNodeReferences Obj
			for ref_obj in references do
			(
				if findItem unique_instances ref_obj != 0 then
					instance_obj = ref_obj
			)
		)
		
		YawAngle =GetYaw Obj.transform as string
		PitchAngle= GetPitch Obj.transform as string
		RollAngle= GetRoll Obj.transform as string
		PosX = Obj.Pos.x as string
		PosY = Obj.Pos.z as string
		PosZ = Obj.Pos.y as string
		
		Pos = stringStream ""
		format "% % %" PosX PosY PosZ to:Pos
		strPos = Pos as string
		
		ScaleStr = stringStream ""
		format "% % %" Obj.Scale.x Obj.Scale.y Obj.Scale.z to:ScaleStr
		strScale = ScaleStr as string
		
		tag1 = stringStream ""
		format "\t<MeshInstance name=\"%\" core=\"%\" pos=\"%\" yaw=\"%\"  pitch=\"%\" roll=\"%\" scale=\"%\"/>\n"  Obj.name instance_obj.name strPos YawAngle PitchAngle RollAngle strScale to:tag1
		strTag1 = tag1 as string
		
		format strTag1 to:ostream		
	)
	
	format "</RenderableObjects>\n" to:ostream
	close ostream
)

fn ExportTextureFile AllObj DestPath = 
(

	if AllObj.count == 0 then 
	(
		format "Empty selection. Select any object \n"
		return -1
	)

	for Obj in AllObj do
	(
		maxOps.cloneNodes Obj cloneType:#instance newNodes:&newObj #nodialog
		
		try
		(
			convertToMesh newObj
			actualObj = newObj[1]
			filename = filenameFromPath actualObj.material.diffuseMap.filename
			DestPath = DestPath + "/" + filename
			
			copyFile Obj.material.diffuseMap.filename DestPath
		)
		catch
		(
			format "Error while exporting object\n"
		)
		delete newObj
	)		
)

--UTIL

fn GetYaw transform =
(
	return (transform as eulerangles).z
)

fn GetPitch transform =
(
	return (transform as eulerangles).x
)
fn GetRoll transform =
(
	return (transform as eulerangles).y
)
	
fn DoesFolderExist f create:true =
(
	local val
	val = if (doesFileExist f) and (getfileattribute f #directory) then true else false
	if not val and create then
	(
		val = makeDir f
	)
	val
 )	
	
	
fn GetAllInstances = 
(
	allInsts = #()
	for obj in geometry do
	(
		if (InstanceMgr.GetInstances obj &instances) > 1 do
		(
			if findItem allInsts obj == 0 do
				join allInsts instances
		)
	)
    allInsts.wirecolor = green
	return allInsts
)

fn GetNodeReferences node =
(
	local inst
	InstanceMgr.GetInstances node &inst
	return for n in inst where not (areNodesInstances node n) collect n
)

fn GetReferences = 
(
	--if nodes == unsupplied do nodes = getCurrentSelection()
	local refnodes = #()
	for node in geometry where findItem refnodes node == 0 do 
	(
		for ref in (GetNodeReferences node) where findItem nodes ref == 0 do append refnodes ref 
	)
	makeUniqueArray refnodes
	return refnodes
)
	
fn GetUniqueInstances = 
(
	local uniqueInstance = #()
	for obj in geometry do
	(
		
		if (InstanceMgr.GetInstances obj &instances) > 1 then
		(
			if findItem uniqueInstance instances[1] == 0 do
				append uniqueInstance instances[1]
		)
		else
		(
			append uniqueInstance obj
		)
	)
    --allInsts.wirecolor = green
	--print uniqueInstance
	return uniqueInstance
)	