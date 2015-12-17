attackType = ATTACK_DRUNKNESS
animationEffect = NM_ANI_NONE
hitEffect = NM_ME_MAGIC_BLOOD
damageEffect = NM_ME_MAGIC_BLOOD
animationColor = GREEN
offensive = false
drawblood = false
drunkObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
function onCast(cid, creaturePos, level, maglv, var)
centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
ret = doTargetMagic(cid, centerpos, drunkObject:ordered())
if(ret) then
Drunk(cid, centerpos, 30000, 1)
end
return ret
end