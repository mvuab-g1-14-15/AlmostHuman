global UABVtxsNormals=#() 		
 			
function UABClearNormalsArray = 		
(
	while UABVtxsNormals.count>0 do 		
	(
		deleteItem UABVtxsNormals 1 		
	)
)
		
function IsSmoothingGroupEnabled IdGroup Value = 		
(
	local ValueMask=2^(IdGroup-1) 		
	return (bit.and Value ValueMask)==ValueMask 		
)

function UABGetVertexNormal obj IdVertex SmoothValue FaceNormal = 		
( 		
	local HasNormal=false 		
	local Normal=point3 0 0 0 		
	for b=1 to 32 do 		
	( 		
		if((IsSmoothingGroupEnabled b SmoothValue)==true) then 		
		( 		
			Normal=Normal+UABVtxsNormals[IdVertex][b] 		
			HasNormal=true 		
		) 		
	) 		
	if HasNormal==false then 		
	( 		
		Normal=FaceNormal 		
	) 		
	return Normal 		
)
	
		
function UABCalcVertexsNormals obj = 		
(
	UABClearNormalsArray() 		
	local NumVtxs=getNumVerts obj 		
	for b=1 to NumVtxs do 		
	(
		UABVtxsNormals[b]=#() 		
		for t=1 to 32 do 		
		(
			UABVtxsNormals[b][t]=point3 0 0 0 		
		)
	)
	local NumFaces=getNumFaces obj 		
	local l_Transform = obj.transform
	local matrix=(obj.transform.rotation*obj.objectoffsetrot) as matrix3
	matrix.pos=obj.transform.pos
	local InvTransform=inverse matrix
	
	for IdFace=1 to NumFaces do 		
	(
		local IdxsFace=getFace obj IdFace 		
		local Vtx1=(getVert obj IdxsFace.x)*InvTransform 		
		local Vtx2=(getVert obj IdxsFace.y)*InvTransform		
		local Vtx3=(getVert obj IdxsFace.z)*InvTransform		
		local FaceNormal=getFaceNormal obj IdFace 		
		local SmoothValue=getFaceSmoothGroup obj IdFace 		
		for b=1 to 32 do 		
		(
			if((IsSmoothingGroupEnabled b SmoothValue)==true) then 		
			(
				UABVtxsNormals[IdxsFace.x][b]=UABVtxsNormals[IdxsFace.x][b] + 		
				FaceNormal		
				UABVtxsNormals[IdxsFace.y][b]=UABVtxsNormals[IdxsFace.y][b] + 		
				FaceNormal 		
				UABVtxsNormals[IdxsFace.z][b]=UABVtxsNormals[IdxsFace.z][b] + 		
				FaceNormal 		
			)
		)
	)
)