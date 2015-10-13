fileIn "Utils.ms"
fileIn "XMLStruct.ms"


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
			format "There are no particles \n"
			return -1
		)
		
		local l_Count = 0
		
		for Obj in AllParticles do
		(
			l_Core = getUserProp Obj "Particle_Core"
			
			l_Name = (l_Core +""+ (l_Count) as String)
			format "Obj: % & l_Name: %\n" Obj l_Name
			
			local l_NewPath = xDoc.getNewNode "particle_instance"	
			l_RootNode.AppendNode l_NewPath 
			l_NewPath.AddAttributeAsString "name" (l_Name as String)
			l_NewPath.AddAttributeAsString "core" (l_Core as String)
			l_NewPath.AddAttributeCoordinate "pos" Obj.Pos
			l_NewPath.AddAttributeAsString "active" "true"
			l_NewPath.AddAttributeAsString "sound" "true"
			
			l_Count = l_Count + 1
		)
	)
	catch
	(
		format "Error generating particles XML\n"
	)
	return xDoc.PrettySaveXML()
)