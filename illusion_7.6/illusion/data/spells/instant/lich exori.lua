--Lich Special Exori by Peonso
 area = {
 {1, 1, 1},
 {1, 0, 1},
 {1, 1, 1}
 }
 
 attackType = ATTACK_PHYSICAL
 needDirection = false
 areaEffect = NM_ME_MAGIC_BLOOD
 animationEffect = NM_ANI_NONE
 
 hitEffect = 8
 damageEffect = NM_ME_DRAW_BLOOD
 animationColor = GREEN
 offensive = true
 drawblood = true
 
 LichExoriObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 
 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 LichExoriObject.minDmg = 120
 LichExoriObject.maxDmg = 160
 
 return doAreaMagic(cid, centerpos, needDirection, areaEffect, area, LichExoriObject:ordered())
 end  
