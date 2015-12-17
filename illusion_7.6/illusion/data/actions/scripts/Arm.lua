-- annihilator chests

function onUse(cid, item, frompos, item2, topos)

   	if item.uid == 1001 then
   		queststatus = getPlayerStorageValue(cid,1001)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found Crown legs.")
   			doPlayerAddItem(cid,2488,1)
   			setPlayerStorageValue(cid,1001,1)
   		else
   			doPlayerSendTextMessage(cid,22,"You have already picked your Armor.")
   		end
   	elseif item.uid == 1002 then
   		queststatus = getPlayerStorageValue(cid,1002)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Golden Armor.")
   			doPlayerAddItem(cid,2466,1)
   			setPlayerStorageValue(cid,1002,1)
   		else
   			doPlayerSendTextMessage(cid,22,"You have already picked your Armor.")
   		end

	else
		return 0
   	end

   	return 1
end