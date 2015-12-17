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
         selfSay('Good Bye.')
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
  		selfSay('Hello ' .. creatureGetName(cid) .. '! I am the son of Bill Gates so i have much money for buy swords, clubs, axes, helmets, boots, legs, shields and armors.')
  		focus = cid
  		talk_start = os.clock()
 	elseif string.find(msg, '(%a*)hi(%a*)') and (focus ~= cid) and getDistanceToCreature(cid) < 3 then
  		selfSay('Sorry, ' .. creatureGetName(cid) .. '! I talk to you in a minute.')  	elseif focus == cid then
 		talk_start = os.clock()
 
 		if msgcontains(msg, 'royal helmet') then
  			sell(cid,2498,1,20000)
  		elseif msgcontains(msg, 'warrior helmet') then
  			sell(cid,2475,1,3000)
  		elseif msgcontains(msg, 'crusader helmet') then
  			sell(cid,2497,1,3000)
  		elseif msgcontains(msg, 'crown helmet') then
  			sell(cid,2491,1,2000)
             elseif msgcontains(msg, 'lady helmet') then
  			sell(cid,2499,1,2500)
  		elseif msgcontains(msg, 'devil helmet') then
  			sell(cid,2462,1,2000)
  		elseif msgcontains(msg, 'mystic turban') then
  			sell(cid,2663,1,400)
  		elseif msgcontains(msg, 'chain helmet') then
  			sell(cid,2458,1,35)
             elseif msgcontains(msg, 'steel helmet') then
  			sell(cid,2457,1,300)
             elseif msgcontains(msg, 'dark helmet') then
  			sell(cid,2490,1,150)
 		elseif msgcontains(msg, 'iron helmet') then
  			sell(cid,2459,1,30)
             elseif msgcontains(msg, 'strange helmet') then
  			sell(cid,2479,1,400)
            elseif msgcontains(msg, 'soldier helmet') then
  			sell(cid,2481,1,40)
             elseif msgcontains(msg, 'viking helmet') then
  			sell(cid,2473,1,66)
            elseif msgcontains(msg, 'brass helmet') then
  			sell(cid,2460,1,30)
			 		elseif msgcontains(msg, 'golden boots') then
  			sell(cid,2646,1,100000)
 		elseif msgcontains(msg, 'demon armor') then
  			sell(cid,2494,1,90000)
 		elseif msgcontains(msg, 'demon legs') then
  			sell(cid,2495,1,90000)
 		elseif msgcontains(msg, 'winged helmet') then
  			sell(cid,2474,1,300000)
 		elseif msgcontains(msg, 'great shield') then
  			sell(cid,2522,1,100000)
  		elseif msgcontains(msg, 'helmets') then
  			selfSay('I buy royal (20k), warrior (3k), crusader (3k), crown (2k), devil (2k), chain (35gp) and iron helmets (30gp), also mystic turbans (500gp).')
 
  		elseif msgcontains(msg, 'steel boots') and focus == cid then
  			sell(cid,2645,1,20000)
  		elseif msgcontains(msg, 'boh') or msgcontains(msg, 'boots of haste') and focus == cid then
  			sell(cid,2195,1,6000)
  		elseif msgcontains(msg, 'boots')  then
  			selfSay('I buy steel boots (20k) and boots of haste (6k).')
 
 		elseif msgcontains(msg, 'magic plate armor') or msgcontains(msg, 'mpa') then
  			sell(cid,2472,1,200000)
 		elseif msgcontains(msg, 'dragon scale mail') or msgcontains(msg, 'dsm') then
  			sell(cid,2492,1,90000)
 		elseif msgcontains(msg, 'golden armor') then
  			sell(cid,2466,1,20000)
			 		elseif msgcontains(msg, 'thunder hammer') then
  			sell(cid,2421,1,100000)
						 		elseif msgcontains(msg, 'magic longsword') then
  			sell(cid,2390,1,100000)
 		elseif msgcontains(msg, 'crown armor') then
  			sell(cid,2487,1,5000)
 		elseif msgcontains(msg, 'knight armor') then
  			sell(cid,2476,1,3000)
 		elseif msgcontains(msg, 'blue robe') then
  			sell(cid,2656,1,3000)
  		elseif msgcontains(msg, 'lady armor') then
  			sell(cid,2500,1,1000)
 		elseif msgcontains(msg, 'plate armor') then
  			sell(cid,2463,1,300)
 		elseif msgcontains(msg, 'brass armor') then
  			sell(cid,2465,1,200)
 		elseif msgcontains(msg, 'chain armor') then
  			sell(cid,2464,1,100)
 		elseif msgcontains(msg, 'scale armor') then
  			sell(cid,2483,1,75)
  		elseif msgcontains(msg, 'armors')  then
  			selfSay('I buy golden (10k), crown (5k), knight (3k), lady (1k), plate (300gp), brass (200gp) and chain armors (100gp), also mpa (100k), dsm (30k) and blue robes (3k).')
 
 		elseif msgcontains(msg, 'golden legs') then
  			sell(cid,2470,1,80000)
 		elseif msgcontains(msg, 'crown legs') then
  			sell(cid,2488,1,5000)
 		elseif msgcontains(msg, 'knight legs') then
  			sell(cid,2477,1,3000)
 		elseif msgcontains(msg, 'plate legs') then
  			sell(cid,2647,1,500)
  		elseif msgcontains(msg, 'brass legs') then
  			sell(cid,2478,1,100)
 		elseif msgcontains(msg, 'chain legs') then
  			sell(cid,2648,1,50)
  		elseif msgcontains(msg, 'legs')  then
  			selfSay('I buy golden (80k), crown (5k), knight (3k), plate (500gp), brass (100gp) and chain legs (50gp).')
 
  		elseif msgcontains(msg, 'mastermind shield') or msgcontains(msg, 'mms') then
  			sell(cid,2514,1,80000)
 		elseif msgcontains(msg, 'demon shield') then
  			sell(cid,2520,1,40000)
  		elseif msgcontains(msg, 'vampire shield') then
  			sell(cid,2534,1,4000)
 		elseif msgcontains(msg, 'medusa shield') then
  			sell(cid,2536,1,3500)
  		elseif msgcontains(msg, 'amazon shield') then
  			sell(cid,2537,1,3000)
 		elseif msgcontains(msg, 'crown shield') then
  			sell(cid,2519,1,2000)
  		elseif msgcontains(msg, 'tower shield') then
  			sell(cid,2528,1,2000)
  		elseif msgcontains(msg, 'dragon shield') then
  			sell(cid,2516,1,1500)
 		elseif msgcontains(msg, 'guardian shield') then
  			sell(cid,2515,1,1200)
 		elseif msgcontains(msg, 'beholder shield') then
  			sell(cid,2518,1,1000)
 		elseif msgcontains(msg, 'dwarven shield') then
  			sell(cid,2525,1,100)
             elseif msgcontains(msg, 'bone shield') then
  			sell(cid,2541,1,70)
             elseif msgcontains(msg, 'battle shield') then
  			sell(cid,2513,1,95)
             elseif msgcontains(msg, 'plate shield') then
  			sell(cid,2510,1,45)
            elseif msgcontains(msg, 'steel shield') then
  			sell(cid,2509,1,55)
 		elseif msgcontains(msg, 'shields')  then
  			selfSay('I buy demon (40k), vampire (4k), medusa (3.5k), amazon (3k), crown (2k), tower (2k), dragon (1.5k), guardian (1.2k), beholder (1k), and dwarven shields (100gp), also mms (80k)')
 
  		elseif msgcontains(msg, 'giant sword') then
  			sell(cid,2393,1,10000)
 		elseif msgcontains(msg, 'bright sword') then
  			sell(cid,2407,1,10000)
 		elseif msgcontains(msg, 'ice rapier')  then
  			sell(cid,2396,1,4000)
 		elseif msgcontains(msg, 'fire sword') then
  			sell(cid,2392,1,3000)
 		elseif msgcontains(msg, 'serpent sword')  then
  			sell(cid,2409,1,1500)
 		elseif msgcontains(msg, 'spike sword')  then
  			sell(cid,2383,1,800)
  		elseif msgcontains(msg, 'two-handed sword')  then
  			sell(cid,2377,1,400)
            elseif msgcontains(msg, 'broad sword')  then
  			sell(cid,2413,1,500)
            elseif msgcontains(msg, 'bone sword')  then
  			sell(cid,2450,1,30)
 		elseif msgcontains(msg, 'sabre') then
  			sell(cid,2385,1,25)
  		elseif msgcontains(msg, 'sword')  then
  			sell(cid,2376,1,25)
             elseif msgcontains(msg, 'heavy machete')  then
  			sell(cid,2442,1,55)
             elseif msgcontains(msg, 'longsword')  then
  			sell(cid,2397,1,45)
 		elseif msgcontains(msg, 'swords')  then
  			selfSay('I buy giant (10k), bright (6k), fire (3k) serpent (1.5k), spike (800gp) and twohanded swords (400gp), also ice rapiers (4k), sabres (25gp) and swords (25gp).')
 
  		elseif msgcontains(msg, 'dragon lance')  then
  			sell(cid,2414,1,10000)
  		elseif msgcontains(msg, 'fire axe')  then
  			sell(cid,2432,1,10000)
 		elseif msgcontains(msg, 'knight axe')  then
  			sell(cid,2430,1,2000)
 		elseif msgcontains(msg, 'double axe')  then
  			sell(cid,2387,1,200)
 		elseif msgcontains(msg, 'halberd')  then
  			sell(cid,2381,1,200)
 		elseif msgcontains(msg, 'battle axe')  then
  			sell(cid,2378,1,100)
  		elseif msgcontains(msg, 'hatchet')  then
  			sell(cid,2388,1,20)
             elseif msgcontains(msg, 'orc axe')  then
  			sell(cid,2428,1,25)
 		elseif msgcontains(msg, 'axes')  then
  			selfSay('I buy fire (10k), knight (2k), double (200gp) and battle axes (100gp), also dragon lances (10k), halberds (200gp) and hatchets (20gp).')
 
 		elseif msgcontains(msg, 'warhammer') then
  			sell(cid,2391,1,1500)
 		elseif msgcontains(msg, 'skull staff') then
  			sell(cid,2436,1,4000)
  		elseif msgcontains(msg, 'dragon hammer')  then
  			sell(cid,2434,1,2000)
  		elseif msgcontains(msg, 'clerical mace')  then
  			sell(cid,2423,1,200)
             elseif msgcontains(msg, 'daramanian mace')  then
  			sell(cid,2439,1,90)
  		elseif msgcontains(msg, 'battle hammer')  then
  			sell(cid,2417,1,60)
  		elseif msgcontains(msg, 'mace') then
  			sell(cid,2398,1,30)
             elseif msgcontains(msg, 'morning star') then
  			sell(cid,2394,1,80)
 		elseif msgcontains(msg, 'clubs')  then
  			selfSay('I buy warhammer (4k), dragon (2k) and battle hammers (60gp), also skull staffs (4k) and clerical maces (200gp).')
 
 
 		elseif msgcontains(msg, 'life ring') then
  			sell(cid,2168,1,200)
 		elseif msgcontains(msg, 'energy ring') then
  			sell(cid,2167,1,200)
  		elseif msgcontains(msg, 'might ring')  then
  			sell(cid,2164,1,400)
  		elseif msgcontains(msg, 'stealth ring')  then
  			sell(cid,2165,1,400)
             elseif msgcontains(msg, 'power ring')  then
  			sell(cid,2166,1,200)
  		elseif msgcontains(msg, 'time ring')  then
  			sell(cid,2169,1,300)
  		elseif msgcontains(msg, 'sword ring') then
  			sell(cid,2207,1,150)
             elseif msgcontains(msg, 'axe ring') then
  			sell(cid,2208,1,150)
             elseif msgcontains(msg, 'club ring') then
  			sell(cid,2209,1,150)
             elseif msgcontains(msg, 'dwarf ring') then
  			sell(cid,2213,1,100)
              		elseif msgcontains(msg, 'ring of healing') or msgcontains(msg, 'roh') then
  			sell(cid,2214,1,500)
                elseif msgcontains(msg, 'ring of the skies') or msgcontains(msg, 'rots') then
  			sell(cid,2123,1,600)
             
 		elseif msgcontains(msg, 'rings')  then
  			selfSay('I buy lifering (200gp), ringofhealing (500gp) , dwarfring (100gp), energyring (200gp), mightring (400gp), stealthring  (400gp), powerring (200gp), timering (300gp), And all skill rings for 150gp. .')
 
 		elseif msgcontains(msg, 'platinum amulet') then
  			sell(cid,2171,1,2000)
 		elseif msgcontains(msg, 'scarf') then
  			sell(cid,2661,1,200)
 		elseif msgcontains(msg, 'amulets')  then
  			selfSay('I buy platinum amulets (2k) and scarfs (200gp).')
 
 
 
 		elseif msgcontains(msg, 'bow') then
  			sell(cid,2456,1,130)
 		elseif msgcontains(msg, 'crossbow') then
  			sell(cid,2455,1,160)
 		elseif msgcontains(msg, 'bows')  then
 	elseif msgcontains(msg, 'bows')  then
  			selfSay('I buy bow (130gp) and crossbow (160gp).')
 
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