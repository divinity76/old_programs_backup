area = {
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
   {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
   {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
   {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
   {0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0},
   {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
   {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
   {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
   {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   }
   
   attackType = ATTACK_PHYSICAL
   needDirection = false
   areaEffect = NM_ME_PUFF
   animationEffect = NM_ANI_NONE
   
   hitEffect = NM_ME_EXPLOSION_DAMAGE
   damageEffect = NM_ME_PUFF
   animationColor = RED
   offensive = true
   drawblood = false
   
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
   	UltimateExplosionObject.minDmg = (level + maglv * 3) * 1.7 - 30
   	UltimateExplosionObject.maxDmg = (level + maglv * 3) * 2.3 	
   end 
   
   return doAreaMagic(cid, centerpos, needDirection, areaEffect, area, UltimateExplosionObject:ordered())
   end  
