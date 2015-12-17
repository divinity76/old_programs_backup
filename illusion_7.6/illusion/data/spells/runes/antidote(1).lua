attackType = ATTACK_NONE
 animationEffect = NM_ANI_NONE
 hitEffect = NM_ME_MAGIC_BLOOD
 damageEffect = NM_ME_MAGIC_BLOOD
 animationColor = GREEN
 offensive = false
 drawblood = false
 paraylzeObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 function onCast(cid, creaturePos, level, maglv, var)
 centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 ret = doTargetMagic(cid, centerpos, paraylzeObject:ordered())
 if(ret) then
 RemoveCondition(cid, 5)
 end
 return ret
 end