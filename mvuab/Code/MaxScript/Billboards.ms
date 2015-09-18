fileIn "Utils.ms"
fileIn "XMLStruct.ms"

/*

billboards.xml
<billboard_instances>
	<billboard_instance name="target_mission01" core="target" pos="64 -16.5 -60"/> 
	<billboard_instances name="teclaR" core="teclaR" pos="64 -16.5 -70"/> 
</billboard_instances>
*/


--export de la class
fn ExportBillboards filenameXML =
(	
	try
	(
		-- create the document and set the filename
		xDoc = XMLDocument filenameXML
		
		--create the base node and append the node to the document
		local l_RootNode = xDoc.getNewNode "billboard_instances"
		xDoc.AppendNode l_RootNode
		
		local AllBillboards = GetBillboards()		
		
		if AllBillboards.count == 0 then 
		(
			format "There are no billboards \n"
			return -1
		)
		
		local l_Count = 0
		
		for Obj in AllBillboards do
		(
			l_Core = getUserProp Obj "Billboard_Core"
			
			l_Name = (l_Core +""+ (l_Count) as String)
			format "Obj: % & l_Name: %\n" Obj l_Name
			
			local l_NewPath = xDoc.getNewNode "billboard_instance"	
			l_RootNode.AppendNode l_NewPath 
			l_NewPath.AddAttributeAsString "name" (l_Name as String)
			l_NewPath.AddAttributeAsString "core" (l_Core as String)
			l_NewPath.AddAttributeCoordinate "pos" Obj.Pos
			
			l_Count = l_Count + 1
		)
	)
	catch
	(
		format "Error generating billboards XML\n"
	)
	return xDoc.PrettySaveXML()
)