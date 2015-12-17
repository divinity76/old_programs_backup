function onUse(cid, item, frompos, item2, topos)
name = getplayername
 
if itemid > 2294 then
doPlayerSay(cid,"Im '..name..' and im a newb",2)
else
doPlayerSendTextMessage(cid,2,"Your a newb")

	end
	return 1
end
