area = {
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
   {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
   {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
   {0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0},
   {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
   {0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0},
   {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
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

   ExoriObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)

   function onCast(cid, creaturePos, level, maglv, var)
   centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}
   n = tonumber(var)   -- try to convert it to a number
   if n ~= nil then
   	-- bugged
   	-- ExoriObject.minDmg = var+0
   	-- ExoriObject.maxDmg = var+0

   	ExoriObject.minDmg = 0
   	ExoriObject.maxDmg = 0
   else
   	-- ExoriObject.minDmg = (level * 2 + maglv * 3) * 2.5 - 30
   	-- ExoriObject.maxDmg = (level * 2 + maglv * 3) * 3.2
	ExoriObject.minDmg = (level * 1.5 + maglv * 1.5) * 1.4
	ExoriObject.maxDmg = (level * 1.5 + maglv * 1.5) * 2.5
   end

   return doAreaMagic(cid, centerpos, needDirection, areaEffect, area, ExoriObject:ordered())
   end
