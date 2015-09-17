fileIn "Utils.ms"
fileIn "XMLStruct.ms"


/*

particules.xml
<particles_instances>
	<particle_instance name="smoke01" core="smoke" pos="64 -16.5 -60"/> 
	<particle_instance name="smoke021" core="smoke2" pos="64 -16.5 -70"/> 
	<particle_instance name="smoke02" core="smoke" pos="66 -16.5 -44"/>
	<particle_instance name="ray01" core="rays" pos="62 -17.5 -28"/>
	<particle_instance name="charge01" core="charge" pos="54 -14 -30"/>
</particles_instances>

*/

--export de la class
fn ExportParticles filenameXML =
(	
	try
	(
		-- create the document and set the filename
		xDoc = XMLDocument filenameXML
		
		--create the base node and append the node to the document
		local l_RootNode = xDoc.getNewNode "particles_instances"
		xDoc.AppendNode l_RootNode
		
		local AllParticles = GetParticles()
		
		if AllParticles.count == 0 then 
		(
			format "There are no Paths \n"
			return -1
		)
		
		local l_Count = 0
		
		for Obj in AllParticles do
		(
			l_Name = getUserProp Obj[1] "Particle_Core"
			
			format "Obj: % & l_Name: %\n" Obj l_Name
			
			local l_NewPath = xDoc.getNewNode "Particle_Core"	
			l_RootNode.AppendNode l_NewPath 
			l_NewPath.AddAttributeAsString "name" (l_Name as String)
			l_NewPath.AddAttributeAsString "core" (l_Name as String)
			l_NewPath.AddAttributeAsString "pos" (l_Name as String)
			
			lCount = lCount + 1
		)
	)
	catch
	(
		format "Error generating particle XML\n"
	)
	return xDoc.PrettySaveXML()
)