-- by GriZzm0

function onUse(cid, item, frompos, item2, topos)
gatepos1 = {x=434, y=272, z=15, stackpos=1}
gatepos2 = {x=435, y=272, z=15, stackpos=1}
gatepos3 = {x=436, y=272, z=15, stackpos=1}
gatepos4 = {x=437, y=272, z=15, stackpos=1}
gatepos5 = {x=438, y=272, z=15, stackpos=1}
getgate1 = getThingfromPos(gatepos1)
getgate2 = getThingfromPos(gatepos2)
getgate3 = getThingfromPos(gatepos3)
getgate4 = getThingfromPos(gatepos4)
getgate5 = getThingfromPos(gatepos5)
if item.uid == 4444 and 
item.itemid == 1945 and 
getgate1.itemid == 1304 and 
getgate2.itemid == 1304 and 
getgate3.itemid == 1304 and
getgate4.itemid == 1304 and
getgate5.itemid == 1304 then
 doRemoveItem(getgate1.uid,1)
 doRemoveItem(getgate2.uid,1)
 doRemoveItem(getgate3.uid,1)
 doRemoveItem(getgate4.uid,1)
 doRemoveItem(getgate5.uid,1)
 doTransformItem(item.uid,item.itemid+1)
elseif item.uid == 4444 and 
item.itemid == 1946 and 
getgate1.itemid == 0 and 
getgate2.itemid == 0 and 
getgate3.itemid == 0 and
getgate4.itemid == 0 and
getgate5.itemid == 0 then
 doCreateItem(1304,1,gatepos1)
 doCreateItem(1304,1,gatepos2)
 doCreateItem(1304,1,gatepos3)
 doCreateItem(1304,1,gatepos4)
 doCreateItem(1304,1,gatepos5)
 doTransformItem(item.uid,item.itemid-1)
else
 doPlayerSendCancel(cid,"N�o � Possivel")
end
return 1
end