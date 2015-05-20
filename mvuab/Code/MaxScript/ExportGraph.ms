--includes
fileIn "XMLStruct.ms"
fileIn "Utils.ms"

struct RouteAndGraph
(
	m_MapNodes,
	m_NodeConnection,
	m_Nodes,
	
	fn Calculate =
	(
		m_MapNodes = dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int16]"
		m_NodeConnection = #()
		m_Nodes = #()

		spl = selection[1]

		/*local l_Transform = spl.transform
		local matrix=(spl.transform.rotation*spl.objectoffsetrot) as matrix3
		matrix.pos=spl.transform.pos
		local InvTransform=inverse matrix	*/
		
		if classOf spl == SplineShape or classOf spl == line then
		(
			local count = 1
			for s = 1 to (numsplines spl) do
			(
				local PrevKnot = -1
				for k = 1 to (numknots spl s) do
				(
					local l_KnotPos = (getKnotPoint spl s k)
					local l_KnotPosStr = (getKnotPoint spl s k) as String
					
					if (m_MapNodes.ContainsKey (l_KnotPosStr)) == false then
					(
						m_MapNodes.Add l_KnotPosStr count
						format "Node not found, appending at id %\n" count
						if PrevKnot != -1 then
						(
							format "Count: %\nm_nodeConnection.count: %\n" count m_NodeConnection.count
							while count > m_NodeConnection.count do
								append m_NodeConnection #()
							append m_NodeConnection[count] PrevKnot
						)
						append m_Nodes l_KnotPos
						
						PrevKnot = count
						count = count + 1
					)
					else
					(
						l_Id = m_MapNodes.Item[l_KnotPosStr]
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
	)
	
	fn ExportGraphXML fileNameXML =
	(
		xDoc = XMLDocument filenameXML
		--create the base node and append the node to the document
		local l_RootNode = xDoc.getNewNode "graph"
		xDoc.AppendNode l_RootNode

		format "RAW Data:\n"
		format "\tNodes (#%): %\n" m_Nodes.count m_Nodes
		format "\tArcs: %\n\n" m_Arcs
		for i = 1 to m_Nodes.count do
		(
			
			local l_NewGraphNode = xDoc.getNewNode "node"
			l_RootNode.AppendNode l_NewGraphNode
			l_NewGraphNode.AddAttributeFloat "id" i
			
			local l_Pos = point3 m_Nodes[i].x m_Nodes[i].y m_Nodes[i].z
			l_NewGraphNode.AddAttributeCoordinate "pos" l_Pos
			
			format "Node: %\n" m_Nodes[i]
			for l_arc in m_Arcs[i] do
			(
				format "\tArc: %\n" l_arc
				
				local l_NewArcGraphNode = xDoc.getNewNode "weight"
				l_RootNode.AppendNode l_NewArcGraphNode
				l_NewArcGraphNode.AddAttributeFloat "src" i	
				l_NewArcGraphNode.AddAttributeFloat "dst" l_arc
				l_NewArcGraphNode.AddAttributeFloat "value" 1
				
			)
		)
		return xDoc.PrettySaveXML()
	)
	
	fn ExportRouteXML fileNameXML =
	(
	)
)