--Lich Beam by Peonso
 area = {
 {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
 {2, 2, 2, 2, 2, 2, 0, 3, 3, 3, 3, 3, 3},
 {0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0}
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
 
 LichBeamObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 
 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 LichBeamObject.minDmg = 100
 LichBeamObject.maxDmg = 140
 
 return doAreaMagic(cid, centerpos, needDirection, areaEffect, area, LichBeamObject:ordered())
 end  
 
