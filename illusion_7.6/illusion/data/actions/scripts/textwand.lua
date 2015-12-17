-- Text Wand by Shyym 
function onUse(cid, item, frompos, item2, topos) 
creature1pos = {x=topos.x, y=topos.y, z=topos.z, stackpos=253} 
creature1 = getThingfromPos(creature1pos) 
if creature1.itemid > 0 then 
rand = math.random(4) 
if rand == 1 then 
doCreatureSay(creature1.uid,"Text1",1) 
elseif rand == 2 then 
doCreatureSay(creature1.uid,"Text2",1) 
elseif rand == 3 then 
doCreatureSay(creature1.uid,"Text3",1) 
elseif rand == 4 then 
doCreatureSay(creature1.uid,"Text4",1) 
end 
end 
end 
