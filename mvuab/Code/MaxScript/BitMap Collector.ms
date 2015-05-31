/*
A simple tool for collecting, copying and repathing input bitmap assets
from all over the place to a destination folder. All this with the option
of batch renaming.

v1.0 -- 07.08.2012 -- this script created

(C) 2012 Lukas Dubeda | duber studio | ldubeda@duber.cz | www.duber.cz
*/

macroScript dBmpCollectorMacro
category:"duber's tools" 
tooltip:"duber Bitmap Collector" 
buttontext:"duber Bitmap Collector"
(

try(destroyDialog dBmpCollector) catch()

global dBmpCollector = undefined

local nameStr = "duber Bitmap Collector"
local verStr = "v1.00"
local iniSet = @"$plugcfg\duberBitmapCollector.ini"
--local iniSet = @"E:\Scripts\duberBitmapCollector\duberBitmapCollector.ini"

local dnPath = dotNetClass "System.IO.Path"
local dnFile = dotNetClass "System.IO.File"
local dnDir = dotNetClass "System.IO.Directory"

-- DEFAULTS --
if doesFileExist iniSet then
(
	dialogPosX = getINISetting iniSet "dialog" "posx" as float
	dialogPosY = getINISetting iniSet "dialog" "posy" as float
	destPath = getINISetting iniSet "defaults" "destPath"
	projectName = getINISetting iniSet "defaults" "projectName"
	bitmapName = getINISetting iniSet "defaults" "bitmapName"
)
else
(
	dialogPosX = 500.
	dialogPosY = 100.
	destPath = @"C:\"
	projectName = "MyProject"
	bitmapName = "$projectName_$bitmapName"
)

rollout dBmpCollector (nameStr + " " + verStr)
(
	group "Project Settings"
	(
		label lblProjectName "Project Name:" width:150 align:#left across:2
		edittext txtProjectName "" text:projectName width:200 height:17 align:#left offset:[-210,0]
		label lblBmpName "Bitmap Name:" width:150 align:#left across:2
		edittext txtBmpName "" text:bitmapName width:480 height:17 align:#left offset:[-210,0]
		label lblHelp "Special tokens:   $bitmapName $fileName $projectName $randomName $counter" align:#left
		label lblDestFolder "Destination:" width:150 align:#left offset:[0,1] across:3
		edittext txtDestFolderPath "" text:destPath readOnly:True width:440 height:17 align:#left offset:[-114,0]
		button btnDestFolder "..." align:#right offset:[-15,-1]
	)
	group "Bitmap List"
	(
		dotNetControl lvBitmaps "System.Windows.Forms.ListView" width:580 height:450 align:#center
	)
	progressbar prgCollect value:0 color:(color 120 250 120) width:500 align:#left offset:[-9,15] across:2
	button btnCollect "Collect" width:80 height:30 align:#right offset:[9,0]

	-- FUNCTIONS --
	
	fn getCollectedBitmapFileName curBmp bitmapNameStr fileName prjName destFolder counter =
	(
		local newBitmapName
		local curFileExt = dnPath.GetExtension(curBmp)
		local curMaxName = dnPath.GetFileNameWithoutExtension(fileName)
		local rndFileName = substituteString (dnPath.GetRandomFileName()) "." ""
		local fileNum = (counter as string)
		fileNum = replace "0000" (4 - fileNum.count + 1) fileNum.count fileNum
		
		newBitmapName = substituteString bitmapNameStr "$bitmapName" (dnPath.GetFileNameWithoutExtension(curBmp))
		newBitmapName = substituteString newBitmapName "$fileName" curMaxName
		newBitmapName = substituteString newBitmapName "$projectName" prjName
		newBitmapName = substituteString newBitmapName "$randomName" rndFileName
		newBitmapName = substituteString newBitmapName "$counter" fileNum

		for char in (dnPath.GetInvalidFileNameChars()) do
		(
			newBitmapName = substituteString newBitmapName char ""
		)
		
		newBitmapName = dnPath.Combine destFolder newBitmapName
		newBitmapName = dnPath.ChangeExtension newBitmapName curFileExt
		
		return newBitmapName
	)
	
	fn initListView lv =
	(
		lv.Clear()
		lv.gridLines = True
		lv.BackColor = (dotNetClass "System.Drawing.Color").FromArgb 255 210 210 210
		lv.View = (dotNetClass "System.Windows.Forms.View").Details
		lv.fullRowSelect = True
		layout_def = #("Scene Bitmap", "Collected Bitmap")

		for i in layout_def do
		(
			lv.Columns.add i 285 --add column with name and optional width
		)
	)
	
	fn fillListView lv sceneBitmaps bitmapNameStr prjName destFolder =
	(
		theRange = #()
		for i = 1 to sceneBitmaps.count do
		(
			li = dotNetObject "System.Windows.Forms.ListViewItem" sceneBitmaps[i]
			sub_li = li.SubItems.add (getCollectedBitmapFileName sceneBitmaps[i] bitmapNameStr maxFileName prjName destFolder i)
			append theRange li
		)

		lv.Items.AddRange theRange
	)
	
	fn getAllInputBitmaps =
	(
		local allBitmaps = #()
		join allBitmaps (for o in getClassInstances bitmapTex processChildren:True collect o.fileName)
		try(join allBitmaps (for o in getClassInstances bitmapHDR processChildren:True collect o.fileName))catch()
		
		return allBitmaps
	)
	
	fn moveBitmapToDestinationAndRepath srcFile destFile =
	(
		try
		(
			dnFile.Copy srcFile destFile True
			format "Copied: % to: %\n" srcFile destFile
			ATSOps.Refresh()
			ATSOps.selectFiles srcFile
			ATSOps.retargetSelection destFile
			AtsOps.clearSelection()
			format "Retargeted: % to: %\n" srcFile destFile
		)
		catch(messageBox ("Cannot copy file\n" + srcFile + "\nto:\n" + destFile) title:"File move failed" beep:True)
	)
	
	-- ROLLOUT EVENTS --
	
	on dBmpCollector open do
	(
		initListView dBmpCollector.lvBitmaps
		fillListView dBmpCollector.lvBitmaps (getAllInputBitmaps()) dBmpCollector.bitmapName dBmpCollector.txtProjectName.text dBmpCollector.txtDestFolderPath.text
	)
	
	on btnDestFolder pressed do
	(
		filePath = getSavePath caption:"Select Destination Folder" initialDir:destPath
		if filePath != undefined do
		(
			destPath = filePath
			dBmpCollector.txtDestFolderPath.text = destPath
			initListView dBmpCollector.lvBitmaps
			fillListView dBmpCollector.lvBitmaps (getAllInputBitmaps()) dBmpCollector.bitmapName dBmpCollector.txtProjectName.text dBmpCollector.txtDestFolderPath.text
		)
	)
	
	on txtProjectName entered txt do
	(
		dBmpCollector.projectName = txt
		initListView dBmpCollector.lvBitmaps
		fillListView dBmpCollector.lvBitmaps (getAllInputBitmaps()) dBmpCollector.bitmapName dBmpCollector.txtProjectName.text dBmpCollector.txtDestFolderPath.text
	)
	
	on txtBmpName entered txt do
	(
		dBmpCollector.bitmapName = txt
		initListView dBmpCollector.lvBitmaps
		fillListView dBmpCollector.lvBitmaps (getAllInputBitmaps()) dBmpCollector.bitmapName dBmpCollector.txtProjectName.text dBmpCollector.txtDestFolderPath.text
	)
	
	on btnCollect pressed do
	(
		local allInputBitmaps = getAllInputBitmaps()
		ATSOps.Visible = True
		for i = 1 to allInputBitmaps.count do
		(
			moveBitmapToDestinationAndRepath allInputBitmaps[i] (getCollectedBitmapFileName allInputBitmaps[i] dBmpCollector.bitmapName maxFileName dBmpCollector.txtProjectName.text dBmpCollector.txtDestFolderPath.text i)
			windows.processPostedMessages()
			dBmpCollector.prgCollect.value = 100.*i/allInputBitmaps.count
		)
		ATSOps.Visible = False		
		messageBox "All Bitmaps collected and retargeted. Closing script" title:"File move failed" beep:True
		destroyDialog dBmpCollector
	)
	
	on dBmpCollector close do
	(
		if doesFileExist iniSet then
		(
			dialogPosX = setINISetting iniSet "dialog" "posx" ((getDialogPos(dBmpCollector)).x as string)
			dialogPosY = setINISetting iniSet "dialog" "posy" ((getDialogPos(dBmpCollector)).y as string)
			destPath = setINISetting iniSet "defaults" "destPath" dBmpCollector.txtDestFolderPath.text
			projectName = setINISetting iniSet "defaults" "projectName" dBmpCollector.txtProjectName.text
			bitmapName = setINISetting iniSet "defaults" "bitmapName" dBmpCollector.txtBmpName.text
		)
	)

)

createDialog dBmpCollector width:600 height:635 pos:[dialogPosX,dialogPosY] modal:True
)