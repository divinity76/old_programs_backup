function onUse(cid, item, frompos, item2, topos)
   
   
   	if item.itemid == 1209 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1211)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1211 then
   --
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x+1, y=frompos.y, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
                   doTransformItem(item.uid,1209)
   --
   else
   		doTransformItem(item.uid,1209)
   end
   
 
   	elseif item.itemid == 1210 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1211)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
 
 
 elseif item.itemid == 1227 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1228)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
 
   	elseif item.itemid == 1228 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x+1, y=frompos.y, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1227)
   else
   		doTransformItem(item.uid,1227)
   end
 
  	elseif item.itemid == 3547 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,3548)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
 
   	elseif item.itemid == 1212 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1214)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1214 then
   
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x, y=frompos.y+1, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1212)
   else
   		doTransformItem(item.uid,1212)
   end
   
   --
   	elseif item.itemid == 1219 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1220)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   
   	elseif item.itemid == 1220 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x+1, y=frompos.y, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1219)
   else
   		doTransformItem(item.uid,1219)
   end
   --
   	elseif item.itemid == 1221 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1222)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
 
 	elseif item.itemid == 1239 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1240)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
 
 	elseif item.itemid == 1235 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1236)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
 
 	elseif item.itemid == 3538 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,3539)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
 
   	elseif item.itemid == 3539 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x, y=frompos.y+1, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,3538)
   else
   		doTransformItem(item.uid,3538)
   end

   	elseif item.itemid == 1222 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x, y=frompos.y+1, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1221)
   else
   		doTransformItem(item.uid,1221)
   end
   ---
   	elseif item.itemid == 1223 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1224)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1224 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x+1, y=frompos.y, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1223)
   else
   		doTransformItem(item.uid,1223)
   end
   --
   	elseif item.itemid == 1225 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1226)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   
   	elseif item.itemid == 1226 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x, y=frompos.y+1, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1225)
   else
   		doTransformItem(item.uid,1225)
   end
   --
   	 	
   --
   	elseif item.itemid == 1229 then
 		if item.actionid == 0 then
 			doTransformItem(item.uid,1229)
 		else
 		        if item.actionid > 9000 and item.actionid < 9500 then 
 		             if getPlayerLevel(cid) < (item.actionid-9000) then
 			       doPlayerSendTextMessage(cid,22,"Only Lvl "..(item.actionid-9000).." and up can open this door.")
 			     else
 			       doTeleportThing(cid,frompos)  -- Moves character into doorway
 			       doTransformItem(item.uid,1229)
 			     end 
 		       else
 		             doPlayerSendTextMessage(cid,22,"It is locked.")
 		       end
 		end
   	elseif item.itemid == 1230 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x, y=frompos.y+1, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1003)
   else
   		doTransformItem(item.uid,1003)
   end
   --
   	elseif item.itemid == 1231 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1233)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1233 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x+1, y=frompos.y, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1231)
   else
   		doTransformItem(item.uid,1231)
   end
   --
   	elseif item.itemid == 1234 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1236)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1236 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x, y=frompos.y+1, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1234)
   else
   		doTransformItem(item.uid,1234)
   end
   --
   	elseif item.itemid == 1237 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1238)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1238 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x+1, y=frompos.y, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1237)
   else
   		doTransformItem(item.uid,1237)
   end
   --
   	elseif item.itemid == 1239 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1240)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1240 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x, y=frompos.y+1, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1239)
   else
   		doTransformItem(item.uid,1239)
   end
   --
   	elseif item.itemid == 1241 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1242)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1242 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x+1, y=frompos.y, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1241)
   else
   		doTransformItem(item.uid,1241)
   end
   --
   
   	elseif item.itemid == 1243 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1244)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1244 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x, y=frompos.y+1, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1243)
   else
   		doTransformItem(item.uid,1243)
   end
   --
   
   	elseif item.itemid == 1245 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1246)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1246 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x+1, y=frompos.y, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1245)
   else
   		doTransformItem(item.uid,1245)
   end
   --
   	elseif item.itemid == 1247 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1248)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1248 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x, y=frompos.y+1, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1247)
   else
   		doTransformItem(item.uid,1247)
   end
   --
   
   	elseif item.itemid == 1249 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1251)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1251 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x+1, y=frompos.y, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1249)
   else
   		doTransformItem(item.uid,1249)
   end
   --
   
   	elseif item.itemid == 1252 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1254)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
elseif item.itemid == 1253 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1254)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1254 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x, y=frompos.y+1, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1252)
   else
   		doTransformItem(item.uid,1252)
   end
   --
   	elseif item.itemid == 1255 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1256)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1256 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x+1, y=frompos.y, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1255)
   else
   		doTransformItem(item.uid,1255)
   end
   --
   	elseif item.itemid == 1257 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1258)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1258 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x, y=frompos.y+1, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1257)
   else
   		doTransformItem(item.uid,1257)
   end
   --
   	elseif item.itemid == 1259 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1260)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1260 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x+1, y=frompos.y, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1259)
   else
   		doTransformItem(item.uid,1259)
   end
   --
   
   	elseif item.itemid == 1261 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1262)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1262 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x, y=frompos.y+1, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1261)
   else
   		doTransformItem(item.uid,1261)
   end
   
   --another simple door--
   	elseif item.itemid == 1539 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,3470)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1540 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x+1, y=frompos.y, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,3469)
   else
   		doTransformItem(item.uid,3469)
   end
   --
   	elseif item.itemid == 1541 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1542)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1542 then
   doorpos = {x=frompos.x, y=frompos.y, z=frompos.z, stackpos=253}
                   getplayer = getThingfromPos(doorpos)
                   if getplayer.itemid > 0 then
                   npos = {x=frompos.x, y=frompos.y+1, z=frompos.z}
                   doTeleportThing(getplayer.uid,npos)
   		doTransformItem(item.uid,1541)
   else
   		doTransformItem(item.uid,1541)
   end
   
   --another simple door--
   
   	elseif item.itemid == 1640 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1241)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1641 then
   		doTransformItem(item.uid,1240)
   
   
   	elseif item.itemid == 1636 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1637)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1637 then
   		doTransformItem(item.uid,1636)
   
   	elseif item.itemid == 1634 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1635)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1635 then
   		doTransformItem(item.uid,1634)
   
   	elseif item.itemid == 1638 then
   		if item.actionid == 0 then
   			doTransformItem(item.uid,1639)
   		else
   			doPlayerSendTextMessage(cid,22,"It is locked.")
   		end
   	elseif item.itemid == 1639 then
   		doTransformItem(item.uid,1638)
   
   	else
   		return 0
   	end
   	return 1
   end