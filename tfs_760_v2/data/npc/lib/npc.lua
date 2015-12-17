-- Include external classes.
dofile(getDataDir() .. 'npc/lib/npcsystem/npcsystem.lua')

-- Callback for isPremium(cid) so Jiddo's npcsystem works
isPlayerPremiumCallback = isPremium

-- move to a creature
function moveToCreature(id)
	if(isCreature(id) == FALSE) then
		debugPrint('moveToCreature(): creature not found.')
		return LUA_ERROR
	end

	local pos = getCreaturePosition(id)
	selfMoveTo(pos.x, pos.y, pos.z)
	return LUA_NO_ERROR
end

-- do one step to reach position
function moveToPosition(x,y,z)
	selfMoveTo(x, y, z)
end


-- stop talking
function selfGotoIdle()
		following = false
		attacking = false
		selfAttackCreature(0)
		target = 0
end

function getCount(msg)
	b, e = string.find(msg, "%d+")
	
	if b == nil or e == nil then
		count = 1
	else
		count = tonumber(string.sub(msg, b, e))
	end
	
	if count > 2000 then
		count = 2000
	end
	
	return count
end

-- buy an item
function buy(cid, itemid, count, cost)
	cost = count*cost
	amount = count
	if doPlayerRemoveMoney(cid, cost) == 1 then
		if getItemStackable(itemid) then
			while count > 100 do
				doPlayerAddItem(cid, itemid, 100)
				count = count - 100
			end
			
			doPlayerAddItem(cid, itemid, count) -- add the last items, if there is left
		else
			while count > 0 do
				doPlayerAddItem(cid, itemid, 1)
				count = count - 1
			end
		end
		
		if amount <= 1 then
			selfSay('Here is your '.. getItemName(itemid) .. '!')
		else
			selfSay('Here are your '.. amount ..' '.. getItemName(itemid) .. 's!')		
		end
	else
		selfSay('Sorry, you do not have enough money.')
	end
end

function buyFluidContainer(cid, itemid, count, cost, fluidtype)
	cost = count*cost
	amount = count
	if doPlayerRemoveMoney(cid, cost) == 1 then
		while count > 0 do
			doPlayerAddItem(cid, itemid, fluidtype)
			count = count - 1
		end
		
		if amount <= 1 then
			selfSay('Here is your '.. getItemName(itemid) .. '!')
		else
			selfSay('Here are your '.. amount ..' '.. getItemName(itemid) .. 's!')		
		end
	else
		selfSay('Sorry, you do not have enough money.')
	end
end

-- sell an item
function sell(cid, itemid, count, cost)
	cost = count*cost
	if doPlayerRemoveItem(cid, itemid, count) == 1 then
		coins = math.floor(cost/10000)
		crystals = coins
		while coins > 100 do
			doPlayerAddItem(cid, 2160, 100)
			coins = coins - 100
		end
		
		doPlayerAddItem(cid, 2160, coins)
		cost = cost - crystals*10000
		
		coins = math.floor(cost/100)
		if coins > 0 then
			doPlayerAddItem(cid, 2152, coins)
			cost = cost - coins*100
		end
		coins = cost
		if cost > 0 and cost < 100 then
			doPlayerAddItem(cid, 2148, coins)
			cost = cost - coins
		end
		
		if cost > 0 then
			selfSay('You couldn\'t retrieve '.. cost ..' gold pieces, please contact the admin.')
		end
		
		if count <= 1 then
			selfSay('Thanks for this '.. getItemName(itemid) .. '!')
		else
			selfSay('Thanks for these '.. count..' '.. getItemName(itemid) .. 's!')		
		end
	else
		selfSay('Sorry, you do not have this item.')
	end
end

-- pay for anything?
function pay(cid, cost)
	if doPlayerRemoveMoney(cid, cost) == 1 then
		return true
	else
		return false
	end
end


-- Travel player
function travel(cid, x, y, z)
	destpos = {x = x, y = y, z = z}
	doTeleportThing(cid, destpos)
	doSendMagicEffect(destpos, 10)
end

function msgcontains(txt, str)
	return (string.find(txt, str) and not string.find(txt, '(%w+)' .. str) and not string.find(txt, str .. '(%w+)'))
end