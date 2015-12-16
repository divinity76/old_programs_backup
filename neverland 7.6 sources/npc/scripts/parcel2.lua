focus = 0
 talk_start = 0
 target = 0
 following = false
 attacking = false
 ox = 115
  oy = 557
  oz = 8
  max = 7
 
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
 
 if (msgcontains(msg, 'hi') and focus == 0) and getDistanceToCreature(cid) < 4 then
 selfSay('Hello ' .. creatureGetName(cid) .. '! I\'m one of Marine Sisters, we selling parcels and letters in all cities.')
 focus = cid
 talk_start = os.clock()
 
 elseif msgcontains(msg, 'hi') and (focus ~= cid) and getDistanceToCreature(cid) < 4 then
 selfSay('Sorry, ' .. creatureGetName(cid) .. '! I talk to you in a minute.')
 
 elseif focus == cid then
 talk_start = os.clock()
 
 if msgcontains(msg, 'parcels') then
 selfSay('I want only 15gps on it.')
 
 elseif msgcontains(msg, 'letters') then
 selfSay('I want only 10gps on it.')
 
 elseif msgcontains(msg, 'letter') then
 selfSay('Do you want to buy a letter for 10 gold pieces?')
 talk_state = 1
 
 elseif msgcontains(msg, 'parcel') then
 selfSay('Do you want to buy a parcel for 15 gold pieces?')
 talk_state = 2
 
 elseif msgcontains(msg, 'label') then
 selfSay('Do you want to buy a label for 5 gold pieces?')
 talk_state = 3
 
 elseif talk_state == 1 then
 if msgcontains(msg, 'yes') then
 if pay(cid,10) then
 buy(cid,2597,1,10)
 selfSay('Dont forget to write the name and the address of the receiver on the label. The label has to be in the parcel beforeyou put the parcel in a mailbox.')
 else
 selfSay('Sorry, you do not have enough money.')
 end
 end
 talk_state = 0
 
 elseif talk_state == 2 then
 if msgcontains(msg, 'yes') then
 if pay(cid,15) then
 buy(cid,2595,1,15)
 buy(cid,2599,1,0)
 selfSay('Dont forget to write the name and the address of the receiver on the label. The label has to be in the parcel beforeyou put the parcel in a mailbox')
 else
 selfSay('Sorry, you do not have enough money.')
 end
 end
 talk_state = 0
 
 elseif talk_state == 3 then
 if msgcontains(msg, 'yes') then
 if pay(cid,5) then
 buy(cid,2599,1,0)
 selfSay('Here you are.')
 else
 selfSay('Sorry, you do not have enough money.')
 end
 end
 talk_state = 0
 
 elseif msgcontains(msg, 'bye') and getDistanceToCreature(cid) < 4 then
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
 
