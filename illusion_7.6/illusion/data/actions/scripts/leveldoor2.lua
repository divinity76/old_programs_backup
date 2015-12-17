-- level doors based on actionId
-- to make door for level x create door on map and set its actionid to x+1000

function onUse(cid, item, frompos, item2, topos)
			
			pos = getPlayerPosition(cid)
	reqlevel = item.actionid - 1000	-- actionids below 100 are reserved

		if getPlayerLevel(cid) >= reqlevel then
			if pos.x == topos.x and getPlayerLevel(cid) >= reqlevel then
				if pos.y < topos.y then
					doTransformItem(item.uid, 1248)
					doMovePlayer(cid,2)
				else
					doTransformItem(item.uid, 1248)
					doMovePlayer(cid,0)
				end
			elseif pos.y == topos.y then
				if pos.x < topos.x then
					doTransformItem(item.uid, 1246)
					doMovePlayer(cid,1)
				else
					doTransformItem(item.uid, 1246)
					doMovePlayer(cid,3)
			end
else
return 0
end
else
doPlayerSendTextMessage(cid,22,'You need level ' .. reqlevel .. ' to pass this door.')
end

return 1

end 