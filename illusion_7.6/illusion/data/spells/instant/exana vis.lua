attackType = ATTACK_PHYSICAL
    animationEffect = NM_ANI_NONE
    
    hitEffect = NM_ME_LOOSE_ENERGY
    damageEffect = NM_ME_LOOSE_ENERGY
    animationColor = BLUE
    offensive = true
    drawblood = false
    
    HealFriendObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
    
    function onCast(cid, creaturePos, level, maglv, var)
    centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z} targetpos = getPosition(var)
    HealFriendObject.minDmg = (level * 2 + maglv * 3) / 2
    HealFriendObject.maxDmg = (level * 2 + maglv * 3) / 2 + 10
    
    if targetpos.x ~= nil and targetpos.z ~= nil and targetpos.y ~= nil then
    	if math.abs(targetpos.x - centerpos.x) < 18 and math.abs(targetpos.y - centerpos.y) < 14 and targetpos.z == centerpos.z then
    		return doTargetMagic(cid, targetpos, HealFriendObject:ordered())
    	end
    end
    
    return false
    end  
    
