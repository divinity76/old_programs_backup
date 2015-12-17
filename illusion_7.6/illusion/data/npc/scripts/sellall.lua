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

  	if (msgcontains(msg, 'hi') and (focus == 0)) and getDistanceToCreature(cid) < 4 then
  		selfSay('Hiho ' .. creatureGetName(cid) .. '! I sell Armors, Helmets, Legs, Shields, Clubs, Axes, Swords, Amulets and Rings.')
  		focus = cid
  		talk_start = os.clock()

  	elseif msgcontains(msg, 'hi') and (focus ~= cid) and getDistanceToCreature(cid) < 4 then
  		selfSay('Sorry, ' .. creatureGetName(cid) .. '! I talk to you in a minute.')

	elseif focus == cid then
		talk_start = os.clock()

		if msgcontains(msg, 'amulets')  then
  			selfSay('I sell Platinum Amulets (5k) and Scarfs (1k).')				elseif msgcontains(msg, 'axes')  then
  			selfSay('I sell , Guardian Halberds (15k), Fire Axes (20k), Knight Axes (4k), Double Axes (400gp) and Battle Axes (200gp), Dragon Lances (20k), Halberds (400gp) and Hatchets (40gp).')
		elseif msgcontains(msg, 'swords')  then
  			selfSay('I sell  Giant Swords (20k), Bright Swords(12k), Fire Swords(6k) Serpent Swords (3k), Spike Swords(1,6k) and Two-Handed Swords (800gp), also Ice Rapiers (8k), Broad Swords (140gp), Short Swords (60gp), Sabres (50gp) and Swords (50gp).')
		elseif msgcontains(msg, 'clubs')  then
  			selfSay('I sell  War Hammers (12k), Dragon Hammers (4k) and Battle Hammers (120gp), also Skull Staffs (20k) and Clerical Maces (400gp).')
		elseif msgcontains(msg, 'armors') then
			selfSay('I sell Magic Plate Armors (200k), Dragon Scale Mails (120k), Golden Armors (60k), Crown Armors (40k),  and Knight Armors (10k). I do not sell Demon Armors, because they are bugged.')
		elseif msgcontains(msg, 'shields')  then
  			selfSay('i sell Demon Shields (80k), Vampire Shields (50k), Medusa Shields (16k), Amazon Shields (8k), Crown Shields (10k), Tower Shields (8k), Dragon Shields (6k), Guardian Shields (4k), Beholder Shields (2k), and Dwarven Shields (200gp), also Mastermind Shields (160k)')
		elseif msgcontains(msg, 'legs')  then
  			selfSay('I sell Dragon Scale Legs (200k), Golden Legs (160k), Crown Legs (30k), Knight Legs (12k), Plate Legs (1k), Brass Legs (200gp) and Chain Legs (100gp).')
		elseif msgcontains(msg, 'boots')  then
  			selfSay('I sell Golden Boots (200k), Steel Boots (80k) and Boots of Haste (80k).')
			
		elseif msgcontains(msg, 'steel boots') and focus == cid then
  			sell(cid,2645,1,80000)
  		elseif msgcontains(msg, 'boh') or msgcontains(msg, 'boots of haste') and focus == cid then
  			sell(cid,2195,1,80000)
		elseif msgcontains(msg, 'golden boots') and focus == cid then
  			sell(cid,2646,1,200000)
  			
		elseif msgcontains(msg, 'magic plate armor') or msgcontains(msg, 'mpa') then
			buy(cid,2472,1,200000)
		elseif msgcontains(msg, 'dragon scale mail') or msgcontains(msg, 'dsm') then
			buy(cid,2492,1,120000)
		elseif msgcontains(msg, 'golden armor') then
			buy(cid,2189,1,60000)
		elseif msgcontains(msg, 'crown armor') then
			buy(cid,2487,1,40000)
		elseif msgcontains(msg, 'knight armor') then
			buy(cid,2476,1,10000)





		elseif msgcontains(msg, 'royal helmet') then
			buy(cid,2498,1,80000)
		elseif msgcontains(msg, 'crusader helmet') then
			buy(cid,2497,1,18000)
		elseif msgcontains(msg, 'warrior helmet') then
			buy(cid,2475,1,12000)
		elseif msgcontains(msg, 'crown helmet') then
			buy(cid,2491,1,10000)
		elseif msgcontains(msg, 'lady helmet') then
			buy(cid,2499,1,8000)
			

		elseif msgcontains(msg, 'golden legs') then
			buy(cid,2470,1,160000)

		elseif msgcontains(msg, 'crown legs') then
			buy(cid,2488,1,30000)
		elseif msgcontains(msg, 'knight legs') then
			buy(cid,2477,1,12000)
		elseif msgcontains(msg, 'plate legs') then
			buy(cid,2647,1,1000)
		elseif msgcontains(msg, 'brass legs') then
			buy(cid,2478,1,200)
			elseif msgcontains(msg, 'chain legs') then
			buy(cid,2648,1,100)
			

		elseif msgcontains(msg, 'mms') or msgcontains(msg, 'mastermind shield') then
			buy(cid,2514,1,160000)

		elseif msgcontains(msg, 'demon shield') then
			buy(cid,2520,1,80000)
		elseif msgcontains(msg, 'vampire shield') then
			buy(cid,2534,1,50000)
		elseif msgcontains(msg, 'medusa shield') then
			buy(cid,2536,1,16000)
		elseif msgcontains(msg, 'crown shield') then
			buy(cid,2519,1,10000)
		elseif msgcontains(msg, 'tower shield') then
			buy(cid,2528,1,8000)
		elseif msgcontains(msg, 'amazon shield') then
			buy(cid,2537,1,8000)
		elseif msgcontains(msg, 'dragon shield') then
			buy(cid,2516,1,6000)
		elseif msgcontains(msg, 'guardian shield') then
			buy(cid,2515,1,4000)
		elseif msgcontains(msg, 'beholder shield') then
			buy(cid,2518,1,3000)
			elseif msgcontains(msg, 'dwarven shield') then
			buy(cid,2525,1,200)
			



		elseif msgcontains(msg, 'giant sword') then
  			buy(cid,2393,1,20000)		elseif msgcontains(msg, 'bright sword') then
  			buy(cid,2407,1,12000)		elseif msgcontains(msg, 'ice rapier')  then
  			buy(cid,2396,1,8000)		elseif msgcontains(msg, 'fire sword') then
  			buy(cid,2392,1,6000)		elseif msgcontains(msg, 'serpent sword')  then
  			buy(cid,2409,1,3000)		elseif msgcontains(msg, 'spike sword')  then
  			buy(cid,2383,1,1600)
  		elseif msgcontains(msg, 'two handed sword')  then
  			buy(cid,2377,1,800)
		elseif msgcontains(msg, 'broad sword') then
  			buy(cid,2413,1,140)
		elseif msgcontains(msg, 'short sword') then
  			buy(cid,2406,1,60)		elseif msgcontains(msg, 'sabre') then
  			buy(cid,2385,1,50)
  		elseif msgcontains(msg, 'sword')  then
  			buy(cid,2376,1,50)
			
		elseif msgcontains(msg, 'dragon lance')  then
  			buy(cid,2414,1,20000)
		elseif msgcontains(msg, 'guardian halberd')  then
  			buy(cid,2427,1,15000)
  		elseif msgcontains(msg, 'fire axe')  then
  			buy(cid,2432,1,20000)		elseif msgcontains(msg, 'knight axe')  then
  			buy(cid,2430,1,4000)		elseif msgcontains(msg, 'double axe')  then
  			buy(cid,2387,1,400)		elseif msgcontains(msg, 'halberd')  then
  			buy(cid,2381,1,400)		elseif msgcontains(msg, 'battle axe')  then
  			buy(cid,2378,1,200)
  		elseif msgcontains(msg, 'hatchet')  then
  			buy(cid,2388,1,40)		elseif msgcontains(msg, 'axes')  then
		
		elseif msgcontains(msg, 'war hammer') then
  			buy(cid,2391,1,12000)		elseif msgcontains(msg, 'skull staff') then
  			buy(cid,2436,1,20000)
  		elseif msgcontains(msg, 'dragon hammer')  then
  			buy(cid,2434,1,4000)
  		elseif msgcontains(msg, 'clerical mace')  then
  			buy(cid,2423,1,400)
  		elseif msgcontains(msg, 'battle hammer')  then
  			buy(cid,2417,1,120)
  		elseif msgcontains(msg, 'mace') then
  			buy(cid,2398,1,60)

	        elseif msgcontains(msg, 'might ring') then
  			buy(cid,2164,1,60)
		elseif msgcontains(msg, 'stealth ring') then
  			buy(cid,2165,1,60)
		elseif msgcontains(msg, 'power ring') then
  			buy(cid,2166,1,60)
		elseif msgcontains(msg, 'energy ring') then
  			buy(cid,2167,1,60)
		elseif msgcontains(msg, 'life ring') then
  			buy(cid,2168,1,60)
		elseif msgcontains(msg, 'time ring') then
  			buy(cid,2169,1,60)
		elseif msgcontains(msg, 'dwarven ring') then
  			buy(cid,2213,1,60)
		elseif msgcontains(msg, 'ring of healing') then
  			buy(cid,2214,1,60)
		elseif msgcontains(msg, 'platinum amulet') then
  			buy(cid,2171,1,10000)		elseif msgcontains(msg, 'scarf') then
  			buy(cid,2661,1,1000)		elseif msgcontains(msg, 'amulets')  then
		

		elseif string.find(msg, '(%a*)bye(%a*)') and getDistanceToCreature(cid) < 4 then
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
end
