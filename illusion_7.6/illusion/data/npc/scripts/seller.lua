
focus = 0
  talk_start = 0
  target = 0
  following = false
  attacking = false
  ox = 145
  oy = 50
  oz = 7
  max = 5
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
  		selfSay('Hello, ' .. creatureGetName(cid) .. '! I sell ropes (50gps), shovels (20gps),pickaxe (100k FUCKERSHAHAHAH), backpacks in all colors (10gps), manafluids (100gps), lifefluids (60gps) and fishing rods (100gps). I buy vials (10gps).')
  		focus = cid
  		talk_start = os.clock()
 
  	elseif string.find(msg, '(%a*)hi(%a*)') and (focus ~= cid) and getDistanceToCreature(cid) < 3 then
  		selfSay('Sorry, ' .. creatureGetName(cid) .. '! I talk to you in a minute.')
 
  	elseif msgcontains(msg, 'rope') and focus == cid then
  		buy(cid,2120,1,50)
  		talk_start = os.clock()
 
  	elseif msgcontains(msg, 'shovel') and focus == cid then
  		buy(cid,2554,1,20)
  		talk_start = os.clock()
 
  	elseif msgcontains(msg, 'brown backpack') and focus == cid then
  		buy(cid,1988,1,10)
  		talk_start = os.clock()
 
 
 elseif msgcontains(msg, 'green backpack') and focus == cid then
  		buy(cid,1998,1,10)
  		talk_start = os.clock()
 
 elseif msgcontains(msg, 'yellow backpack') and focus == cid then
  		buy(cid,1999,1,10)
  		talk_start = os.clock()
 
 elseif msgcontains(msg, 'red backpack') and focus == cid then
  		buy(cid,2000,1,10)
  		talk_start = os.clock()
 
 elseif msgcontains(msg, 'purple backpack') and focus == cid then
  		buy(cid,2001,1,10)
  		talk_start = os.clock()
 
 elseif msgcontains(msg, 'blue backpack') and focus == cid then
  		buy(cid,2002,1,10)
  		talk_start = os.clock()
 
 elseif msgcontains(msg, 'black backpack') and focus == cid then
  		buy(cid,2003,1,10)
  		talk_start = os.clock()
 
 elseif msgcontains(msg, 'orange backpack') and focus == cid then
  		buy(cid,2004,1,10)
  		talk_start = os.clock()
 
 elseif msgcontains(msg, 'jungle backpack') and focus == cid then
  		buy(cid,3940,1,10)
  		talk_start = os.clock()
 
      elseif msgcontains(msg, 'pickaxe') and focus == cid then
  		buy(cid,2553,1,1000000)
  		talk_start = os.clock()
      elseif msgcontains(msg, 'pick') and focus == cid then
  		buy(cid,2553,1,1000000)
  		talk_start = os.clock()
 
  	elseif msgcontains(msg, 'manafluid') and focus == cid then
  		buy(cid,2006,7,100)
  		talk_start = os.clock()
 
  	elseif msgcontains(msg, '20 manafluids') and focus == cid then
  		BuyContainer(cid,2812,7,2000)
  		talk_start = os.clock()
 
  	elseif msgcontains(msg, 'ice rapier') and focus == cid then
  		BuyContainer(cid,3222,1,2000)
  		talk_start = os.clock()
 
  	elseif msgcontains(msg, 'lifefluid') and focus == cid then
  		buy(cid,2006,10,60)
  		talk_start = os.clock()
 
  	elseif msgcontains(msg, 'fishing rod') and focus == cid then
  		buy(cid,2580,1,100)
  		talk_start = os.clock()
 
 	elseif (msgcontains(msg, 'vial') or msgcontains(msg, 'flask')) and focus == cid then
  		sell(cid,2006,1,10)
  		talk_start = os.clock()
 
  	elseif string.find(msg, '(%a*)bye(%a*)') and focus == cid and getDistanceToCreature(cid) < 3 then
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