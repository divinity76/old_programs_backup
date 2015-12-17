--BB Special Exori by Peonso
 area = {
 {1, 1, 1},
 {1, 0, 1},
 {1, 1, 1}
 }
 
 attackType = ATTACK_ENERGY
 needDirection = true
 areaEffect = NM_ME_MAGIC_BLOOD
 animationEffect = NM_ANI_NONE
 
 hitEffect = NM_ME_HIT_AREA
 damageEffect = NM_ME_DRAW_BLOOD
 animationColor = RED
 offensive = true
 drawblood = true
 
 BbExoriObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 
 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 BbExoriObject.minDmg = 25
 BbExoriObject.maxDmg = 70
 
 return doAreaMagic(cid, centerpos, needDirection, areaEffect, area, BbExoriObject:ordered())
 end  
