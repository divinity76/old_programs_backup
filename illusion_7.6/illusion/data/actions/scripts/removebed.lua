---created by Dzojo---
function onUse(cid, item, frompos, item2, topos)
luzkoit = {x=topos.x+1, y=topos.y, z=topos.z, stackpos=1}
luzkoit2 = {x=topos.x, y=topos.y+1, z=topos.z, stackpos=1}
getbed1 = getThingfromPos(luzkoit)
getbed2 = getThingfromPos(luzkoit2)

if item.itemid == 1768 then
doRemoveItem(getbed1.uid,1)
doCreateItem(1757,1,luzkoit)
doTransformItem(item.uid, 1756)
doSendMagicEffect(topos,2)

elseif item.itemid == 1764 then
doRemoveItem(getbed1.uid,1)
doCreateItem(1761,1,luzkoit)
doTransformItem(item.uid, 1760)
doSendMagicEffect(topos,2)

elseif item.itemid == 1766 then
doRemoveItem(getbed2.uid,1)
doCreateItem(1759,1,luzkoit2)
doTransformItem(item.uid, 1758)
doSendMagicEffect(topos,2)

elseif item.itemid == 1762 then
doRemoveItem(getbed2.uid,1)
doCreateItem(1755,1,luzkoit2)
doTransformItem(item.uid, 1754)
doSendMagicEffect(topos,2)

return 0
end
return 1
end