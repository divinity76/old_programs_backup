-- Mass bolt attack by Lukky513 --
area = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    }
 
    attackType = ATTACK_NONE
    needDirection = false
    areaEffect = NM_ME_HIT_AREA
    animationEffect = NM_ANI_BOLT
 
    hitEffect = NM_ME_HIT_AREA
    damageEffect = NM_ME_HIT_AREA
    animationColor = RED
    offensive = true
    drawblood = true
 
    UltimateExplosionObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
 
    function onCast(cid, creaturePos, level, maglv, var)
    centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
 	UltimateExplosionObject.minDmg = (level * 2.5 + maglv * 2) * 0.8
 	UltimateExplosionObject.maxDmg = (level * 2.5 + maglv * 2) * 1.8

 centerpos.x = centerpos.x + 2
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
 centerpos.y = centerpos.y - 1
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
 centerpos.y = centerpos.y - 1
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
 centerpos.x = centerpos.x - 1
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
 centerpos.x = centerpos.x - 1
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
 centerpos.x = centerpos.x - 1
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
 centerpos.x = centerpos.x - 1
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
 centerpos.y = centerpos.y + 1
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
 centerpos.y = centerpos.y + 1
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
 centerpos.y = centerpos.y + 1
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
 centerpos.y = centerpos.y + 1
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
 centerpos.x = centerpos.x + 1
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
 centerpos.x = centerpos.x + 1
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
 centerpos.x = centerpos.x + 1
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
 centerpos.x = centerpos.x + 1
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
 centerpos.y = centerpos.y - 1
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
 centerpos.y = centerpos.y - 1
 doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())

   end