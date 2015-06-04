fileIn "Utils.ms"
fileIn "XMLStruct.ms"

--export de la class
fn ExportPaths filenameXML =
(	
	try
	(
		-- create the document and set the filename
		xDoc = XMLDocument filenameXML
		
		--create the base node and append the node to the document
		local l_RootNode = xDoc.getNewNode "Paths"
		xDoc.AppendNode l_RootNode
		
		local AllPaths = GetPaths()
		
		if AllPaths.count == 0 then 
		(
			format "There are no Paths \n"
			return -1
		)
		
		for Obj in AllPaths do
		(
			l_Name = getUserProp Obj[1] "Path_Name"
			
			format "Obj: % & l_Name: %\n" Obj l_Name
			
			local l_NewPath = xDoc.getNewNode "path"	
			l_RootNode.AppendNode l_NewPath 
			l_NewPath.AddAttributeAsString "name" (l_Name as String)

			for EachNode in Obj do
			(
				local l_NewPathNode = xDoc.getNewNode "node"	
				l_NewPath.AppendNode l_NewPathNode 
				l_NewPathNode.AddAttributeCoordinate "pos" EachNode.Pos
			)
		)
	)
	catch
	(
		format "Error generating trigger XML\n"
	)
	return xDoc.PrettySaveXML()
)