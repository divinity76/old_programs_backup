attackType = ATTACK_ENERGY
animationEffect = NM_ANI_NONE

hitEffect = NM_ME_MAGIC_ENERGIE
damageEffect = NM_ME_MAGIC_ENERGIE
animationColor = RED
offensive = false
drawblood = false
minDmg = 200
maxDmg = 200
subDelayTick = 2000
subDamageCount = 5

WhiteWind = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, minDmg, maxDmg)
SubWhiteWind = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 500, 500)

function onCast(cid, creaturePos, level, maglv, var)
centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}

return doTargetExMagic(cid, centerpos, WhiteWindrdered(),
subDelayTick, subDamageCount, SubWhiteWindrdered())

end
