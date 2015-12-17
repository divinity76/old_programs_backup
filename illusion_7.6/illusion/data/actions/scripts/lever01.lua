function onUse(cid, item, frompos, item2, topos)

   	-- from main to island
   	if item.uid == 1000 then
			gopos = {x=198, y=233, z=7}
   			doTeleportThing(cid, gopos)	
				doSendMagicEffect(gopos,10)
	   	-- from island to main
   	elseif item.uid == 1003 then
   		backpos = {x=205, y=194, z=7}
   		doTeleportThing(cid, backpos)
				doSendMagicEffect(backpos,10)

	else
		return 0
   	end

   	return 1
end
