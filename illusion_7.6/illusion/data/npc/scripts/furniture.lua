ox = 218
  oy = 109
  oz = 8
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
  		selfSay('Hello ' .. creatureGetName(cid) .. '! Welcome to the famous Rulys Furniture Shop. I sell chairs, tables, plants, water pipe, containers, pillows and more.')
  		focus = cid
  		talk_start = os.clock()
 	elseif string.find(msg, '(%a*)hi(%a*)') and (focus ~= cid) and getDistanceToCreature(cid) < 3 then
  		selfSay('Sorry, ' .. creatureGetName(cid) .. '! I talk to you in a minute.')  	elseif focus == cid then
 		talk_start = os.clock()
 
 		if msgcontains(msg, 'wooden chair') then
  			buy(cid,3901,1,500)
 		elseif msgcontains(msg, 'sofa chair') then
  			buy(cid,3902,1,500)
 		elseif msgcontains(msg, 'red cushioned chair') then
  			buy(cid,3903,1,500)
 		elseif msgcontains(msg, 'green cushioned chair') then
  			buy(cid,3904,1,500)
 		elseif msgcontains(msg, 'tusk chair') then
  			buy(cid,3905,1,500)
 		elseif msgcontains(msg, 'ivory chair') then
  			buy(cid,3906,1,500)
  		elseif msgcontains(msg, 'chairs') then
  			selfSay('I sell wooden, sofa, red cushioned, green cushioned, tusk and ivory chairs.')
 
 		elseif msgcontains(msg, 'big table') then
  			buy(cid,3909,1,500)
 		elseif msgcontains(msg, 'square table') then
  			buy(cid,3910,1,500)
 		elseif msgcontains(msg, 'round table') then
  			buy(cid,3911,1,500)
 		elseif msgcontains(msg, 'small table') then
  			buy(cid,3912,1,500)
 		elseif msgcontains(msg, 'stone table') then
  			buy(cid,3913,1,500)
 		elseif msgcontains(msg, 'tusk table') then
  			buy(cid,3914,1,500)
 		elseif msgcontains(msg, 'bamboo table') then
  			buy(cid,3919,1,500)
 		elseif msgcontains(msg, 'tables') then
  			selfSay('I sell big, square, round, small, stone, tusk, bamboo tables.')
 
 		elseif msgcontains(msg, 'pink flower') then
  			buy(cid,3928,1,500)
 		elseif msgcontains(msg, 'green flower') then
  			buy(cid,3929,1,500)
 		elseif msgcontains(msg, 'christmas tree') then
  			buy(cid,3931,1,500)
 		elseif msgcontains(msg, 'plants') then
  			selfSay('I sell pink and green flowers, also christmas trees.')
 
 
 		elseif msgcontains(msg, 'large trunk') then
  			buy(cid,3938,1,500)
 		elseif msgcontains(msg, 'drawer') then
  			buy(cid,3921,1,500)
 		elseif msgcontains(msg, 'dresser') then
  			buy(cid,3932,1,500)
 		elseif msgcontains(msg, 'locker') then
  			buy(cid,3934,1,500)
 		elseif msgcontains(msg, 'trough') then
  			buy(cid,3935,1,500)
 		elseif msgcontains(msg, 'box') then
  			buy(cid,1738,1,500)
 		elseif msgcontains(msg, 'containers') then
  			selfSay('I sell large trunks, boxes, drawers, dressers, lockers and troughs.')
 
 		elseif msgcontains(msg, 'coal basin') then
  			buy(cid,3908,1,500)
 		elseif msgcontains(msg, 'birdcage') then
  			buy(cid,3918,1,500)
 		elseif msgcontains(msg, 'harp') then
  			buy(cid,3917,1,500)
 		elseif msgcontains(msg, 'piano') then
  			buy(cid,3926,1,500)
 		elseif msgcontains(msg, 'globe') then
  			buy(cid,3927,1,500)
 		elseif msgcontains(msg, 'clock') then
  			buy(cid,3933,1,500)
 		elseif msgcontains(msg, 'lamp') then
  			buy(cid,3937,1,500)
 		elseif msgcontains(msg, 'more')  then
  			selfSay('I sell coal basins, birdcages, harps, pianos, globes, clocks and lamps.')
 
 		elseif msgcontains(msg, 'small purple pillow') then
  			buy(cid,1678,1,500)
 		elseif msgcontains(msg, 'small green pillow') then
  			buy(cid,1679,1,500)
 		elseif msgcontains(msg, 'small red pillow') then
  			buy(cid,1680,1,500)
 		elseif msgcontains(msg, 'small blue pillow') then
  			buy(cid,1681,1,500)
 		elseif msgcontains(msg, 'small orange pillow') then
  			buy(cid,1682,1,500)
 		elseif msgcontains(msg, 'small turquiose pillow') then
  			buy(cid,1683,1,500)
 		elseif msgcontains(msg, 'small white pillow') then
  			buy(cid,1684,1,500)
 		elseif msgcontains(msg, 'heart pillow') then
  			buy(cid,1685,1,500)
 		elseif msgcontains(msg, 'blue pillow') then
  			buy(cid,1686,1,500)
 		elseif msgcontains(msg, 'red pillow') then
  			buy(cid,1687,1,500)
 		elseif msgcontains(msg, 'green pillow') then
  			buy(cid,1688,1,500)
 		elseif msgcontains(msg, 'yellow pillow') then
  			buy(cid,1689,1,500)
 		elseif msgcontains(msg, 'round blue pillow') then
  			buy(cid,1690,1,500)
 		elseif msgcontains(msg, 'round red pillow') then
  			buy(cid,1691,1,500)
 		elseif msgcontains(msg, 'round purple pillow') then
  			buy(cid,1692,1,500)
 		elseif msgcontains(msg, 'round turquiose pillow') then
  			buy(cid,1693,1,500)
 		elseif msgcontains(msg, 'small') then
  			selfSay('I sell small purple, small green, small red, small blue, small orange, small turquiose and small white pillows.')
 		elseif msgcontains(msg, 'round') then
  			selfSay('I sell round blue, round red, round purple and round turquiose pillows.')
 		elseif msgcontains(msg, 'square') then
  			selfSay('I sell blue, red, green and yellow pillows.')
 		elseif msgcontains(msg, 'pillows') then
  			selfSay('I sell heart, small, sqare and round pillows.')
 
 		elseif msgcontains(msg, 'water pipe') then
  			buy(cid,2093,1,200)
 
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