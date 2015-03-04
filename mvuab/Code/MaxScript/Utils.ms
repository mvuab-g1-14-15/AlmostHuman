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