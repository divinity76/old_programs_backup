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
  		selfSay('Hello ' .. creatureGetName(cid) .. '! Welcome to the famous Rulys Restaurant.. I sell ham, meat, carrots, apples, brown breads, brown mushroom, eggs, beer, wine,lemonade...')
  		focus = cid
  		talk_start = os.clock()
  	end
  
  	if string.find(msg, '(%a*)hi(%a*)') and (focus ~= cid) and getDistanceToCreature(cid) < 3 then
  		selfSay('Sorry, ' .. creatureGetName(cid) .. '! I talk to you in a minute.')
  	end
  
  	if msgcontains(msg, 'brown bread') and focus == cid then
  		buy(cid,2691,1,8)
  		talk_start = os.clock()
  	end
  
  	if msgcontains(msg, 'ham') and focus == cid then
  		buy(cid,2671,1,8)
  		talk_start = os.clock()
  	end
      if msgcontains(msg, '100 hams') and focus == cid then
  		buy(cid,2671,100,800)
  		talk_start = os.clock()
  	end
  
  	if msgcontains(msg, 'carrot') and focus == cid then
  		buy(cid,2684,1,8)
  		talk_start = os.clock()
  	end
  
  	if msgcontains(msg, 'meat') and focus == cid then
  		buy(cid,2666,1,8)
  		talk_start = os.clock()
  	end
 
 	if msgcontains(msg, '100 meats') and focus == cid then
  		buy(cid,2666,100,800)
  		talk_start = os.clock()
  	end
  
  	if msgcontains(msg, 'apple') and focus == cid then
  		buy(cid,2674,1,8)
  		talk_start = os.clock()
  	end
  
  	if msgcontains(msg, 'brown mushroom') and focus == cid then
  		buy(cid,2789,1,8)
  		talk_start = os.clock()
  	end
 
 	if msgcontains(msg, '100 brown mushrooms') and focus == cid then
  		buy(cid,2789,100,800)
  		talk_start = os.clock()
  	end
  
  	if msgcontains(msg, 'egg') and focus == cid then
  		buy(cid,2695,1,8)
  		talk_start = os.clock()
  	end
  	if msgcontains(msg, 'beer') and focus == cid then
  		buy(cid,2006,3,5)
  		talk_start = os.clock()
  	end
  	if msgcontains(msg, 'wine') and focus == cid then
  		buy(cid,2006,15,5)
  		talk_start = os.clock()
  	end
  	if msgcontains(msg, 'lemonade') and focus == cid then
  		buy(cid,2006,5,3)
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
  end
  
