--Fireatack by Peonso
 
 attackType = ATTACK_FIRE
 animationEffect = NM_ANI_FIRE
 
 hitEffect = NM_ME_HITBY_FIRE
 damageEffect = NM_ME_HITBY_FIRE
 animationColor = 193
 offensive = true
 drawblood = true
 
 FireAtackObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, minDmg, maxDmg)
 
 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 
 FireAtackObject.minDmg = (level * 2 + maglv *3) * 0.1
 FireAtackObject.maxDmg = (level * 2 + maglv *3) * 0.2
 
 return doTargetMagic(cid, centerpos, FireAtackObject:ordered())
 end