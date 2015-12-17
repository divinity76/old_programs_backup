-- Rotatecode by Rizzo --
 
 function onUse(cid, item, frompos, item2, topos)
 	if item.itemid == 1667 then
 		-- Red Cush North to East
 		doTransformItem(item.uid,1668)
 		doSendMagicEffect(topos,14)
 		return 1
 	elseif item.itemid == 1668 then
 		-- Red Cush East to South
 		doTransformItem(item.uid,1666)
 		doSendMagicEffect(topos,14)
 		return 1
 	elseif item.itemid == 1666 then
 		-- Red Cush South to West
 		doTransformItem(item.uid,1669)
 		doSendMagicEffect(topos,14)
 		return 1
 	elseif item.itemid == 1669 then
 		-- Red Cush West to North
 		doTransformItem(item.uid,1667)
 		doSendMagicEffect(topos,14)	
 		return 1
 	elseif item.itemid == 1659 then
 		-- Green Sofa North to East
 		doTransformItem(item.uid,1660)
 		doSendMagicEffect(topos,14)
 		return 1
 	elseif item.itemid == 1660 then
 		-- Green Sofa East to South
 		doTransformItem(item.uid,1658)
 		doSendMagicEffect(topos,14)
 		return 1
 	elseif item.itemid == 1658 then
 		-- Green Sofa South to West
 		doTransformItem(item.uid,1661)
 		doSendMagicEffect(topos,14)
 		return 1
 	elseif item.itemid == 1661 then
 		-- Green Sofa West to North
 		doTransformItem(item.uid,1659)
 		doSendMagicEffect(topos,14)	
 		return 1
 	elseif item.itemid == 1671 then
 		-- Green Cush North to East
 		doTransformItem(item.uid,1672)
 		doSendMagicEffect(topos,14)
 		return 1
 	elseif item.itemid == 1672 then
 		-- Green Cush East to South
 		doTransformItem(item.uid,1670)
 		doSendMagicEffect(topos,14)
 		return 1
 	elseif item.itemid == 1670 then
 		-- Green Cush South to West
 		doTransformItem(item.uid,1673)
 		doSendMagicEffect(topos,14)
 		return 1
 	elseif item.itemid == 1673 then
 		-- Green Cush West to North
 		doTransformItem(item.uid,1671)
 		doSendMagicEffect(topos,14)	
 		return 1
 	else
 		return 0
 	end
 end