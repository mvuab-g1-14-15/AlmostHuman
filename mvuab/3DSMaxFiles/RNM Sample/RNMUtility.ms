fileIn "SetRNM.ms"
fileIn "GenerateRNM.ms"

utility RNMUtility "RNM Utility" width:162 height:130
(
	button genRNMbtn "Generate RNM" pos:[24,24] width:118 height:29
	button setRNMbtn "Set RNM" pos:[24,72] width:118 height:29
	on genRNMbtn pressed  do
	(
		createDialog GenerateRNMRollout
	)
	on setRNMbtn pressed  do
	(
		createDialog SetRNMRollout
	)
)
