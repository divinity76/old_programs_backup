--BB Poison Exori by Peonso
 area = {
 {1, 1, 1},
 {1, 0, 1},
 {1, 1, 1}
 }
 
 attackType = ATTACK_POISON
 needDirection = false
 areaEffect = 20
 animationEffect = NM_ANI_NONE
 
 hitEffect = NM_ME_POISEN
 damageEffect = NM_ME_POISEN_RINGS
 animationColor = GREEN
 offensive = true
 drawblood = true
 minDmg = 40
 maxDmg = 110
 
 LichPoBeamObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, minDmg, maxDmg)
 SubLichPoBeamObject1 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 5, 5)
 SubLichPoBeamObject2 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 4, 4)
 SubLichPoBeamObject3 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 3, 3)
 SubLichPoBeamObject4 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 2, 2)
 SubLichPoBeamObject5 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 1, 1)
 
 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 
 return doAreaExMagic(cid, centerpos, needDirection, areaEffect, area, LichPoBeamObject:ordered(),
 	2000, 2, SubLichPoBeamObject1:ordered(),
 	2000, 3, SubLichPoBeamObject2:ordered(),
 	2000, 4, SubLichPoBeamObject3:ordered(),
 	2000, 5, SubLichPoBeamObject4:ordered(),
 	2000, 6, SubLichPoBeamObject5:ordered(),
 5)
 end  
