
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
	for obj in geometry do
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
	for obj in geometry do
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