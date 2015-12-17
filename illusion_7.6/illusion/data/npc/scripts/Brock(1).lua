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
          selfSay('Até Logo.')
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
 
  	if ((string.find(msg, '(%a*)oi(%a*)')) and (focus == 0)) and getDistanceToCreature(cid) < 4 then
  		selfSay('Olá, ' .. creatureGetName(cid) .. '! Eu vendo instrumentos musicais; Fanfare(100gps), Harp(150gps), Drum (200gps), Panpipes(300gps), Lute(200gps), Lyre(100gps), Royal Fanfare(1k) !!.')
  		focus = cid
  		talk_start = os.clock()
 
  	elseif string.find(msg, '(%a*)oi(%a*)') and (focus ~= cid) and getDistanceToCreature(cid) < 4 then
  		selfSay('Desculpa, ' .. creatureGetName(cid) .. '! Aguarde 1 minuto.')
 
  	elseif msgcontains(msg, 'fanfare') and focus == cid then
  		buy(cid,2076,1,100)
  		talk_start = os.clock()
 
  	elseif msgcontains(msg, 'harp') and focus == cid then
  		buy(cid,2084,1,150)
  		talk_start = os.clock()
 
  	elseif msgcontains(msg, 'drum') and focus == cid then
  		buy(cid,2073,1,200)
  		talk_start = os.clock()
 
  	elseif msgcontains(msg, 'panpines') or msgcontains(msg, 'panpines') and focus == cid then
  		buy(cid,2074,1,300)
  		talk_start = os.clock()
 
  	elseif msgcontains(msg, 'lute') or msgcontains(msg, 'lute') and focus == cid then
  		buy(cid,2072,1,200)
  		talk_start = os.clock()
 
  	elseif msgcontains(msg, 'lyre') and focus == cid then
  		buy(cid,2071,1,100)
  		talk_start = os.clock()
 
       elseif msgcontains(msg, 'royal fanfare') and focus == cid then
  		buy(cid,2077,1,1000)
  		talk_start = os.clock()
 
 
  	elseif string.find(msg, '(%a*)tchau(%a*)') and focus == cid and getDistanceToCreature(cid) < 4 then
  		selfSay('Tchau, ' .. creatureGetName(cid) .. '!')
  		focus = 0
  		talk_start = 0
  	end
  end
 
 
  function onCreatureChangeOutfit(creature)
 
  end
 
 
  function onThink()
  	if (os.clock() - talk_start) > 30 then
  		if focus > 0 then
  			selfSay('Próximo por favor...')
  		end
  			focus = 0
  	end
 	if focus ~= 0 then
 		if getDistanceToCreature(focus) > 5 then
 			selfSay('Tchau.')
 			focus = 0
 		end
 	end
  end
 
