
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
    --allInsts.wirecolor = green
	--print uniqueInstance
	return uniqueInstance
)

fn IsEmptyDestPath SrcPath =
(
	if SrcPath == "" then
	(
		format "Destination Path undefined, please choose a path \n"
		return -1 
	)
)

fn ExportSelectedObjectsMeshes aListObjects MeshDir =
(
	for Obj in aListObjects do
	(
		try
		(
			filename = MeshDir +"\\"+ Obj.name + ".mesh"--poner parametrico
			local file = fopen filename "wb"
			local l_ObjectMesh = SMesh()
			l_ObjectMesh.InitFromMaxObject Obj
			l_ObjectMesh.WriteToFile file
			fclose file
		)
		catch
		(
			format "%\n" (getCurrentException())
		)
	)
	
	format "Check the meshes directory: % \n" MeshDir
)
	