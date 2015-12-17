-- Destroy item-rune - by Alreth (v1.2)
 
 function onUse(cid, item, frompos, item2, topos)
     allowall = false -- Set to false to only allow gamemasters and true to allow any player to use this rune.
     
     playeraccess = getPlayerAccess(cid)
     if (allowall == false and playeraccess < 1) then
         doPlayerSendCancel(cid, "Only available to gamemasters.")
     else
         while (item2.itemid == 1) do
             topos.stackpos = topos.stackpos+1
             item2 = getThingfromPos(topos)
         end
         if (((topos.stackpos > 0 and topos.x ~= 65535) or topos.x == 65535) and item2.itemid > 0) then
             if (doRemoveItem(item2.uid, item2.type) ~= -1) then
                 doSendMagicEffect(topos, 3)
             else
                 doPlayerSendCancel(cid, "There was an error when removing the item (itemid: "..item2.itemid.." stackpos: "..topos.stackpos..").")
             end
         else
             doPlayerSendCancel(cid, "You can not remove ground.")
         end
     end
     return 1
 end