area = {    			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    				{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    				{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    				{0, 0, 0, 0, 0, 0, 2, 2, 0, 3, 3, 0, 0, 0, 0, 0, 0},
    				{0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0},
    				{0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0},
    				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}    }
 
    attackType = ATTACK_PHYSICAL
    needDirection = true
    areaEffect = NM_ME_SOUND_GREEN
    animationEffect = NM_ANI_SUDDENDEATH
 
    hitEffect = NM_ME_MORT_AREA
    damageEffect = NM_ME_YELLOW_RINGS
    animationColor = PURPLE
    offensive = true
    drawblood = true
 
    GreatEnergyBeamObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 
    function onCast(cid, creaturePos, level, maglv, var)
    centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
    GreatEnergyBeamObject.minDmg = (level * 1 + maglv *1) * 1.0
    GreatEnergyBeamObject.maxDmg = (level * 1 + maglv *1) * 1.4
 
    return doAreaMagic(cid, centerpos, needDirection, areaEffect, area, GreatEnergyBeamObject:ordered())
    end
 
