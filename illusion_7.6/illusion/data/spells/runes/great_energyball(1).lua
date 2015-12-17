area = {
 {0, 0, 1, 1, 1, 0, 0},
 {0, 1, 1, 1, 1, 1, 0},
 {1, 1, 1, 1, 1, 1, 1},
 {1, 1, 1, 1, 1, 1, 1},
 {1, 1, 1, 1, 1, 1, 1},
 {0, 1, 1, 1, 1, 1, 0},
 {0, 0, 1, 1, 1, 0, 0}
 }
 
 attackType = ATTACK_ENERGY
 needDirection = false
 areaEffect = NM_ME_ENERGY_AREA
 animationEffect = NM_ANI_ENERGY
 
 hitEffect = NM_ME_ENERGY_AREA
 damageEffect = NM_ME_DRAW_BLOOD
 animationColor = LIGHT_BLUE
 offensive = true
 drawblood = false
 
 GreatEnergyballObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 
 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 
 GreatEnergyballObject.minDmg = (level * 2 + maglv *3) * 0.40
 GreatEnergyballObject.maxDmg = (level * 2 + maglv *3) * 0.75
 
 return doAreaMagic(cid, centerpos, needDirection, areaEffect, area, GreatEnergyballObject:ordered())
 end
