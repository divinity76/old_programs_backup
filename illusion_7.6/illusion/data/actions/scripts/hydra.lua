-- Create Hydra by Czepek --
  function onUse(cid, item, frompos, item2, topos)

pos = getPlayerPosition(cid) 

if getTilePzInfo(topos) == 0 then
  if getTilePzInfo(pos) == 0 then

 if item.itemid == 4850 then
 doSummonCreature("Hydra", pos)
doSendMagicEffect(topos,12)
doPlayerSay(cid,"Amgad",1)
	doRemoveItem(item.uid,item.type)
   
else

     doPlayerSendCancel(cid,"You cannot use that.")
    end  
  else
    doPlayerSendCancel(cid,"You Cannot use this Egg while you are in a Protect Zone.")
  end
else
  doPlayerSendCancel(cid,"You cannot use this Egg on a protect zone.")
end

return 1

end