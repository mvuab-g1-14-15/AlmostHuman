fileIn "Utils.ms"
fileIn "XMLStruct.ms"

--export de la class
fn ExportTriggers filenameXML =
(	
	--try
	--(
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
			-- <trigger name="PuntoEnergia" position="50.06 -16.515 -26.52" shape="box" size="4 8 4" enter_event="true" stay_event="false" leave_event="true" enter_script="ShowImage()" stay_script="ShowImage()" leave_script="ShowImage()" group="0" paint="true" color="1.0 0.0 0.0 0.5" technique="RenderForwardDebugShapeTechnique"/>
			local l_Name = getUserProp Obj "Trigger_Name"
			local l_Shape =  (classof Obj) as String

			local l_OnEnter = getUserProp Obj "Enter_Event"
			local l_OnStay = getUserProp Obj "Stay_Event"
			local l_OnLeave = getUserProp Obj "Leave_Event"
			
			local l_OnEnterFunc = getUserProp Obj "Enter_Event_Function"
			local l_OnStayFunc = getUserProp Obj "Stay_Event_Function"
			local l_OnLeaveFunc = getUserProp Obj "Leave_Event_Function"
			
			--format "Obj: % \n " Obj.name
			
			local l_NewTriggerNode = xDoc.getNewNode "trigger"			
			-- append the new node static_mesh to the root node static_meshs
			l_RootNode.AppendNode l_NewTriggerNode 
			
			l_NewTriggerNode.AddAttributeAsString "name" l_Name
			l_NewTriggerNode.AddAttributeAsString "shape" l_Shape
			l_NewTriggerNode.AddAttributeCoordinate "position" Obj.Pos
			l_NewTriggerNode.AddAttributeCoordinate "size" (point3 Obj.width Obj.height Obj.length)
			-- AddAttributeBool exists? YES
			-- Todo a falso y nulo de momento. A lo mejor es mejor ni ponerlo. Hay que pensar esta parte.
			-- O poner con user properties si se usan alguno de los eventos y que el nombre de la funcion sea a partir del nombre del trigger, supongo que es mejor asi.
			l_NewTriggerNode.AddAttributeBool "enter_event" l_OnEnter
			l_NewTriggerNode.AddAttributeBool "stay_event" l_OnStay
			l_NewTriggerNode.AddAttributeBool "leave_event" l_OnLeave
			l_NewTriggerNode.AddAttributeAsString "enter_script" l_OnEnterFunc
			l_NewTriggerNode.AddAttributeAsString "stay_script" l_OnStayFunc
			l_NewTriggerNode.AddAttributeAsString "leave_script" l_OnLeaveFunc
			l_NewTriggerNode.AddAttributeFloat "group" 0
			l_NewTriggerNode.AddAttributeBool "paint" true
			--AddAttributeRGB exists? YES. Get the color of the obj. We need the alpha?
			l_NewTriggerNode.AddAttributeRGBa "color" Obj.wirecolor 
			l_NewTriggerNode.AddAttributeAsString "technique" "RenderForwardDebugShapeTechnique"
		)
	--)
	--catch
	--(
	--	format "Error generating trigger XML\n"
	--)
	return xDoc.PrettySaveXML()
)
