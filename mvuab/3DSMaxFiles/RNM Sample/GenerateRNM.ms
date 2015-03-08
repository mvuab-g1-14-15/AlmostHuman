function SetRNMAxisMaterial material _color = 
(
	material.bumpMap=Normal_Bump()
	material.bumpMapAmount=100
	material.bumpMapEnable=true
	material.bumpMap.method=3
	material.bumpMap.normal_map=Checker()
	material.bumpMap.normal_map.color1=_color
	material.bumpMap.normal_map.color2=_color
)

function SetRNMAxis Obj color =
(
	if (classof Obj.material)==Standardmaterial then
	(
		SetRNMAxisMaterial Obj.material color
	)
	else
	(
		for i=1 to Obj.material.count do
		(
			SetRNMAxisMaterial Obj.material[i] color
		)
	)
)

rollout GenerateRNMRollout "Generate RNM" width:279 height:177
(
	button Xbtn "Generate X" pos:[88,31] width:101 height:36
	button Ybtn "Generate Y" pos:[90,70] width:101 height:36
	button Zbtn "Generate Z" pos:[88,112] width:101 height:36
	on Xbtn pressed  do
	(
		SetRNMAxis $ (color 231 127 201)
	)
	
	on Ybtn pressed  do
	(
		SetRNMAxis $ (color 75 217 201)
	)
	
	on Zbtn pressed  do
	(
		SetRNMAxis $ (color 75 37 201)
	)
)