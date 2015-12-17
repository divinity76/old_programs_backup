--Yeti Power by Peonso
 area = {
 {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
 {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
 {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
 {2, 2, 2, 0, 0, 1, 0, 0, 3, 3, 3},
 {2, 2, 2, 2, 2, 0, 3, 3, 3, 3, 3},
 {2, 2, 2, 0, 0, 4, 0, 0, 3, 3, 3},
 {0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0},
 {0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0},
 {0, 0, 0, 0, 4, 4, 4, 0, 0, 0, 0}
 }
 
 attackType = ATTACK_PHYSICAL
 needDirection = true
 areaEffect = NM_ME_MAGIC_BLOOD
 animationEffect = NM_ANI_NONE
 
 hitEffect = NM_ME_HIT_AREA
 damageEffect = NM_ME_DRAW_BLOOD
 animationColor = RED
 offensive = true
 drawblood = true
 
 YetiPowerObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 
 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 YetiPowerObject.minDmg = 150
 YetiPowerObject.maxDmg = 200
 
 return doAreaMagic(cid, centerpos, needDirection, areaEffect, area, YetiPowerObject:ordered())
 end  
 
