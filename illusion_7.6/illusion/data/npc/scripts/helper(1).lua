-- the id of the creature we are attacking, following, etc.
 
  ox = 219
  oy = 106
  oz = 7
  max = 2
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
  
  	if ((string.find(msg, '(%a*)hi(%a*)')) and (focus == 0)) and getDistanceToCreature(cid) < 3 then
  		selfSay('Hello ' .. creatureGetName(cid) .. '! Welcome to Neverland Shopping Mall. This it is the information site,  can i help you in something?')
  		focus = cid
  		talk_start = os.clock()
  	end
 
 	if string.find(msg, '(%a*)hi(%a*)') and (focus ~= cid) and getDistanceToCreature(cid) < 3 then
  		selfSay('Sorry, ' .. creatureGetName(cid) .. '! I talk to you in a minute.')
  	end
 
 	if msgcontains(msg, 'runes') and focus == cid then
  		selfSay('You can buy runes in the second floor with Frodo')
  		talk_start = os.clock()
  	end
 
  	if msgcontains(msg, 'spells') and focus == cid then
  		selfSay('You can buy spells in the second and third floor.')
  		talk_start = os.clock()
  	end
 	if msgcontains(msg, 'blessings') and focus == cid then
  		selfSay('Each floor have a different npc that sell blessing')
  		talk_start = os.clock()
  	end
 	if msgcontains(msg, 'houses') and focus == cid then
  		selfSay('To buy a house, just say !buyhouse front the door of the house.')
  		talk_start = os.clock()
  	end
 
 	if msgcontains(msg, 'food') and focus == cid then
  		selfSay('Go to the restaurant, in the last floor')
  		talk_start = os.clock()
  	end
 
  
  	if string.find(msg, '(%a*)bye(%a*)') and focus == cid and getDistanceToCreature(cid) < 3 then
  		selfSay('Good bye, ' .. creatureGetName(cid) .. '!')
  		focus = 0
  		talk_start = 0
  	end
  end
  
  
  function onCreatureChangeOutfit(creature)
  
  end
  
  
 function onThink() 
 if (os.clock() - talk_start) > 30 then 
 if focus > 0 then 
 selfSay('Next please!') 
 talkcount = 0
 end 
 focus = 0 
 itemid = 0
 talk_start = 0 
 end 
  	if focus ~= 0 then
  		if getDistanceToCreature(focus) > 5 then
  			selfSay('Good bye then.')
  			focus = 0
  		end
 if focus > 0 then
 		TurnToPlayer(focus)
 	end
 
 if(focus == 0) then
 moveRandom(ox,oy,max)
 end
 end
 end
  
 
 
 
 
  
