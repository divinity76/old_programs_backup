--Warlock Explosion by Peonso
 
 area = {
 {1, 1, 1},
 {1, 1, 1},
 {1, 1, 1}
 }
 
 attackType = ATTACK_POISON
 needDirection = false
 areaEffect = NM_ME_EXPLOSION_AREA
 animationEffect = NM_ANI_FIRE
 
 hitEffect = NM_ME_EXPLOSION_DAMAGE
 damageEffect = NM_ME_DRAW_BLOOD
 animationColor = RED
 offensive = true
 drawblood = true
 
 wExploObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 
 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 
 wExploObject.minDmg = (level * 2 + maglv *3) * 0.35
 wExploObject.maxDmg = (level * 2 + maglv *3) * 0.65
 
 return doAreaMagic(cid, centerpos, needDirection, areaEffect, area, wExploObject:ordered())
 end
