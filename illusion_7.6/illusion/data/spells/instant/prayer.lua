attackType = ATTACK_NONE
 animationEffect = NM_ANI_NONE
 

hitEffect = NM_ME_POISEN
 damageEffect = NM_ME_POISEN_RINGS
 animationColor = GREEN
 offensive = true
 needDirection = false
 drawblood = false
 minDmg = 0
 maxDmg = 0
 
 UltimateHealing = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, minDmg, maxDmg)
 SubUltimateHealing1 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing2 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing3 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing4 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing5 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing6 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing7 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing8 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing9 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing10 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing11 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing12 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing13 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing14 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing15 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing16 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing17 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing18 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing19 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing20 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing21 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing22 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing23 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing24 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing25 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)
 SubUltimateHealing26 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 0, 0)

 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 n = tonumber(var)   -- try to convert it to a number
 	if n ~= nil then
 		UltimateHealingObject.minDmg = 0
 		UltimateHealingObject.maxDmg = 0
 	else
 		UltimateHealingObject.minDmg = (level * 2 + maglv *3) * 1.3 - 30
 if UltimateHealingObject.maxDmg < 250 then
 	UltimateHealingObject.maxDmg = 250
 end
 		UltimateHealingObject.maxDmg = (level * 2 + maglv *3) * 1.7
 if UltimateHealingObject.maxDmg < 250 then
 	UltimateHealingObject.maxDmg = 250
 end
 	end
 
 return doTargetMagic(cid, centerpos, UltimateHealingObject:ordered())
 end

