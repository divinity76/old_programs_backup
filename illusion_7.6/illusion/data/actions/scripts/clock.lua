----- date + hours viewr by colex-------
function onUse(cid, item, frompos, item2, topos)


if item.itemid == 2036 then
data = os.date('%c')

doPlayerSendTextMessage(cid, 22, ""..data.."")
end

return 1
end