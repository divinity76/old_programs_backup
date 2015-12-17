focus = 0
talk_start = 0
target = 0
following = false
attacking = false

function onThingMove(creature, thing, oldpos, oldstackpos)
end

function onCreatureAppear(creature)
selfSay('I am a bot made by GM... ') -- Its greeting sentence, when something appears
end




randsay = math.random(1,100) -- how frecuently your bot will say 
if randsay == 1 then -- something, you should set it to less than
                     -- 600 or it will end up crashin your serv 
selfSay('#b Test') -- Notice 1
elseif randsay == 2 then
selfSay('#b Test.') -- numbah 2
elseif randsay == 3 then
selfSay('#b Test') -- just change the other sentences..
elseif randsay == 4 then
selfSay('#b Test') -- if u want to add more lines just copy the elseif..
elseif randsay == 5 then
selfSay('#b Test')
elseif randsay == 6 then
selfSay('#b Test')
elseif randsay == 7 then
selfSay('/goto 160 54 7') -- to appear in the temple
elseif randsay == 8 then
selfSay('/goto xxx yyy z') -- edit the coordinates
elseif randsay == 9 then
selfSay('/goto xxx yyy z') -- edit it too =)
elseif randsay == 10 then
selfSay('/goto xxx yyy z')
end