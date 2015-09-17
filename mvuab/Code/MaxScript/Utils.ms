
fn GetPitch rot =
(
	local a = QuatToEuler2(rot)
	return degToRad(a.x)
)

fn GetYaw rot =
(
	local a = QuatToEuler2(rot)
	return degToRad(a.z)
)

fn GetRoll rot =
(
	local a = QuatToEuler2(rot)
	return degToRad(a.y)
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
	for obj in geometry where IsObjExportable obj == true do
	(
		format " Objeto geometrico %" obj
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
	local refnodes = #()
	for node in geometry where findItem refnodes node == 0 do 
	(
		for ref in (GetNodeReferences node) where findItem nodes ref == 0 do append refnodes ref 
	)
	makeUniqueArray refnodes
	return refnodes
)

function GetObjectCore obj uniqueInstanceSceneList =
(
	pos = findItem uniqueInstanceSceneList obj 
	if pos != 0 then (
		return (obj.name)
	) else (
		InstanceMgr.GetInstances obj &instances
		for i = 1 to instances.count do (
			pos = findItem uniqueInstanceSceneList instances[i]
			if pos != 0	then (
				return (uniqueInstanceSceneList[pos].name)
			)
		)
	)
)


/* Method to extract core instances from selection (loops all selected objects of scene) */
fn GetUniqueInstances = 
(
	local uniqueInstance = #()
	for obj in selection do
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
	return uniqueInstance
)
/* Method to extract core instances from scene (loops all geometry of scene) */
fn GetUniqueInstancesScene = 
(
	local uniqueInstanceScene = #()
	for obj in geometry where IsObjExportable obj == true do
	(
		if (InstanceMgr.GetInstances obj &instances) > 1 then
		(
			if findItem uniqueInstanceScene instances[1] == 0 do
				append uniqueInstanceScene instances[1]
		)
		else
		(
			append uniqueInstanceScene obj
		)
	)
	return uniqueInstanceScene
)

fn IsEmptyDestPath SrcPath =
(
	if SrcPath == "" then
	(
		format "Destination Path undefined, please choose a path \n"
		return -1 
	)
)

fn IsEmptyInstanceArray UniqueInstanceArray =
(
	if UniqueInstanceArray.count == 0 then 
	(
		format "Empty selection. Select any object \n"
		return -1
	)
)

fn IsEmptyLevelList LevelListText =
(
	if LevelListText == "" then 
	(
		format "Empty level. Please select level to export data \n"
		return -1
	)
)

fn IsObjExportable Obj =
(
	local isExportable = false
	
	if (getUserProp Obj "Trigger_Name" != undefined AND getUserProp Obj "Trigger_Name" != "") then
	(
		isExportable = false
	)
	else if (getUserProp Obj "Cover_Name" != undefined AND getUserProp Obj "Cover_Name" != "") then
	(
		isExportable = false
	)
	else if (getUserProp Obj "Path_Name" != undefined AND getUserProp Obj "Path_Name" != "") then
	(
		isExportable = false
	)
	else if(classof Obj == CATParent OR classof Obj == HubObject OR classof Obj == CATBone) then
	(
		isExportable = false
	)
	else
	(
		isExportable = true
	)

	return isExportable
)

fn IsLightExportable Obj =
(
	l_Property = getUserProp Obj "no_export_lights"
	
	format "Light name % and IsExportable %" Obj l_Property
	
	if l_Property == true then
	(	
		return false
	)
	else
	(
		return true
	)
)

fn GetAllExportableObjects =
(
	l_ExportableObjects = #()

	for Obj in Geometry do 
	(
		if IsObjExportable Obj then
		(
			append l_ExportableObjects Obj
		)
	)
	return l_ExportableObjects
)


fn GetNoExportableObjects =
(
	l_NoExportableObjects = #()

	for Obj in Geometry do 
	(
		if IsObjExportable Obj == false then
		(
			append l_NoExportableObjects Obj
		)
	)
	return l_NoExportableObjects
)

fn GetTriggers =
(
	l_Triggers = #()
	for Obj in GetNoExportableObjects() where getUserProp Obj "Trigger_Name" != "" AND getUserProp Obj "Trigger_Name" != undefined do
	(
		append l_Triggers Obj
	)
	
	return l_Triggers
)

fn GetPaths =
(
	l_Paths = #()
		
	l_IndexPaths=dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int16]"
	
	for Obj in GetNoExportableObjects() where getUserProp Obj "Path_Name"	 != "" AND getUserProp Obj "Path_Name" != undefined do
	(
		format "Object path: %\n" Obj.name
		
		l_PathName = getUserProp Obj "Path_Name"
		l_Index = 0
		if l_IndexPaths.ContainsKey l_PathName == false then
		(
			l_Index = l_IndexPaths.count + 1
			l_IndexPaths.Add l_PathName l_Index
			append l_Paths #()
		)
		else
			l_Index = l_IndexPaths.Item[l_PathName]
		
		l_IndexNode = getUserProp Obj "Path_Num_Node"
		
		--format "Material ID: %\n" material_id
		while l_IndexNode >  l_Paths[l_Index].count do
		(
			append  l_Paths[l_Index] 0
		)
		
		l_Paths[l_Index][l_IndexNode] = Obj
	)
	
	return l_Paths
)

fn GetBillboards =
(
	
	l_BillboardName = #()
	
	for Obj in GetNoExportableObjects() where getUserProp Obj "Billboard_Core" != "" AND getUserProp Obj "Billboard_Core" != undefined do
	(
		append l_BillboardName Obj
	)
	
	return l_BillboardName
	
)

fn GetParticles =
(
	
	l_ParticleName = #()
	
	for Obj in GetNoExportableObjects() where getUserProp Obj "Particle_Core" != "" AND getUserProp Obj "Particle_Core" != undefined do
	(
		append l_ParticleName Obj
	)
	
	return l_ParticleName
	
)

