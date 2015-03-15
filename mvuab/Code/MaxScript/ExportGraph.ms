
m_MapNodes = dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int16]"
m_NodeConnection = #()
m_Nodes = #()

spl = selection[1]

if classOf spl == SplineShape or classOf spl == line then
(
	count = 1
	for s = 1 to (numsplines spl) do
	(
		local PrevKnot = -1
		for k = 1 to (numknots spl s) do
		(
			local l_KnotPos = (getKnotPoint spl s k) as String
			
			if (m_MapNodes.ContainsKey (l_KnotPos)) == false then
			(
				m_MapNodes.Add l_KnotPos count
				format "Node not found, appending at id %\n" count
				if PrevKnot != -1 then
				(
					format "Count: %\nm_nodeConnection.count: %\n" count m_NodeConnection.count
					while count > m_NodeConnection.count do
						append m_NodeConnection #()
					append m_NodeConnection[count] PrevKnot
				)
				append m_Nodes l_KnotPos
				--format "spline_%_knot_%_pos = %\n" s k (getKnotPoint spl s k)	
				--format "Spline_%_knot_%_pos = %\n Added" s k (getKnotPoint spl s k)
				PrevKnot = count
				count = count + 1
			)
			else
			(
				l_Id = m_MapNodes.Item[l_KnotPos]
				format "Node found, id %\n" l_Id
				if PrevKnot != -1 then
					append m_NodeConnection[l_Id] PrevKnot
				PrevKnot = l_Id
			)
		)
	)
)

m_Arcs = #()
while m_Arcs.count < m_Nodes.count do
	append m_Arcs #()
for i = 1 to m_NodeConnection.count do
(
	l_Node1 = i
	l_ActualNode = m_NodeConnection[i]
	for j = 1 to l_ActualNode.count do
	(
		l_Node2 = l_ActualNode[j]
		append m_Arcs[l_Node1] l_Node2
		append m_Arcs[l_Node2] l_Node1
	)
)

format "RAW Data:\n"
format "\tNodes (#%): %\n" m_Nodes.count m_Nodes
format "\tArcs: %\n\n" m_Arcs
for i = 1 to m_Nodes.count do
(
	format "Node: %\n" m_Nodes[i]
	for l_arc in m_Arcs[i] do
	(
		format "\tArc: %\n" l_arc
	)
)