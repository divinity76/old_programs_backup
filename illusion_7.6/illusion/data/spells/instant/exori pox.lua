area = {
 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 2, 0, 3, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
 }
 attackType = ATTACK_POISON
 needDirection = true
 areaEffect = 20
 animationEffect = NM_ANI_NONE
 hitEffect = NM_ME_POISEN_RINGS
 damageEffect =  20
 animationColor = GREEN
 offensive = true
 drawblood = false
 GreatEnergyBeamObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 GreatEnergyBeamObject.minDmg = (level * 1 + maglv *1) * 0.8
 GreatEnergyBeamObject.maxDmg = (level * 1 + maglv *1)
 return doAreaMagic(cid, centerpos, needDirection, areaEffect, area, GreatEnergyBeamObject:ordered())
 end