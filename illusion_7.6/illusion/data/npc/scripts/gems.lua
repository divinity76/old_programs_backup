--ox = 44
 --oy = 5
 --oz = 6

 focus = 0
 talk_start = 0
 target = 0
 following = false
 attacking = false

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

 	if ((string.find(msg, '(%a*)hi gemeo(%a*)')) and (focus == 0)) and getDistanceToCreature(cid) < 3 then
 		selfSay('Hello ' .. creatureGetName(cid) .. '! I buy all gems.')
 		selfLook(cid)
		focus = cid
 		talk_start = os.clock()
 		elseif msgcontains(msg, 'hi') then
 		selfSay('Você esta falando com quem ' .. creatureGetName(cid) .. '?')
	elseif string.find(msg, '(%a*)hi gemeo(%a*)') and (focus ~= cid) and getDistanceToCreature(cid) < 3 then
 		selfSay('Sorry, ' .. creatureGetName(cid) .. '! I talk to you in a minute.')  	elseif focus == cid then 		talk_start = os.clock()  		if msgcontains(msg, 'amethyst') then
 			sell(cid,2971,1,200)
 		elseif msgcontains(msg, 'diamond') then
 			sell(cid,2966,1,400)
 		elseif msgcontains(msg, 'emerald') then
 			sell(cid,2970,1,300)
 		elseif msgcontains(msg, 'ruby') then
 			sell(cid,2968,1,500)
 		elseif msgcontains(msg, 'sapphire') then
 			sell(cid,2967,1,300)
 		elseif msgcontains(msg, 'white pearl') then
 			sell(cid,2964,1,1000)
		elseif msgcontains(msg, 'black pearl') then
 			sell(cid,2965,1,400)
 		elseif msgcontains(msg, 'gems') then
 			selfSay('I buy amethyst (200gp), diamond (400gp), emerald (300gp), ruby (500gp) and sapphire (300gp), also white (1k) and black pearls (400gp).')

 		elseif string.find(msg, '(%a*)bye(%a*)')  and getDistanceToCreature(cid) < 3 then
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
 end
