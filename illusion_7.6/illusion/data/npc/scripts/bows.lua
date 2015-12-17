ox = 222
  oy = 106
  oz = 4
  max = 7
  
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
  		selfSay('Hello ' .. creatureGetName(cid) .. '! I am the famous Robin Hood and i want sell crossbows (200gps), bows (100gps), arrows (100gps), bolts (150gps), powerbolt (2mil), burst (1mil) and spears (100gps). All the money that i gain is for the poor people')
  		focus = cid
  		talk_start = os.clock()
  	end
  
  	if string.find(msg, '(%a*)hi(%a*)') and (focus ~= cid) and getDistanceToCreature(cid) < 3 then
  		selfSay('Sorry, ' .. creatureGetName(cid) .. '! I talk to you in a minute.')
  	end
  
  	if msgcontains(msg, 'crossbow') and focus == cid then
  		buy(cid,2455,1,200)
  		talk_start = os.clock()
  	end
  
  	if msgcontains(msg, 'bow') and focus == cid then
  		buy(cid,2456,1,100)
  		talk_start = os.clock()
  	end
  
  	if msgcontains(msg, 'arrows') and focus == cid then
  		buy(cid,2544,100,100)
  		talk_start = os.clock()
  	end
  
  	if msgcontains(msg, 'bolts') and focus == cid then
  		buy(cid,2543,100,150)
  		talk_start = os.clock()
  	end
  	if msgcontains(msg, 'burst') and focus == cid then
  		buy(cid,2546,1,1000000)
  		talk_start = os.clock()
  	end
  
  	if msgcontains(msg, 'powerbolt') and focus == cid then
  		buy(cid,2547,1,2000000)
  		talk_start = os.clock()
  	end
 
  	if msgcontains(msg, 'spears') and focus == cid then
  		buy(cid,2389,10,50)
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