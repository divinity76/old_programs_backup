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
 UltimateHealingObject.minDmg = (level * 2 + maglv * 3) * 2.2
 if UltimateHealingObject.minDmg < 250 then
 	UltimateHealingObject.minDmg = 250
		RemoveParalyze(cid, creaturePos)
	speed = getSpeed(cid, creaturePos)
    	time = 20  --in seconds
    	addspeed = speed*0
    	
    	changeSpeed(cid, addspeed, time)
 end

 UltimateHealingObject.maxDmg = (level * 2 + maglv * 3) * 3
 if UltimateHealingObject.maxDmg < 250 then
 	UltimateHealingObject.maxDmg = 250
		RemoveParalyze(cid)
	speed = getSpeed(cid)
    	time = 20  --in seconds
    	addspeed = speed*0
    	
    	changeSpeed(cid, addspeed, time)
 end
 
 return doTargetMagic(cid, centerpos, paraylzeObject:ordered())

 end