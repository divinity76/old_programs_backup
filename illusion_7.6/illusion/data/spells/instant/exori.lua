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
    areaEffect = NM_ME_HIT_AREA
    animationEffect = NM_ANI_NONE
 
    hitEffect = NM_ME_HIT_AREA
    damageEffect = NM_ME_HIT_AREA
    animationColor = RED
    offensive = true
    drawblood = true
 
    UltimateExplosionObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 
    function onCast(cid, creaturePos, level, maglv, var)
    centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
    n = tonumber(var)   -- try to convert it to a number
    if n ~= nil then
    	-- bugged
    	-- ultimateExplosionObject.minDmg = var+0
    	-- UltimateExplosionObject.maxDmg = var+0
 
    	UltimateExplosionObject.minDmg = 0
    	UltimateExplosionObject.maxDmg = 0
    else
    	-- UltimateExplosionObject.minDmg = (level * 3 + maglv * 5) * 2.3 - 30
    	-- UltimateExplosionObject.maxDmg = (level * 3 + maglv * 5) * 3.0
 	UltimateExplosionObject.minDmg = (level * 2.5 + maglv * 3.5) * 1.2
 	UltimateExplosionObject.maxDmg = (level * 2.5 + maglv * 3.5) * 2.3
    end
 
    return doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
    end
