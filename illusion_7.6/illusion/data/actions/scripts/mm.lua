function onUse(cid, item, frompos, item2, topos)
 if getSlotItem(cid,8) == 2640 then
 pos = getPlayerPosition(cid)
 doTeleportThing(cid,frompos)
 doSendMagicEffect(pos,1)
 else
 return 0
 end
 return 1
 end