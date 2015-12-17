function onUse(cid, item, frompos, item2, topos)
--by CVS - Modified by Heliton--

if item.uid == 9015 then
queststatus = getPlayerStorageValue(cid,1001)
if queststatus == -1 or queststatus == 0 then
doPlayerSendTextMessage(cid,22,"You have found a 100 platinum coins.")
doPlayerAddItem(cid,2152,100)
doSendMagicEffect(topos,12)
setPlayerStorageValue(cid,1001,1)
else
doPlayerSendTextMessage(cid,22,"It is empty.")
end

elseif item.uid == 9016 then
queststatus = getPlayerStorageValue(cid,1002)
if queststatus == -1 or queststatus == 0 then
doPlayerSendTextMessage(cid,22,"You have found a ring of healing.")
doPlayerAddItem(cid,2214,1)
doSendMagicEffect(topos,12)
setPlayerStorageValue(cid,1002,1)
else
doPlayerSendTextMessage(cid,22,"It is empty.")
end
else
return 0
end

return 1

end 