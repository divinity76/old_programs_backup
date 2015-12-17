area = {
{0, 0, 0, 0, 0},
{0, 1, 1, 1, 0},
{0, 1, 0, 1, 0},
{0, 1, 1, 1, 0},
{0, 0, 0, 0, 0}}
attackType = ATTACK_ENERGY
needDirection = false
areaEffect = NM_ME_NONE
animationEffect = NM_ANI_ENERGY

hitEffect = NM_ME_ENERGY_DAMAGE
damageEffect = NM_ME_ENERGY_DAMAGE
animationColor = LIGHT_BLUE
offensive = true
drawblood = false
minDmg = 0
maxDmg = 0

MagicTrapObject = MagicDamageObject(attackType, animationEffect, hitEffect, damageEffect, animationColor, offensive, drawblood, 0, 0)
SubMagicTrapObject1 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, minDmg, maxDmg)
SubMagicTrapObject2 = MagicDamageObject(attackType, NM_ANI_NONE, NM_ME_NONE, damageEffect, animationColor, offensive, drawblood, 25, 25)

function onCast(cid, creaturePos, level, maglv, var)
centerpos = {x=creaturePos.x, y=creaturePos.y, z=creaturePos.z}

return doAreaGroundMagic(cid, centerpos, needDirection, areaEffect, area, MagicTrapObject:ordered(),
	0, 1, SubMagicTrapObject1:ordered(),
	5000, 1, SubMagicTrapObject2:ordered(),
	2, 10000, 1499, 1)

end
