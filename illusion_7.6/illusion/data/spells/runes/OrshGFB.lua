-- Orshabaal Greatfireball by Peonso
 
 area = {
 {0, 0, 0, 0, 1, 0, 0, 0, 0},
 {0, 0, 1, 1, 1, 1, 1, 0, 0},
 {0, 1, 1, 1, 1, 1, 1, 1, 0},
 {0, 1, 1, 1, 1, 1, 1, 1, 0},
 {1, 1, 1, 1, 1, 1, 1, 1, 1},
 {0, 1, 1, 1, 1, 1, 1, 1, 0},
 {0, 1, 1, 1, 1, 1, 1, 1, 0},
 {0, 0, 1, 1, 1, 1, 1, 0, 0},
 {0, 0, 0, 0, 1, 0, 0, 0, 0}
 }
 
 attackType = ATTACK_FIRE
 needDirection = false
 areaEffect = NM_ME_FIRE_AREA
 animationEffect = NM_ANI_FIRE
 
 hitEffect = NM_ME_FIRE_AREA
 damageEffect = NM_ME_HITBY_FIRE
 animationColor = 193
 offensive = true
 drawblood = false
 
 OrshGFBObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 
 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 
 OrshGFBObject.minDmg = 550
 OrshGFBObject.maxDmg = 700
 
 return doAreaMagic(cid, centerpos, needDirection, areaEffect, area, OrshGFBObject:ordered())
 end
