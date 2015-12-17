function onUse(cid, item, frompos, item2, topos)
doorpos = {x=frompos.x, y=frompos.y, z=frompos.z}
playerpos = getPlayerPosition(cid)
playerlevel = getPlayerLevel(cid)
if item.itemid == 1229 and doorpos.x == 216 and doorpos.y == 94 and doorpos.z == 7 then
if playerlevel > 19 then
doTransformItem(item.uid,item.itemid+1)
elseif playerlevel < 20 then
doPlayerSendTextMessage(cid,22,"You need to be lvl 20 or higher.")
end
end
return 1
end
