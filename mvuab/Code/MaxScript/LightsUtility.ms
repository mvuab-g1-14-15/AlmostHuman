filein "Lights.ms"

utility LightsUtility "Lights Utility" width:235 height:487
(
	button SetPathButton "Select Path" pos:[5,5] width:150 height:25
	label PathLabel "" pos:[5,35] width:150 height:25
	colorPicker AmbientLightPicker "Ambient light" pos:[5,60] width:150 height:20
	button ExportAllLights "Export All lights" pos:[5, 90] width:150 height:25
	button ExportSelectedLights "Export Selected lights" pos:[5, 120] width:150 height:25
	
	-----------------------------------------------------------------
	on LightsUtility open do
	(
		PathLabel.text = "E:\Master"
		AmbientLightPicker.color = ambientColor
	)
	on ExportSelectedLights pressed do
	(
		--export lights
		format "%" AmbientLightPicker.color
		local LightDir = PathLabel.text + "\\lights.xml"
		ExportLights LightDir selection
	)
	on ExportAllLights pressed do
	(
		--export lights
		local LightDir = PathLabel.text + "\\lights.xml"
		ExportLights LightDir lights
	)
	on AmbientLightPicker pressed do
	(
		ambientColor = AmbientLightPicker.color
	)
)
