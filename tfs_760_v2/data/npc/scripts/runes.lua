local keywordHandler = KeywordHandler:new()
local npcHandler = NpcHandler:new(keywordHandler)
NpcSystem.parseParameters(npcHandler)
local talkState = {}

function onCreatureAppear(cid)				npcHandler:onCreatureAppear(cid)			end
function onCreatureDisappear(cid) 			npcHandler:onCreatureDisappear(cid)			end
function onCreatureSay(cid, type, msg)			npcHandler:onCreatureSay(cid, type, msg)		end
function onThink()					npcHandler:onThink()					end

local shopModule = ShopModule:new()
npcHandler:addModule(shopModule)

shopModule:addBuyableItem({'spellbook'}, 2175, 150, 'spellbook')
shopModule:addBuyableItem({'magic lightwand'}, 2163, 400, 'magic lightwand')

shopModule:addBuyableItem({'instense healing'}, 2265, 95, 1, 'intense healing rune')
shopModule:addBuyableItem({'ultimate healing'}, 2273, 175, 1, 'ultimate healing rune')
shopModule:addBuyableItem({'magic wall'}, 2293, 350, 3, 'magic wall rune')
shopModule:addBuyableItem({'destroy field'}, 2261, 45, 3, 'destroy field rune')
shopModule:addBuyableItem({'light magic missile'}, 2287, 40, 10, 'light magic missile rune')
shopModule:addBuyableItem({'heavy magic missile'}, 2311, 120, 10, 'heavy magic missile rune')
shopModule:addBuyableItem({'great fireball'}, 2304, 180, 4, 'great fireball rune')
shopModule:addBuyableItem({'explosion'}, 2313, 250, 6, 'explosion rune')
shopModule:addBuyableItem({'sudden death'}, 2268, 350, 3, 'sudden death rune')
shopModule:addBuyableItem({'death arrow'}, 2263, 300, 3, 'death arrow rune')
shopModule:addBuyableItem({'paralyze'}, 2278, 700, 1, 'paralyze rune')
shopModule:addBuyableItem({'animate dead'}, 2316, 375, 1, 'animate dead rune')
shopModule:addBuyableItem({'convince creature'}, 2290, 80, 1, 'convince creature rune')
shopModule:addBuyableItem({'chameleon'}, 2291, 210, 1, 'chameleon rune')
shopModule:addBuyableItem({'desintegrate'}, 2310, 80,  3, 'desintegreate rune')

shopModule:addBuyableItem({'wand of vortex', 'vortex'}, 2190, 500, 'wand of vortex')
shopModule:addBuyableItem({'wand of dragonbreath', 'dragonbreath'}, 2191, 1000, 'wand of dragonbreath')
shopModule:addBuyableItem({'wand of plague', 'plague'}, 2188, 5000, 'wand of plague')
shopModule:addBuyableItem({'wand of cosmic energy', 'cosmic energy'}, 2189, 10000, 'wand of cosmic energy')
shopModule:addBuyableItem({'wand of inferno', 'inferno'}, 2187, 15000, 'wand of inferno')

shopModule:addBuyableItem({'snakebite rod', 'snakebite'}, 2182, 500, 'snakebite rod')
shopModule:addBuyableItem({'moonlight rod', 'moonlight'}, 2186, 1000, 'moonlight rod')
shopModule:addBuyableItem({'volcanic rod', 'volcanic'}, 2185, 5000, 'volcanic rod')
shopModule:addBuyableItem({'quagmire rod', 'quagmire'}, 2181, 10000, 'quagmire rod')
shopModule:addBuyableItem({'tempest rod', 'tempest'}, 2183, 15000, 'tempest rod')

shopModule:addSellableItem({'wand of vortex', 'vortex'}, 2190, 250, 'wand of vortex')
shopModule:addSellableItem({'wand of dragonbreath', 'dragonbreath'}, 2191, 500, 'wand of dragonbreath')
shopModule:addSellableItem({'wand of decay', 'decay'}, 2188, 2500, 'wand of decay')
shopModule:addSellableItem({'wand of cosmic energy', 'cosmic energy'}, 2189, 5000, 'wand of cosmic energy')
shopModule:addSellableItem({'wand of inferno', 'inferno'},2187, 7500, 'wand of inferno')

shopModule:addSellableItem({'snakebite rod', 'snakebite'}, 2182, 250,'snakebite rod')
shopModule:addSellableItem({'moonlight rod', 'moonlight'}, 2186, 500,   'moonlight rod')
shopModule:addSellableItem({'volcanic rod', 'volcanic'}, 2185, 2500, 'volcanic rod')
shopModule:addSellableItem({'northwind rod', 'northwind'}, 8911, 3750, 'northwind rod')
shopModule:addSellableItem({'quagmire rod', 'quagmire'}, 2181, 5000, 'quagmire rod')
shopModule:addSellableItem({'tempest rod', 'tempest'}, 2183, 7500, 'tempest rod')

npcHandler:addModule(FocusModule:new())
