(
rollout fuse "Fuser" width:212 height:51
(
fn fuser lim:1=
(
--local lim = float 1
local sel = #()
local bits = #{}
local col = #()
local avg = float 0
local vals,dist,knot,ov,iv

for s in 1 to numSplines $ where (getKnotSelection $ s).count > 0 do
for k in getKnotSelection $ s do append sel #(s,k)
vals = for k in sel collect getKnotPoint $ k[1] k[2]
bits = #{1..sel.count}
for n in 1 to sel.count where bits[n] do
(
bits[n] = off
col=for i in 1 to sel.count collect
(
if	sel[i]==sel[n] or distance vals[n] vals[i] < lim then
(
bits[i] = off
sel[i]
)
else dontCollect
)
if col.count > 1 then
(
for o in col do avg+=getKnotPoint $ o[1] o[2]
avg/=col.count
--avg=[aprox avg.x,aprox avg.y,aprox avg.z]
for o in col do
(
knot = getKnotPoint $ o[1] o[2]
ov = getOutVec $ o[1] o[2] - knot
iv = getInVec $ o[1] o[2] - knot
setKnotPoint $ o[1] o[2] avg
setOutVec $ o[1] o[2] (ov+avg)
setInVec $ o[1] o[2] (iv+avg)
--format "processed: spline % vertex %\n" o[1] o[2]
)
avg=0
)
)
updateShape $
ok
)
	button run "Fuse" pos:[3,7] width:70 height:40 across:2
	spinner rad "Thereshold" pos:[108,20] width:89 height:16 range:[0,100,1]
	on run pressed do
		fuser lim:rad.value
)
createDialog Fuse
)