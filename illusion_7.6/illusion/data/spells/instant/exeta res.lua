area = {
 {1, 1, 1, 1, 1},
 {1, 1, 1, 1, 1},
 {1, 1, 0, 1, 1},
 {1, 1, 1, 1, 1},
 {1, 1, 1, 1, 1}
 }
 
 attackType = ATTACK_NONE
 animationEffect = NM_ANI_NONE
 needDirection = false
 areaEffect = NM_ME_MAGIC_BLOOD
 hitEffect = NM_ME_NONE
 damageEffect = NM_ME_MAGIC_ENERGIE
 animationColor = GREEN
 offensive = false
 drawblood = false
 
 SuperObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 
 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 ret = doAreaMagic(cid, centerpos, needDirection, areaEffect, area, SuperObject:ordered())
 if(ret) then
     challenge(cid)
 end
 
 return ret
 end 