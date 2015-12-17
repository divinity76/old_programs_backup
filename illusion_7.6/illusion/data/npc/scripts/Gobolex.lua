-- the id of the creature we are attacking, following, etc.
 
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
   selfSay('Hey there ' .. creatureGetName(cid) .. '! I can teach you the basics of mining. I also buy and sell things to do with mining. Just say mining manual or offer.')
   focus = cid
   talk_start = os.clock()
  end 
 if string.find(msg, '(%a*)hi(%a*)') and (focus ~= cid) and getDistanceToCreature(cid) < 3 then
   selfSay('Sorry, ' .. creatureGetName(cid) .. '! I talk to you in a minute.')
  end
 if msgcontains(msg, 'mining manual') and focus == cid then
   selfSay('I can tell you about requirements, basics, obstacles, and also advantages!')
   talk_start = os.clock()
  end
 
 if msgcontains(msg, 'requirements') and focus == cid then
   selfSay('The requirements for mining that are set are: a pick, level 20, and club skill 20. Thats all!')
  talk_start = os.clock()
  end 
 if msgcontains(msg, 'basics') and focus == cid then
   selfSay('When mining all you basically do is look for gold and treasure in the rocks.')
  selfSay('You can recieve different amount of gold and also different treasure.')
  talk_start = os.clock()
  end 
 if msgcontains(msg, 'obstacles') and focus == cid then
   selfSay('Mining has its obstacles: your pick can break, an earthquake can happen, monsters come from the rocks, you get exhausted, and other things. ')
  talk_start = os.clock()
  end
 if msgcontains(msg, 'advantages') and focus == cid then
   selfSay('What are the advantages? Well you get rich! Also your club skill will go up when you mine!')
  talk_start = os.clock()
  end
 
 if msgcontains(msg, 'offer') and focus == cid then
   selfSay('I sell picks(100k), shovels(50gp), and backpacks(10gp). I buy diamonds(200gp), rubies(100gp), and gold nuggets(250gp).')
  talk_start = os.clock()
  end
  if msgcontains(msg, 'pick') and focus == cid then
   buy(cid,2553,1,1000000)
   talk_start = os.clock()
  end
 
 if msgcontains(msg, 'shovel') and focus == cid then
   buy(cid,2554,1,50)
   talk_start = os.clock()
  end
 
 if msgcontains(msg, 'backpack') and focus == cid then
   buy(cid,2003,1,10)
   talk_start = os.clock()
  end
 
 if msgcontains(msg, 'diamond') and focus == cid then
   sell(cid,2145,1,200)
   talk_start = os.clock()
  end
 
 if msgcontains(msg, 'ruby') and focus == cid then
   sell(cid,2147,1,100)
   talk_start = os.clock()
  end
 
 if msgcontains(msg, 'gold nugget') and focus == cid then
   sell(cid,2157,1,3)
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
 end
 