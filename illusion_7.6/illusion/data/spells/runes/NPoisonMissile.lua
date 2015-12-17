--Necro Poison Atack by Peonso
 
 attackType = ATTACK_POISON
 animationEffect = 14
 
 hitEffect = NM_ME_POISEN
 damageEffect = NM_ME_POISEN_RINGS
 animationColor = GREEN
 offensive = true
 drawblood = true
 
 NPoisonMissileObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 
 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 
 NPoisonMissileObject.minDmg = 36
 NPoisonMissileObject.maxDmg = 93
 
 return doTargetMagic(cid, centerpos, NPoisonMissileObject:ordered())
 end