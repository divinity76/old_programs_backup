function onUse(cid, item, frompos, item2, topos)
if item.uid == 1000 then
queststatus = getPlayerStorageValue(cid,1000)
if queststatus == -1 then
 doPlayerSendTextMessage(cid,22,"You have found Steel boots.") 
doPlayerAddItem(cid,2645,1)
setPlayerStorageValue(cid,1000,1) 
else 
doPlayerSendTextMessage(cid,22,"It is empty.")
end 
end 
return 1 
end