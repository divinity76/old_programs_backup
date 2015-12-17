area = {
 {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
 {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
 {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
 {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
 {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
 {1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1},
 {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
 {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
 {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
 {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
 {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}
 }
 
 attackType = ATTACK_NONE
 needDirection = false
 areaEffect = NM_ME_MAGIC_ENERGIE
 animationEffect = NM_ANI_NONE
 
 hitEffect = NM_ME_NONE
 damageEffect = NM_ME_MAGIC_ENERGIE
 animationColor = GREEN
 offensive = false
 drawblood = false
 
 MassHealingObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 
 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 
 n = tonumber(var)
 	if n ~= nil then
 		MassHealingObject.minDmg = 0
 		MassHealingObject.maxDmg = 0
 	else
 		MassHealingObject.minDmg = ((level+maglv)*3) * 1.0
 if MassHealingObject.minDmg < 250 then
 	MassHealingObject.minDmg = 250
 end
 		MassHealingObject.maxDmg = ((level+maglv)*3) * 2.2
 if MassHealingObject.maxDmg < 250 then
 	MassHealingObject.maxDmg = 250
 end
 	end
 
 return doAreaMagic(cid, centerpos, needDirection, areaEffect, area, MassHealingObject:ordered())
 end
 
 
 
 
 
