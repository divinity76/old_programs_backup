local config = {
	reportReadsToExpire = 1
}

function onSay(cid, words, param)
	if(isNumber(param) == TRUE) then
		local reportId = tonumber(param)
		local report = db.getResult("SELECT * FROM `server_reports` WHERE `id` = " .. reportId)
		if(report:getID() ~= -1) then
			db.executeQuery("UPDATE `server_reports` SET `reads` = `reads` + 1 WHERE `id` = " .. reportId)
			doPlayerPopupFYI(cid, "Report # " .. reportId .. "\n\nName: " .. getPlayerNameByGUID(report:getDataInt("player_id")) .. "\nPosition: [X: " .. report:getDataInt("posx") .. " | Y: " .. report:getDataInt("posy") .. " | Z: " .. report:getDataInt("posz") .. "]\nDate: " .. os.date("%c", report:getDataInt("timestamp")) .. "\nReport:\n" .. report:getDataString("report"))
			report:free()
		else
			doPlayerSendTextMessage(cid, MESSAGE_STATUS_CONSOLE_BLUE, "Report with ID: " .. reportId .. " does not exists.")
		end
	else
		local list = db.getResult("SELECT `id`, `player_id` FROM `server_reports` WHERE `reads` < " .. config.reportReadsToExpire)
		if(list:getID() ~= -1) then
			doPlayerSendTextMessage(cid, MESSAGE_STATUS_CONSOLE_BLUE, "New reports:")
			while(true) do
				doPlayerSendTextMessage(cid, MESSAGE_STATUS_CONSOLE_BLUE, "# " .. list:getDataInt("id") .. ", by " .. getPlayerNameByGUID(list:getDataInt("player_id")) .. ".")
				if not(list:next()) then
					break
				end
			end
			list:free()
		else
			doPlayerSendTextMessage(cid, MESSAGE_STATUS_CONSOLE_BLUE, "There are no active reports.")
		end
	end
	return FALSE
end