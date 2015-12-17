function onWalk(cid, item, itemuid, itemaid, posx, posy, posz)

	playerlevel = getPlayerLevel(cid)

if item.itemid==446 and posy == 812 and posz == 7 and playerlevel < 4 then
	newpos = {x=posx, y=posy, z=posz}
	doSendMagicEffect(newpos,2)
	newpos2 = {x=788, y=812, z=7}
	doTeleportThing(cid,newpos2)
	doSendMagicEffect(newpos2,10)

elseif item.itemid==446 and posy == 813 and posz == 7 and playerlevel < 4 then
	newpos = {x=posx, y=posy, z=posz}
	doSendMagicEffect(newpos,2)
	newpos2 = {x=788, y=813, z=7}
	doTeleportThing(cid,newpos2)
	doSendMagicEffect(newpos2,10)
	return 1

	end
end