attackType = ATTACK_NONE
 animationEffect = NM_ANI_NONE
 hitEffect = NM_ME_NONE
 damageEffect = NM_ME_NONE
 animationColor = GREEN
 offensive = false
 drawblood = false
 LightMagicMissileObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, 
 drawblood, 0, 0)
 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 ret = doTargetMagic(cid, centerpos, LightMagicMissileObject:ordered())
 if(ret) then
 monsterlook = 68
 end
 time = 60
 changeOutfit(cid, time, monsterlook)
 return ret
 end