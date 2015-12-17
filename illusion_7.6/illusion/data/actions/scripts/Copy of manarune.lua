function onUse(cid, item, frompos, item2, topos)
    
mag = getPlayerMagLevel(cid)
if mag >= 3 then
doSendMagicEffect(topos,1)
doPlayerAddMana(cid, 1000)
if item.type > 1 then
doChangeTypeItem(item.uid,item.type-1)
else
doRemoveItem(item.uid,1)
end
else
            doPlayerSendCancel(cid, "You are exhausted.")
end
return 1
end