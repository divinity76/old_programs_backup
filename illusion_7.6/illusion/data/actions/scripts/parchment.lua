--parchment by peonso

function onUse(cid, item, frompos, item2, topos)
playerpos = getPlayerPosition(cid)
demon1pos = {x=229, y=230, z=12}
demon2pos = {x=229, y=237, z=12}
demon3pos = {x=222, y=231, z=12}
demon4pos = {x=222, y=237, z=12}
demon5pos = {x=218, y=233, z=12}

if item.uid == 5555 then
queststatus = getPlayerStorageValue(cid,5555)
if queststatus == -1 or queststatus == 0 then
doPlayerSendTextMessage(cid,22,"You have found a Golden Key.")
player2pos = {x=284, y=115, z=7}
  doTeleportThing(cid,player2pos)
  doSummonCreature("Demon", demon1pos)
  doSummonCreature("Demon", demon2pos)
  doSummonCreature("Demon", demon3pos)
  doSummonCreature("Demon", demon4pos)
  doSummonCreature("Demon", demon5pos)
doTeleportThing(cid,playerpos)
doSendMagicEffect(topos,12)
key_uid = doPlayerAddItem(cid,2091,1)
doSetItemActionId(key_uid,5000)
setPlayerStorageValue(cid,5555,1)

else
doPlayerSendTextMessage(cid,22,"wANT TO GET AGAIN GUY?")
end
return 0
end
return 1
end