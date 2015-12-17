area = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    }
 
    attackType = ATTACK_PHYSICAL
    needDirection = false
    areaEffect = NM_ME_PUFF
    animationEffect = NM_ANI_NONE
 
    hitEffect = NM_ME_SOUND_RED
    damageEffect = 255
    animationColor = GREEN
    offensive = true
    drawblood = true
 
    UltimateExplosionObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 
    function onCast(cid, creaturePos, level, maglv, var)
    centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
    n = tonumber(var)   -- try to convert it to a number
    if n ~= nil then 
    	UltimateExplosionObject.minDmg = 0
    	UltimateExplosionObject.maxDmg = 0
    else
 	UltimateExplosionObject.minDmg = (level * 1.5 + maglv * 1.5) * 1.2
 	UltimateExplosionObject.maxDmg = (level * 1.5 + maglv * 1.5) * 2.3
    end
 
    return doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
    end
