-- level doors based on actionId
-- to make door for level x create door on map and set its actionid to x+1000

function onUse(cid, item, frompos, item2, topos)

			pos = getPlayerPosition(cid)
	reqlevel = item.actionid - 1000	-- actionids below 100 are reserved

			if pos.x == topos.x and getPlayerLevel(cid) >= reqlevel then
				if pos.y < topos.y then
					doRemoveItem(item.uid,item.type)
					doCreateItem(1248,1,frompos)
					doMovePlayer(cid,2)
				else
					doRemoveItem(item.uid,item.type)
					doCreateItem(1248,1,frompos)
					doMovePlayer(cid,0)
				end
			elseif pos.y == topos.y then
				if pos.x < topos.x then
					doRemoveItem(item.uid,item.type)
					doCreateItem(1246,1,frompos)
					doMovePlayer(cid,1)
				else
					doRemoveItem(item.uid,item.type)
					doCreateItem(1246,1,frompos)
					doMovePlayer(cid,3)
			end
  return 0
 end
 return 1
 end