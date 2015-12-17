-- Mining V2.6
-- Please understand this is more of a bug fix release
-- Coded by Roman, some credits to twiz for giving an example of his V1.0
function onUse(cid, item, frompos, item2, topos)
Level = getPlayerLevel(cid)
ClubSkill = getPlayerSkill(cid,1)
if item2.actionid == 5000 and         -- set the action id of rocks
Level >= 20   -- set the level requirement to be able to mine
and
ClubSkill >= 20  -- set the club skill requirement to be able to mine
then 
rand = math.random(1,2000)
if rand < 20 then
doPlayerSendTextMessage(cid,22,"A fierce Giant Worm came out of the pile of rocks!")
doSummonCreature("Giant Worm", topos)          
elseif rand == 1300 then
doPlayerSendTextMessage(cid,22,"You have found a golden nugget.")              
doPlayerAddItem(cid,2157,1)                
elseif rand > 1995 then
doPlayerSendTextMessage(cid,22,"You have found a diamond.")
doPlayerAddItem(cid,2145,1)
elseif rand >500 and rand <700 then
doPlayerSendTextMessage(cid,22,"OMG A CRAFTING ORE!")
doPlayerAddItem(cid,2225,7)
elseif rand >700 and rand <1000 then
doPlayerAddItem(cid,2160,5)
elseif rand >1000 and rand <1300 then
doPlayerAddItem(cid,2160,4)
elseif rand >300 and rand <600 then
doPlayerAddItem(cid,2160,5)
elseif rand >1500 and rand <1800 then
doPlayerAddItem(cid,2160,7)
elseif rand >1100 and rand <1400 then
doPlayerAddItem(cid,2160,10)
elseif rand >800 and rand <1100 then
doPlayerAddHealth(cid,-100)
doPlayerSendTextMessage(cid,22,"You lost hitpoints due to exhaustion of mining.")
elseif rand >500 and rand <800 then
doPlayerSendTextMessage(cid,22,"You have found a ruby.")
doPlayerAddItem(cid,2147,1)
elseif rand >550 and rand <800 then
doPlayerSendTextMessage(cid,22,"A fierce Giant Worm came out of the pile of rocks!")
doSummonCreature("Giant Worm", topos)
elseif rand == 40 then
doPlayerSendTextMessage(cid,22,"A fierce dwarf guard came out of the pile of rocks!")
doSummonCreature("Dwarf Guard", topos)
elseif rand == 20 then
doPlayerSendTextMessage(cid,22,"You hit the rock to hard making a hole, A orshabaal comes up striaght from hell!")
doSummonCreature("Orshabaal", topos)
elseif rand >2000 then
doPlayerSendTextMessage(cid,22,"Earthquake!")
doPlayerAddHealth(cid,-100)
elseif rand == 100 and rand <140 then
doPlayerSendTextMessage(cid,22,"A weak troll has appeared from the pile of rocks!")
doSummonCreature("Troll", topos)
elseif rand == 200 then
doPlayerSendTextMessage(cid,22,"A dwarf soldier came out of the pile of rocks!")
doSummonCreature("Dwarf Soldier", topos)
elseif rand >= 200 and rand <255 then
    doRemoveItem(item.uid,1)
  doSendMagicEffect(topos,2)
doPlayerSendTextMessage(cid,22,"Your pick has been heavily damaged and broke...")
end
doSendMagicEffect(topos,3)
doPlayerAddSkillTry(cid,1,100)
doPlayerSay(cid,"~Clank",1)
else
 doPlayerSendCancel(cid,"You either are not mining on the specific rock or you do not have the required level(20) or club skill(20) to mine.")
end
return 1
end