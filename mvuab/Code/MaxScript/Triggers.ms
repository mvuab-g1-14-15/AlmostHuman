fileIn "Utils.ms"
fileIn "XMLStruct.ms"

--export de la class
fn ExportTriggers filenameXML =
(	
	try
	(
		-- create the document and set the filename
		xDoc = XMLDocument filenameXML
		
		--create the base node and append the node to the document
		local l_RootNode = xDoc.getNewNode "triggers"
		xDoc.AppendNode l_RootNode
		
		local AllTriggers = GetTriggers()
				
		if AllTriggers.count == 0 then 
		(
			format "There are no Triggers \n"
			return -1
		)
		
		for Obj in AllTriggers do
		(
			l_Name = getUserProp Obj "Trigger_Name"
			l_Width = Obj.width
			l_Height = Obj.height
			l_Length = Obj.length
		
			local l_NewTriggerNode = xDoc.getNewNode "trigger"			
			-- append the new node static_mesh to the root node static_meshs
			l_RootNode.AppendNode l_NewTriggerNode 

			l_NewTriggerNode.AddAttributeAsString "name" l_Name	
			l_NewTriggerNode.AddAttributeAsString "obj_name" Obj.name	
			l_NewTriggerNode.AddAttributeCoordinate "pos" Obj.Pos
			l_NewTriggerNode.AddAttributeFloat "width" l_Width
			l_NewTriggerNode.AddAttributeFloat "height" l_Height
			l_NewTriggerNode.AddAttributeFloat "length" l_Length
		)
	)
	catch
	(
		format "Error generating trigger XML\n"
	)
	return xDoc.PrettySaveXML()
)