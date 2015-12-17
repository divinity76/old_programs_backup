-- NPC Broadcaster by Alreth, v1.37, 2006-07-25 18:15 CET
-- Idea by souhlen (Anel)

focus = 0
next_focus = 0
talk_start = 0
msgHi = 'Heylo! Would you like to broadcast anything?'
msgBye = 'See you another time!'
price = 100
inConvo = 0
broadcast = ''
cast = ''
target = 0
following = false
attacking = false

function onThingMove(creature, thing, oldpos, oldstackpos)
end

function onCreatureAppear(creature)
end

function onCreatureDisappear(cid, pos)
 	if (focus == cid) then
		selfSay(msgBye)
			focus = next_focus
			next_focus = 0
		distFocus = getDistanceToCreature(next_focus)
		if (distFocus ~= nil and distFocus < 4) then
			selfSay(msgHi)
			talk_start = os.clock()
			focus = next_focus
			next_focus = 0
			inConvo = 1
		else
			focus = 0
			talk_start = 0
			inConvo = 0
		end
 	end
end

function onCreatureTurn(creature)
end

function msgcontains(txt, str)
	return (string.find(txt, str) and not string.find(txt, '(%w+)' .. str) and not string.find(txt, str .. '(%w+)'))
end

function onCreatureSay(cid, type, msg)
    msg2 = msg
	msg = string.lower(msg)
	if (cast ~= '') then
	    selfSay('does nothing')
	-- do nothing, wait until message has been broadcasted and npc has replied
	elseif (getDistanceToCreature(cid) >= 4 and focus == cid) then
	    selfSay('Please come closer, '..creatureGetName(cid)..'. I can\'t hear what you are saying from over there.')
	    
	elseif (type == 1) then
	 	if ((string.find(msg, '(%a*)hi(%a*)') or string.find(msg, '(%a*)hello(%a*)') or string.find(msg, '(%a*)heylo(%a*)') or string.find(msg, '(%a*)greetings(%a*)')) and (focus == 0) and getDistanceToCreature(cid) < 4) then
			selfSay(msgHi)
			focus = cid
			talk_start = os.clock()
			inConvo = 1

		elseif ((string.find(msg, '(%a*)hi(%a*)') or string.find(msg, '(%a*)hello(%a*)') or string.find(msg, '(%a*)heylo(%a*)') or string.find(msg, '(%a*)greetings(%a*)')) and (focus ~= cid) and getDistanceToCreature(cid) < 4)  then
 			if (next_focus > 0 and getDistanceToCreature(next_focus) < 4) then
	 			selfSay('Take a nap while I help the ones before you, ' .. creatureGetName(cid) .. '.')
 			else
	 			selfSay('I\'ll soon come to you, ' .. creatureGetName(cid) .. '.')
 				next_focus = cid
 			end

	 	elseif (focus == cid) then
			talk_start = os.clock()

			if ((msgcontains(msg, 'broadcast') or msgcontains(msg, 'say')) and inConvo == 0) then
				if (getPlayerLevel(creatureGetName(cid)) >= 30) then
			    	selfSay('Do you want me to broadcast anything for you?')
			    	inConvo = 1
				else
				    selfSay('Come back when you\'re older!')
				end

			elseif (msgcontains(msg, 'yes') and inConvo == 1) then
				selfSay('Tell me what you want me to broadcast.')
				inConvo = 2

			elseif (msgcontains(msg, 'no') and inConvo == 1) then
				selfSay('Perhaps tomorrow then..')
		        if (next_focus > 0 and getDistanceToCreature(next_focus) < 4) then
	    			selfSay(msgHi)
    				talk_start = os.clock()
    				focus = next_focus
    				next_focus = 0
    				inConvo = 1
    			else
	    			focus = 0
    				talk_start = 0
    				inConvo = 0
    			end
    		    broadcast = ''

			elseif (inConvo == 2 and (msg ~= 'stop')) then
				if (string.len(msg) > 90) then
					selfSay('I am sorry, but your message can not be longer than 90 letters, including spaces and other characters. Say a new message or say STOP to end.')
				else
				    broadcast = msg2
					selfSay('Do you want to pay '..price..' gold for me to broadcast this message? "'..broadcast..'"')
					inConvo = 3
			    end

			elseif (msgcontains(msg, 'yes') and inConvo == 3) then
			    if (pay(cid, price)) then
			        selfSay('/B '..creatureGetName(cid)..' says: '..broadcast)
			        saycast = os.clock()
			        inConvo = 1
			        
                    io.output(io.open("broadcast.log", "a"))
                    io.write("["..os.date("%y.%m.%d %X").."] <"..creatureGetName(cid).."> "..broadcast.."\n")
                    io.close()
        		    cast = broadcast
			    else
			        selfSay('You can not afford this service. Please come back another time when you have more money!')
            		if (next_focus > 0) then
                		distFocus = getDistanceToCreature(next_focus)
                		if (distFocus ~= nil and distFocus < 4) then
        	    			selfSay(msgHi)
            				talk_start = os.clock()
            				focus = next_focus
            				next_focus = 0
            				inConvo = 1
            			else
        	    			focus = 0
            				talk_start = 0
            				inConvo = 0
            			end
            		    broadcast = ''
    			    end
			    end

			elseif (msgcontains(msg, 'no') and inConvo == 3) then
				selfSay('Ok then.')
				inConvo = 0
        		broadcast = ''

			elseif (string.find(msg, '(%a*)job(%a*)')) then
				selfSay('Oh, I am just sitting around here and gamble now and then.')
				inConvo = 0

 			elseif (string.find(msg, '(%a*)bye(%a*)')) then
	    		selfSay(msgBye)
        		if (next_focus > 0) then
            		distFocus = getDistanceToCreature(next_focus)
            		if (distFocus ~= nil and distFocus < 4) then
    	    			selfSay(msgHi)
        				talk_start = os.clock()
        				focus = next_focus
        				next_focus = 0
        				inConvo = 1
        			else
    	    			focus = 0
        				talk_start = 0
        				inConvo = 0
        			end
            		broadcast = ''
     			end
     	    end
 		end
 	end
end


function onCreatureChangeOutfit(creature)
end

function onThink()
    if (cast ~= '' and (os.clock() - saycast) > 1) then
        selfSay('Do you want to broadcast anything else?')
        cast = ''
    elseif (os.clock() - talk_start) > 30 then
        if (focus > 0) then	
    		selfSay(msgBye)
    		if (next_focus > 0) then
        		distFocus = getDistanceToCreature(next_focus)
        		if (distFocus ~= nil and distFocus < 4) then
        			selfSay(msgHi)
        			talk_start = os.clock()
        			focus = next_focus
        			next_focus = 0
        			inConvo = 1
		        else
        			focus = 0
        			talk_start = 0
        			inConvo = 0
    			end
    		else
    			focus = 0
    			talk_start = 0
    			inConvo = 0
    		end
        	broadcast = ''
    	end
 	elseif (focus ~= 0) then
 	    distFocus = getDistanceToCreature(focus)
		if (distFocus ~= nil and distFocus > 5) then
    		selfSay(msgBye)
    		if (next_focus > 0) then
        		distFocus = getDistanceToCreature(next_focus)
        		if (distFocus ~= nil and distFocus < 4) then
        			selfSay(msgHi)
        			talk_start = os.clock()
        			focus = next_focus
        			next_focus = 0
        			inConvo = 1
        		else
        			focus = 0
        			talk_start = 0
        			inConvo = 0
        		end
    		end
        	broadcast = ''
		end
	end
end