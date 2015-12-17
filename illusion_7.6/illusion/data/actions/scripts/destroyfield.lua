--<¤ ACTION SCRIPT BY CAYAN  ¤>--
 --<¤    For ThotthOTServ 1.1 ¤>--
 
 function onUse(cid, item, frompos, item2, topos)
 fieldpos = {x=topos.x, y=topos.y, z=topos.z, stackpos=254}
 fielditem = getThingfromPos(fieldpos)
 
 itempos = {x=topos.x, y=topos.y, z=topos.z, stackpos=1}
 fieldcamp = getThingfromPos(itempos)
 
 if getPlayerMagLevel(cid) >= 3 then
 if ((fielditem.itemid > 0) and (fielditem.itemid ~= 1497) and (fielditem.itemid ~= 1498)) then
 doRemoveItem(fielditem.uid,1)
 doSendMagicEffect(topos,2)
 if item.type > 1 then
 doChangeTypeItem(item.uid,item.type-1)
 else
 doRemoveItem(item.uid,1)
 end
 end
 if ((fieldcamp.itemid >= 1487 and fieldcamp.itemid <= 1496) or (fieldcamp.itemid >= 1500 and fieldcamp.itemid <= 1507)) then
 doRemoveItem(fieldcamp.uid,1)
 doSendMagicEffect(topos,2)
 if item.type > 1 then
 doChangeTypeItem(item.uid,item.type-1)
 else
 doRemoveItem(item.uid,1)
 end
 end 
 else
 doSendMagicEffect(frompos,2)
 doPlayerSendCancel(cid,"Your magic level is too low.")
 end
 return 1
 end