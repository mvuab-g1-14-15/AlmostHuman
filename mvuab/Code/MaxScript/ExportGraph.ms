
m_MapNodes = dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int16]"
m_NodeConnection = #()
m_Nodes = #()

spl = selection[1]

if classOf spl == SplineShape then
(
	count = 1
	local PrevKnot
	for s = 1 to (numsplines spl) do
	(
		for k = 1 to (numknots spl s) do (
			local l_KnotPos = (getKnotPoint spl s k) as String
			
			if (m_MapNodes.ContainsKey (l_KnotPos)) == false then
			(
				m_MapNodes.Add l_KnotPos count
				append m_NodeConnection #(PrevKnot)
				append m_Nodes l_KnotPos
				--format "spline_%_knot_%_pos = %\n" s k (getKnotPoint spl s k)	
				--format "Spline_%_knot_%_pos = %\n Added" s k (getKnotPoint spl s k)
				count = count + 1
			)
			else
			(
				l_Id = m_MapNodes.Item[l_KnotPos]
				append m_NodeConnection[l_Id] PrevKnot
			)
			PrevKnot = count
		)
	)
)

for i = 1 to m_Nodes.count do
(
	format "Node: %\n" m_Nodes[i]
	for l_arc in m_NodeConnection[i] do
	(
		format "\tArc: %\n" l_arc
	)
)