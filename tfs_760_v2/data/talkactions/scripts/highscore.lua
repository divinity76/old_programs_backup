function onSay(cid, words, param)

     -- max players on the highscore
     local players = 20 -- The amount of players that is shown in the list

     -- don't touch --
     local str = ""
     local value = ""
     local name = ""
     local player_id = 0
     local result = nil
	if param == "level" or param == "exp" or param == "lvl" then
		str = "# [Level] - Name\n"
		value = "level"
		name = "name"
		result = db.getResult("SELECT `level`, `name` FROM `players` ORDER BY `level` DESC, `name` ASC;")
	elseif param == "magiclevel" or param == "magic" or param == "mlevel" or param == "ml" then
		str = "# [Magic Level] - Name\n"
		value = "maglevel"
		name = "name"
		result = db.getResult("SELECT `name`, `maglevel` FROM `players` ORDER BY `maglevel` DESC, `name` ASC;")
	elseif param == "fist" then
		str = "# [Fist] - Name\n"
		value = "value"
		player_id = "player_id"
		result = db.getResult("SELECT `player_id`, `value` FROM `player_skills`  WHERE `skillid` = 0 ORDER BY `value` DESC, `player_id` ASC;")
	elseif param == "club" then
		str = "# [Club] - Name\n"
		value = "value"
		player_id = "player_id"
		result = db.getResult("SELECT `player_id`, `value` FROM `player_skills`  WHERE `skillid` = 1 ORDER BY `value` DESC, `player_id` ASC;")
	elseif param == "sword" then
		str = "# [Sword] - Name\n"
		value = "value"
		player_id = "player_id"
		result = db.getResult("SELECT `player_id`, `value` FROM `player_skills`  WHERE `skillid` = 2 ORDER BY `value` DESC, `player_id` ASC;")
	elseif param == "axe" then
		str = "# [Axe] - Name\n"
		value = "value"
		player_id = "player_id"
		result = db.getResult("SELECT `player_id`, `value` FROM `player_skills`  WHERE `skillid` = 3 ORDER BY `value` DESC, `player_id` ASC;")
	elseif param == "dist" or param == "distance" then
		str = "# [Distance] - Name\n"
		value = "value"
		player_id = "player_id"
		result = db.getResult("SELECT `player_id`, `value` FROM `player_skills`  WHERE `skillid` = 4 ORDER BY `value` DESC, `player_id` ASC;")
	elseif param == "shield" or param == "shielding" then
		str = "# [Shield] - Name\n"
		value = "value"
		player_id = "player_id"
		result = db.getResult("SELECT `player_id`, `value` FROM `player_skills`  WHERE `skillid` = 5 ORDER BY `value` DESC, `player_id` ASC;")
	elseif param == "fish" or param == "fishing" then
		str = "# [Fishing] - Name\n"
		value = "value"
		player_id = "player_id"
		result = db.getResult("SELECT `player_id`, `value` FROM `player_skills`  WHERE `skillid` = 6 ORDER BY `value` DESC, `player_id` ASC;")
	else
	     doPlayerSendTextMessage(cid, MESSAGE_STATUS_CONSOLE_BLUE, "Unbekannter skill: '" .. param .. "'")
	     return TRUE
	end
	if (result:getID() ~= -1) then
		local i = 1
		while TRUE do
			if not(result:next()) or i > players then
				break
			end
			str = str .. "\n " .. i .. ". [" .. result:getDataInt(value) .. "] - " .. (name ~= "" and result:getDataString("name") or getPlayerNameByGUID(result:getDataInt(player_id)))
			i = i+1
		end
		result:free()
	end
	if str ~= "" then
		doPlayerPopupFYI(cid, str)
	end
	return TRUE
end