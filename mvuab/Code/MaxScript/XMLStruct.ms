DotNet.LoadAssembly "System.Xml"

struct XMLNode(
	Tag,
	Parent,
	DotNetNode,
	
	fn init dnNode = (
		DotNetNode = dnNode
		Tag = dnNode.LocalName
		Parent = dnNode.ParentNode
	),
	fn GetTag = (
		DotNetNode.Name
	),
	fn SetTag newTag =(
		try(DotNetNode.Name = newTag)catch(False)
	),
	fn GetText = (
		DotNetNode.InnerText
	), 
	fn SetText txt = (
		DotNetNode.InnerText = txt
	),
	fn AddAttribute attribute value = (
		DotNetNode.SetAttribute attribute value
	),
	fn AddAttributeCoordinate attribute CoordAtt = 
	(
		pos = stringStream ""
		format "% % %" CoordAtt.x CoordAtt.z CoordAtt.y to:pos
		DotNetNode.SetAttribute attribute pos as string
	),
	fn AddAttribute2DCoordinate attribute CoordAtt = 
	(
		pos = stringStream ""
		format "% %" CoordAtt.x CoordAtt.y to:pos
		DotNetNode.SetAttribute attribute pos as string
	),
	fn AddAttributeRGB attribute RGBAtt = 
	(
		Colors = stringStream ""
		format "% % %" ( RGBAtt.r / 255 ) ( RGBAtt.g / 255 ) ( RGBAtt.b / 255 ) to:Colors
		DotNetNode.SetAttribute attribute Colors as string
	),
	fn AddAttributeRGBNormalized attribute RGBAtt = 
	(
		Colors = stringStream ""
		format "% % %" ( RGBAtt.r / 255 ) ( RGBAtt.g / 255 ) ( RGBAtt.b / 255 ) to:Colors
		DotNetNode.SetAttribute attribute Colors as string
	),
	fn AddAttributeAsString attribute NoStringValueAtt = (
		DotNetNode.SetAttribute attribute NoStringValueAtt as string
	),
	fn AddAttributeFloat attribute FloatAtt = (
		l_FloatStr = FloatAtt as string
		DotNetNode.SetAttribute attribute l_FloatStr
	),
	fn AddAttributeBool attribute BoolAtt = (
		l_BoolStr = BoolAtt as string
		DotNetNode.SetAttribute attribute l_BoolStr
	),
	fn GetAttribute attribute = (
		DotNetNode.GetAttribute attribute
	),
	fn SetAttributeValue attribute value = (
		DotNetNode.SetAttribute attribute value
	),
	fn DeleteAttribute attribute = (
		DotNetNode.RemoveAttribute attribute
	),
	fn GetAllAttributes = (
		ret = #()
		attribs = DotNetNode.Attributes
		for i = 0 to (attribs.Count - 1) do(
			t = #()
			item = (attribs.ItemOf i)
			append t item.Name
			append t item.Value
			append ret t
		)
		return ret
	),
	fn HasAttribute attribute =(
		DotNetNode.HasAttribute attribute
	),
	
	fn AppendNode newNode = (
		if classof newNode == XMLNode then(
			DotNetNode.AppendChild newNode.DotNetNode
			return True
		)else(False)
	),
	
	fn GetChild index = (
		dNode = DotNetNode.ChildNodes.ItemOf index
		if dNode.Name != "#text" then(
			newNode = XMLNode()
			newNode.init dnode
			newNode
		)else(return False)
	),
	fn GetChildren = (
		nodes = #()
		for i = 0 to (DotNetNode.ChildNodes.Count - 1) do(
			dNode = DotNetNode.ChildNodes.ItemOf i
			newNode = XMLNode()
			newNode.init dNode
			append nodes newNode
		)
		nodes
	),
	fn DeleteChild childNode = (
		if classof childNode == XMLNode then (
			DotNetNode.RemoveChild childNode.DotNetNode
			return True
		)else(False)
	)
)

struct XMLDocument ( 
	file = undefined,
	loaded = true,
	rootNode = undefined,
	dom = dotNetObject "System.Xml.XmlDocument",
	
	fn LoadXML pathToXml = (
		file = pathToXml
		doc = dom.Load file
		loaded = True
		True
	),
	
	fn SaveXML = (
		if loaded == True then(
			dom.Save file
			return True
		)else(False)
	),
	
	fn GetRootNode = (
		rootNode = XMLNode()
		rootNode.init dom.DocumentElement
		rootNode
	),
	fn getNewNode tag = (
		dNode=dom.CreateElement tag
		newNode=XMLNode()
		newNode.init dNode
		newNode
	),
	fn AppendNode newNode = (
		if classof newNode == XMLNode then(
			dom.AppendChild newNode.DotNetNode
			return True
		)else(False)
	),
	fn PrettySaveXML = (
		if loaded == True then(
			writer = dotNetClass "System.Xml.XmlTextWriter"
			wSettings = dotNetObject "System.Xml.XmlWriterSettings"
			wSettings.indent = True
			w = writer.create file wSettings
			--dom.writeTo w
			dom.writeContentTo w
			w.close()
			True
		)
	)
)
