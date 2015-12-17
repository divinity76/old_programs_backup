function onUse(cid, item, frompos, item2, topos)
    if (exhaust(cid, 1543, 2) > 0) then
doSendMagicEffect(topos,1)
doPlayerAddMana(cid, 3000)
if item.type > 1 then
doChangeTypeItem(item.uid,item.type-1)
else
doRemoveItem(item.uid,1)
end
    else
     doPlayerSendCancel(cid,"No Macro Plix?.")
    end
    return 1
end