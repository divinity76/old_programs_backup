attackType = ATTACK_NONE
 animationEffect = NM_ANI_NONE
 
 hitEffect = NM_ME_NONE
 damageEffect = NM_ME_MAGIC_ENERGIE
 animationColor = GREEN
 offensive = false
 drawblood = false
 
 MildHealingObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 
 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 n = tonumber(var)   -- try to convert it to a number
 	if n ~= nil then
 		MildHealingObject.minDmg = 0
 		MildHealingObject.maxDmg = 0
 	else
 		MildHealingObject.minDmg = (level * 2 + maglv * 3) * 0.99
 if MildHealingObject.minDmg < 35 then
 	MildHealingObject.minDmg = 35
 end
 		MildHealingObject.maxDmg = (level * 2 + maglv * 3) * 1.67
 	if MildHealingObject.maxDmg < 35 then
 	MildHealingObject.maxDmg = 35
 end
 end
 return doTargetMagic(cid, centerpos, MildHealingObject:ordered())
 end