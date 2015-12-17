-- Construction kits, originally by JTE
 --
 -- Bugfix by Alreth: containers can now be opened after being constructed
 --
 -- Second update by Alreth: construction kits opened in inventory are
 -- now constructed on ground under character instead of error message
 
 function onUse(cid, item, frompos, item2, topos)
     if item.itemid == 3901 then
     -- Wooden chair
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(1652, 1, newpos)
         return 1
 
     elseif item.itemid == 3902 then
     -- Sofa chair
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(1658, 1, newpos)
         return 1
         
     elseif item.itemid == 3903 then
     -- Red cushioned chair
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(1666, 1, newpos)
         return 1
         
     elseif item.itemid == 3904 then
     -- Green cushioned chair
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(1670, 1, newpos)
         return 1
         
     elseif item.itemid == 3905 then
     -- Tusk chair
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(3813, 1, newpos)
         return 1
         
     elseif item.itemid == 3906 then
     -- Ivory chair
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(3817, 1, newpos)
         return 1
         
     elseif item.itemid == 3907 then
     -- Small trunk
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(3821, 1, newpos)
         return 1
         
     elseif item.itemid == 3908 then
     -- Coal basin
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(2602, 1, newpos)
         return 1
         
     elseif item.itemid == 3909 then
     -- Big table
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(1614, 1, newpos)
         return 1
         
     elseif item.itemid == 3910 then
     -- Square table
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(1615, 1, newpos)
         return 1
         
     elseif item.itemid == 3911 then
     -- Round table
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(1616, 1, newpos)
         return 1
         
     elseif item.itemid == 3912 then
     -- Small table
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(1619, 1, newpos)
         return 1
         
     elseif item.itemid == 3913 then
     -- Stone table
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(3805, 1, newpos)
         return 1
         
     elseif item.itemid == 3914 then
     -- Tusk table
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(3807, 1, newpos)
         return 1
         
     elseif item.itemid == 3915 then
     -- Box
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(1738, 1, newpos)
         return 1
         
     elseif item.itemid == 3917 then
     -- Harp
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(2084, 1, newpos)
         return 1
         
     elseif item.itemid == 3918 then
     -- Birdcage
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(2095, 1, newpos)
         return 1
         
     elseif item.itemid == 3919 then
     -- Bamboo table
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(3809, 1, newpos)
         return 1
         
     elseif item.itemid == 3920 then
     -- Thick trunk
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(3811, 1, newpos)
         return 1
         
     elseif item.itemid == 3921 then
     -- Drawer
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(1716, 1, newpos)
         return 1
         
     elseif item.itemid == 3923 then
     -- Barrel
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(1774, 1, newpos)
         return 1
         
     elseif item.itemid == 3926 then
     -- Piano
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(2080, 1, newpos)
         return 1
         
     elseif item.itemid == 3927 then
     -- Globe
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(2098, 1, newpos)
         return 1
         
     elseif item.itemid == 3928 then
     -- Potted flower (pink one)
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(2104, 1, newpos)
         return 1
         
     elseif item.itemid == 3929 then
     -- Potted flower (the boring green one)
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(2101, 1, newpos)
         return 1
         
     elseif item.itemid == 3931 then
     -- Christmas tree
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(2105, 1, newpos)
         return 1
         
     elseif item.itemid == 3932 then
     -- Dresser
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(1724, 1, newpos)
         return 1
         
     elseif item.itemid == 3933 then
     -- Pendelum clock
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(1728, 1, newpos)
         return 1
         
     elseif item.itemid == 3934 then
     -- Locker
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(1732, 1, newpos)
         return 1
         
     elseif item.itemid == 3935 then
     -- Trough
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(1775, 1, newpos)
         return 1
         
     elseif item.itemid == 3936 then
     -- Bamboo dresser
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(3832, 1, newpos)
         return 1
         
     elseif item.itemid == 3937 then
     -- Table lamp
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(2064, 1, newpos)
         return 1
         
     elseif item.itemid == 3938 then
     -- Large trunk
         if (frompos.x == 65535) then
             newpos = getPlayerPosition(cid)
         else
             newpos = {x = frompos.x, y = frompos.y, z = frompos.z}
         end
         doRemoveItem(item.uid, item.type)
         doSendMagicEffect(newpos, 2)
         doCreateItem(1750, 1, newpos)
         return 1
         
     else
         return 0
         
     end
 end