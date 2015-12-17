-- level doors based on actionId
-- to make door for level x create door on map and set its actionid to x+1000

function onUse(cid, item, frompos, item2, topos)

			pos = getPlayerPosition(cid)

			if pos.x == topos.x then
				if pos.y < topos.y then
					doMovePlayer(cid,2)
					doTransformItem(item.uid,item.itemid+1)
			else
					doMovePlayer(cid,0)
					doTransformItem(item.uid,item.itemid+1)
			end
  return 0
 end
 return 1
 end