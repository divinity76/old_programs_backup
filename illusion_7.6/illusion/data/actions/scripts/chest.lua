function onUse(cid, item, frompos, item2, topos)
 
 
-- annihilator
   	if item.uid == 5006 then
   		queststatus = getPlayerStorageValue(cid,5010)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Demon Armor.")
   			doPlayerAddItem(cid,2494,1)
   			setPlayerStorageValue(cid,5010,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 5007 then
   		queststatus = getPlayerStorageValue(cid,5010)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Sword of Valor.")
   			doPlayerAddItem(cid,2400,1)
   			setPlayerStorageValue(cid,5010,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 5008 then
   		queststatus = getPlayerStorageValue(cid,5010)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Stonecutter's Axe.")
   			doPlayerAddItem(cid,2431,1)
   			setPlayerStorageValue(cid,5010,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 5009 then
   		queststatus = getPlayerStorageValue(cid,5010)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have a Teddy bear.")
   			doPlayerAddItem(cid,2112,1)
   			setPlayerStorageValue(cid,5010,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
--- behe
   	elseif item.uid == 5010 then
   		queststatus = getPlayerStorageValue(cid,5012)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Golden Armor.")
   			doPlayerAddItem(cid,2466,1)
   			setPlayerStorageValue(cid,5012,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 5011 then
   		queststatus = getPlayerStorageValue(cid,5013)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Demon Shield.")
   			doPlayerAddItem(cid,2520,1)
   			setPlayerStorageValue(cid,5013,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 5012 then
   		queststatus = getPlayerStorageValue(cid,5014)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Skull Staff.")
   			doPlayerAddItem(cid,2436,1)
   			setPlayerStorageValue(cid,5014,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 5014 then
   		queststatus = getPlayerStorageValue(cid,5015)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a MMS.")
   			doPlayerAddItem(cid,2514,1)
   			setPlayerStorageValue(cid,5015,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
--- Demon Quest
   	elseif item.uid == 5015 then
   		queststatus = getPlayerStorageValue(cid,5016)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Demon Helmet.")
   			doPlayerAddItem(cid,2493,1)
   			setPlayerStorageValue(cid,5016,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
--hero quest?
   	elseif item.uid == 5016 then
   		queststatus = getPlayerStorageValue(cid,5017)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Crown legs.")
   			doPlayerAddItem(cid,2488,1)
   			setPlayerStorageValue(cid,5017,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end

   	elseif item.uid == 5017 then
   		queststatus = getPlayerStorageValue(cid,5018)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Serpent Sword.")
   			doPlayerAddItem(cid,2409,1)
   			setPlayerStorageValue(cid,5018,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 5018 then
   		queststatus = getPlayerStorageValue(cid,5019)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Sprite wand.")
   			doPlayerAddItem(cid,2453,1)
   			setPlayerStorageValue(cid,5019,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
---hydra please
   	elseif item.uid == 5019 then
   		queststatus = getPlayerStorageValue(cid,5020)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Horned Helmet.")
   			doPlayerAddItem(cid,2496,1)
   			setPlayerStorageValue(cid,5020,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 5020 then
   		queststatus = getPlayerStorageValue(cid,5021)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Sprite Wand.")
   			doPlayerAddItem(cid,2453,1)
   			setPlayerStorageValue(cid,5021,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 5021 then
   		queststatus = getPlayerStorageValue(cid,5022)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Helmet of The Ancients.")
   			doPlayerAddItem(cid,2343,1)
   			setPlayerStorageValue(cid,5022,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   		end
   	elseif item.uid == 1005 then
   		queststatus = getPlayerStorageValue(cid,1005)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Crown.")
   			doPlayerAddItem(cid,2128,1)
   			setPlayerStorageValue(cid,1005,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
   	    end
   	elseif item.uid == 1006 then
   		queststatus = getPlayerStorageValue(cid,1006)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Crystal Mace.")
   			doPlayerAddItem(cid,2445,1)
   			setPlayerStorageValue(cid,1006,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 1007 then
   		queststatus = getPlayerStorageValue(cid,1007)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Million Gold Pieces.")
   			doPlayerAddItem(cid,2160,100)
   			setPlayerStorageValue(cid,1007,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 1008 then
   		queststatus = getPlayerStorageValue(cid,1008)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Demon Shield.")
   			doPlayerAddItem(cid,2520,1)
   			setPlayerStorageValue(cid,1008,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 1009 then
   		queststatus = getPlayerStorageValue(cid,1009)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Crusader Helmet.")
   			doPlayerAddItem(cid,2497,1)
   			setPlayerStorageValue(cid,1009,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end

   	elseif item.uid == 2469 then
   		queststatus = getPlayerStorageValue(cid,2469)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found Dragon scale legs.")
   			doPlayerAddItem(cid,2469,1)
   			setPlayerStorageValue(cid,2469,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 1993 then
   		queststatus = getPlayerStorageValue(cid,1993)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found Plasma shield.")
   			doPlayerAddItem(cid,2523,1)
   			setPlayerStorageValue(cid,1993,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 1994 then
   		queststatus = getPlayerStorageValue(cid,1994)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found Great axe.")
   			doPlayerAddItem(cid,2415,1)
   			setPlayerStorageValue(cid,1994,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 1995 then
   		queststatus = getPlayerStorageValue(cid,1995)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found Golden helmet.")
   			doPlayerAddItem(cid,2471,1)
   			setPlayerStorageValue(cid,1995,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 1996 then
   		queststatus = getPlayerStorageValue(cid,1996)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found Crocodile boots.")
   			doPlayerAddItem(cid,3982,1)
   			setPlayerStorageValue(cid,1996,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 1997 then
   		queststatus = getPlayerStorageValue(cid,1997)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Red robe.")
   			doPlayerAddItem(cid,2655,1)
   			setPlayerStorageValue(cid,1997,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 1992 then
   		queststatus = getPlayerStorageValue(cid,1992)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a warlord sword.")
   			doPlayerAddItem(cid,2408,1)
   			setPlayerStorageValue(cid,1992,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 1998 then
   		queststatus = getPlayerStorageValue(cid,1998)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Charmer Tiara.")
   			doPlayerAddItem(cid,3971,1)
   			setPlayerStorageValue(cid,1998,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 1999 then
   		queststatus = getPlayerStorageValue(cid,1999)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found Gods Spear.")
   			doPlayerAddItem(cid,3965,1)
   			setPlayerStorageValue(cid,1999,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 1911 then
   		queststatus = getPlayerStorageValue(cid,1911)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found Soft Boots.")
   			doPlayerAddItem(cid,2640,1)
   			setPlayerStorageValue(cid,1911,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 1910 then
   		queststatus = getPlayerStorageValue(cid,1910)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found MasterMind shield.")
   			doPlayerAddItem(cid,2514,1)
   			setPlayerStorageValue(cid,1910,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 1912 then
   		queststatus = getPlayerStorageValue(cid,1912)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Golden Helmet.")
   			doPlayerAddItem(cid,2471,1)
   			setPlayerStorageValue(cid,1912,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 1913 then
   		queststatus = getPlayerStorageValue(cid,1913)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Rose Shield.")
   			doPlayerAddItem(cid,2527,1)
   			setPlayerStorageValue(cid,1913,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 1914 then
   		queststatus = getPlayerStorageValue(cid,1914)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a Horsemens helmet.")
   			doPlayerAddItem(cid,3969,1)
   			setPlayerStorageValue(cid,1914,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 2222 then
   		queststatus = getPlayerStorageValue(cid,2222)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found a pick.")
   			doPlayerAddItem(cid,2553,1)
   			setPlayerStorageValue(cid,2222,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
   	elseif item.uid == 9999 then
   		queststatus = getPlayerStorageValue(cid,9999)
   		if queststatus == -1 then
   			doPlayerSendTextMessage(cid,22,"You have found BOOTS!.")
   			doPlayerAddItem(cid,2641,1)
   			setPlayerStorageValue(cid,9999,1)
   		else
   			doPlayerSendTextMessage(cid,22,"It is empty.")
	end
	else
		return 0
   	end

   	return 1
end
