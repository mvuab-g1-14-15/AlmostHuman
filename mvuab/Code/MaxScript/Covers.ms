fileIn "Utils.ms"
fileIn "XMLStruct.ms"

--export de la class
fn ExportCovers filenameXML =
(	
	try
	(
		-- create the document and set the filename
		xDoc = XMLDocument filenameXML
		
		--create the base node and append the node to the document
		local l_RootNode = xDoc.getNewNode "covers"
		xDoc.AppendNode l_RootNode
		
		local AllCovers = GetCovers()
				
		if AllCovers.count == 0 then 
		(
			format "There are no Covers \n"
			return -1
		)
		
		for Obj in AllCovers do
		(
			l_Name = getUserProp Obj "Cover_Name"
			l_Width = Obj.width
			l_Height = Obj.height
			l_Length = Obj.length
		
			local l_NewCoverNode = xDoc.getNewNode "cover"			
			-- append the new node static_mesh to the root node static_meshs
			l_RootNode.AppendNode l_NewCoverNode 

			l_NewCoverNode.AddAttributeAsString "name" l_Name	
			l_NewCoverNode.AddAttributeAsString "obj_name" Obj.name	
			l_NewCoverNode.AddAttributeCoordinate "pos" Obj.Pos
			l_NewCoverNode.AddAttributeFloat "width" l_Width
			l_NewCoverNode.AddAttributeFloat "height" l_Height
			l_NewCoverNode.AddAttributeFloat "length" l_Length
		)
	)
	catch
	(
		format "Error generating cover XML\n"
	)
	return xDoc.PrettySaveXML()
)