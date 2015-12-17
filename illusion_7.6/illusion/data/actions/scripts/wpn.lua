-- annihilator chests

function onUse(cid, item, frompos, item2, topos)

   	if item.uid == 1003 then
   		queststatus = getPlayerStorageValue(cid,1004)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Bright Sword.")
   			doPlayerAddItem(cid,2407,1)
   			setPlayerStorageValue(cid,1004,1)
   		else
   			doPlayerSendTextMessage(cid,22,"You have already picked your weapon.")
   		end
   	elseif item.uid == 1004 then
   		queststatus = getPlayerStorageValue(cid,1004)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Fire Axe.")
   			doPlayerAddItem(cid,2432,1)
   			setPlayerStorageValue(cid,1004,1)
   		else
   			doPlayerSendTextMessage(cid,22,"You have already picked your Weapon.")
   		end

	else
		return 0
   	end

   	return 1
end