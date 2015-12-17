focus = 0
talk_start = 0
target = 0
following = false
attacking = false
talk_state = 0

function onThingMove(creature, thing, oldpos, oldstackpos)

end


function onCreatureAppear(creature)

end


function onCreatureDisappear(cid, pos)
  	if focus == cid then
          selfSay('Good bye then.')
          focus = 0
          talk_start = 0
  	end
end


function onCreatureTurn(creature)

end
function msgcontains(txt, str)
  	return (string.find(txt, str) and not string.find(txt, '(%w+)' .. str) and not string.find(txt, str .. '(%w+)'))
end


function onCreatureSay(cid, type, msg)
  	msg = string.lower(msg)

  	if (msgcontains(msg, 'hi') and (focus == 0)) and getDistanceToCreature(cid) < 4 then
 		selfSay('Hello ' .. creatureGetName(cid) .. '! I sell premium, promotions and aols.')
 		selfLook(cid)
		focus = cid
 		talk_start = os.clock()
	elseif msgcontains(msg, 'hi') and (focus ~= cid) and getDistanceToCreature(cid) < 4 then
  		selfSay('Sorry, ' .. creatureGetName(cid) .. '! I talk to you in a minute.')  	elseif focus == cid then
		talk_start = os.clock()

		if msgcontains(msg, 'promotion') or msgcontains(msg, 'promotiom') then
 			if isPromoted(cid) then
 				selfSay('Sorry, you are already promoted.')
 				talk_state = 0
 			elseif getPlayerLevel(creatureGetName(cid)) < 20 then
				selfSay('Sorry, you need level 20 to buy promotion.')
				talk_state = 0
			else
				selfSay('Do you want to buy promotion for 20000 gold pieces?')
				talk_state = 1
			end

		elseif msgcontains(msg, 'premium') then
			if isPremium(cid) then
 				selfSay('Sorry, you already have premium, come back when it will end.')
				talk_state = 0
			else
				selfSay('I can sell premium for 10days(10k), 30days(30k), 90days(90k), how many days you want to buy?')
				talk_state = 2
			end
			
		elseif msgcontains(msg, 'aol') or msgcontains(msg, 'amulet of loss') then
			selfSay('Do you want to buy an amulet of loss for 10000 gold pieces?')
		talk_state = 3

		elseif talk_state == 1 then
			if msgcontains(msg, 'yes') then
				if pay(cid,20000) then
					selfSay('/promote ' .. creatureGetName(cid))
					selfSay('You are now promoted!')
				else
					selfSay('Sorry, you do not have enough money.')
				end
 			end
			talk_state = 0

	elseif talk_state == 3 then
	if msgcontains(msg, 'yes') then
	if pay(cid,10000) then
	buy(cid,2173,1,0)
	selfSay('Here it is.')
	else
	selfSay('Sorry, you do not have enough money.')
	end
	end
	talk_state = 0

		elseif talk_state == 2 then
			if msgcontains(msg, '10') then
				if pay(cid,10000) then
					selfSay('/premmy 10 ' .. creatureGetName(cid))
					selfSay('OK you have 10 Premium days, buy another only if this will end.')
				else
					selfSay('Sorry, you do not have enough money.')
				end
			elseif msgcontains(msg, '30') then
				if pay(cid,30000) then
					selfSay('/premmy 30 ' .. creatureGetName(cid))
					selfSay('OK you have 30 Premium days, buy another only if this will end.')
				else
					selfSay('Sorry, you do not have enough money.')
				end
			elseif msgcontains(msg, '90') then
				if pay(cid,90000) then
					selfSay('/premmy 90 ' .. creatureGetName(cid))
					selfSay('OK you have 90 Premium days, buy another only if this will end.')
				else
					selfSay('Sorry, you do not have enough money.')
				end
			end
			talk_state = 0

  		elseif msgcontains(msg, 'bye')  and getDistanceToCreature(cid) < 4 then
  			selfSay('Good bye, ' .. creatureGetName(cid) .. '!')
  			focus = 0
  			talk_start = 0
  		end
  	end
end


function onCreatureChangeOutfit(creature)

end


function onThink()
if (os.clock() - talk_start) > 30 then
if focus > 0 then
selfSay('Next Please...')
end
focus = 0
end
if focus ~= 0 then
if getDistanceToCreature(focus) > 5 then
selfSay('Good bye then.')
focus = 0
end
end
if focus == 0 then
cx, cy, cz = selfGetPosition()
randmove = math.random(1,20)
if randmove == 1 then
nx = cx + 1
end
if randmove == 2 then
nx = cx - 1
end
if randmove == 3 then
ny = cy + 1
end
if randmove == 4 then
ny = cy - 1
end
if randmove >= 5 then
nx = cx
ny = cy
end
moveToPosition(nx, ny, cz)
--summons = 30
--summons2 = 30
end
end