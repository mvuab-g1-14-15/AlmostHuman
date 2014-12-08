rollout UABSetProperties "UABSetProperties" width:264 height:406
(
	GroupBox RenderGroup "Render" pos:[19,19] width:227 height:112
	checkbox ExportCheckRender "Export" pos:[26,36] width:61 height:17
	dropdownList ExportListRender "" pos:[108,33] width:123 height:21 enabled:false items:#("instance_mesh", "animated_mesh")
	checkbox VisibleCheckRender "Visible" pos:[26,56] width:51 height:17
	checkbox LayerCheckRender "Layer" pos:[27,75] width:51 height:17
	dropdownList LayerListRender "" pos:[108,71] width:123 height:21 enabled:false items:#("solid", "alpha_objects", "alpha_blend_objects", "particles")
	label AnimModelRender "Anim. model" pos:[32,104] width:64 height:18
	edittext AnimModelEditText "" pos:[108,101] width:120 height:19
	
	GroupBox PhysicsGroup "Physics" pos:[18,152] width:226 height:67
	checkbox GenerateCheckPhysics "Generate" pos:[29,167] width:66 height:13
	checkbox LayerCheckPhysics "Layer" pos:[30,194] width:62 height:13
	dropdownList GenerateListPhysics "" pos:[109,162] width:123 height:21 enabled:false items:#("triangle_mesh", "convex_shape", "box_shape", "sphere_shape", "plane_shape", "capsule_shape")
	dropdownList LayerListPhysics "" pos:[109,189] width:123 height:21 enabled:false
	
	GroupBox LogicGroup "Logic" pos:[20,230] width:223 height:69
	checkbox GenerateCheckLogic "Generate" pos:[29,248] width:66 height:13
	dropdownList GenerateListLogic "" pos:[109,244] width:123 height:21 enabled:false items:#("", "event_box", "event_sphere", "stairs", "grippable_edges", "edges_where_to_be_dropped")
	
	button GetButton "Get" pos:[14,369] width:77 height:29
	button SetButton "Set" pos:[171,369] width:77 height:29
	
	on ExportCheckRender changed theState do
	if (ExportCheckRender.checked)==true then
	(
		ExportListRender.enabled=true
	)
	else
	(
		ExportListRender.enabled=false
	)
	
	on LayerCheckRender changed theState do
	if (LayerCheckRender.checked)==true then
	(
		
		LayerListRender.enabled=true
	)
	else
	(
		LayerListRender.enabled=false
	)
	
	on GenerateCheckPhysics changed theState do
	if (GenerateCheckPhysics.checked)==true then
	(
		
		GenerateListPhysics.enabled=true
	)
	else
	(
		GenerateListPhysics.enabled=false
	)
	
	on LayerCheckPhysics changed theState do
	if (LayerCheckPhysics.checked)==true then
	(
		
		LayerListPhysics.enabled=true
	)
	else
	(
		LayerListPhysics.enabled=false
	)
	
	on GenerateCheckLogic changed theState do
	if (GenerateCheckLogic.checked)==true then
	(
		
		GenerateListLogic.enabled=true
	)
	else
	(
		GenerateListLogic.enabled=false
	)
	
	on GetButton pressed do
	(
		for i=1 to $.count do
		(
			local Property=getUserProp $[i] "export_render"
			if Property!="" then
			(
				if Property!=undefined then
				(
					ExportCheckRender.checked=true
					for i=1 to ExportListRender.items.count do
					(
						if ExportListRender.items[i]==Property then
						(
							ExportListRender.selection=i
							ExportListRender.enabled=true
						)
					)
				)
			)
			else
			(
				ExportCheckRender.checked=false
				ExportListRender.selection=1
				ExportListRender.enabled=false
			)
			Property=(getUserProp $[i] "visible_render")
			if Property!=true then
			(
				if Property!=undefined then
				(
					VisibleCheckRender.checked=false
				)
			)
			else
			(
				VisibleCheckRender.checked=true
			)
			Property=getUserProp $[i] "layer_render"
			if Property!="" then
			(
				if Property!=undefined then
				(
					LayerCheckRender.checked=true
					for i=1 to LayerListRender.items.count do
					(
						if LayerListRender.items[i]==Property then
						(
							LayerListRender.selection=i
							LayerListRender.enabled=true
						)
					)
				)
			)
			else
			(
				LayerCheckRender.checked=false
				LayerListRender.selection=1
				LayerListRender.enabled=false
			)
			Property=getUserProp $[i] "anim_model"
			if Property!="" then
			(
				if Property!=undefined then
				(
					AnimModelEditText.text=Property
				)
			)
			else
			(
				AnimModelEditText.text=""
			)
			Property=getUserProp $[i] "generate_physics"
			if Property!="" then
			(
				if Property!=undefined then
				(
					GenerateCheckPhysics.checked=true
					for i=1 to GenerateListPhysics.items.count do
					(
						if GenerateListPhysics.items[i]==Property then
						(
							GenerateListPhysics.selection=i
							GenerateListPhysics.enabled=true
							
						)
					)
				)
			)
			else
			(
				GenerateCheckPhysics.checked=false
				GenerateListPhysics.selection=1
				GenerateListPhysics.enabled=false
			)
			Property=getUserProp $[i] "layer_physics"
			if Property!="" then
			(
				if Property!=undefined then
				(
					LayerCheckPhysics.checked=true
					for i=1 to LayerListPhysics.items.count do
					(
						if LayerListPhysics.items[i]==Property then
						(
							LayerListPhysics.selection=i
							LayerListPhysics.enabled=true
						)
					)
				)
			)
			else
			(
				LayerCheckPhysics.checked=false
				LayerListPhysics.selection=1
				LayerListPhysics.enabled=false
			)
			Property=getUserProp $[i] "generate_logic"
			if Property!="" then
			(
				if Property!=undefined then
				(
					GenerateCheckLogic.checked=true
					for i=1 to GenerateListLogic.items.count do
					(
						if GenerateListLogic.items[i]==Property then
						(
							GenerateListLogic.selection=i
							GenerateListLogic.enabled=true
						)
					)
				)
			)
			else
			(
				GenerateCheckLogic.checked=false
				GenerateListLogic.selection=1
				GenerateListLogic.enabled=false
			)
		)
	)
	
	on SetButton pressed do
	(
		for i=1 to $.count do
		(
			if (ExportCheckRender.checked)==true then
			(
				setUserProp $[i] "export_render" ExportListRender.items[ExportListRender.selection]
			)
			else
			(
				setUserProp $[i] "export_render" ""
			)
			
			if (VisibleCheckRender.checked)==true then
			(
				setUserProp $[i] "visible_render" true
			)
			else
			(
				setUserProp $[i] "visible_render" false
			)
			
			if (LayerCheckRender.checked)==true then
			(
				setUserProp $[i] "layer_render" LayerListRender.items[LayerListRender.selection]
			)
			else
			(
				setUserProp $[i] "layer_render" ""
			)
			
			if AnimModelEditText.text != "" then
			(
				setUserProp $[i] "anim_model" AnimModelEditText.text
			)
			else
			(
				setUserProp $[i] "anim_model" ""
			)
			
			if (GenerateCheckPhysics.checked)==true then
			(
				setUserProp $[i] "generate_physics" GenerateListPhysics.items[GenerateListPhysics.selection]
			)
			else
			(
				setUserProp $[i] "generate_physics" ""
			)
			
			if (LayerCheckPhysics.checked)==true then
			(
				setUserProp $[i] "layer_physics" LayerListPhysics.items[LayerListPhysics.selection]
			)
			else
			(
				setUserProp $[i] "layer_physics" ""
			)
			
			if (GenerateCheckLogic.checked)==true then
			(
				setUserProp $[i] "generate_logic" GenerateListLogic.items[GenerateListLogic.selection]
			)
			else
			(
				setUserProp $[i] "generate_logic" ""
			)
		)
		
	)
	
)
createDialog UABSetProperties