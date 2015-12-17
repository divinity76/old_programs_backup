-- 7.5 version by Darkonia Team, Mod by Peonso
 
 MAX_FOOD = 30000
 
 function onUse(cid, item, frompos, item2, topos)
 	
 if item.itemid == 2667 then
 	food = 2999500
 elseif item.itemid == 2674 then
 	food = 25
 elseif item.itemid == 2676 then
 	food = 30
 elseif item.itemid == 2669 then
 	food = 100
 elseif item.itemid == 2677 then
 	food = 5
 elseif item.itemid == 2689 then
 	food = 35
 elseif item.itemid == 2690 then
 	food = 35
 elseif item.itemid == 2691 then
 	food = 35
 elseif item.itemid == 2688 then
 	food = 15
 elseif item.itemid == 2684 then
 	food = 15
 elseif item.itemid == 2696 then
 	food = 40
 elseif item.itemid == 2679 then
 	food = 5
 elseif item.itemid == 2678 then
 	food = 30
 elseif item.itemid == 2687 then
 	food = 10
 elseif item.itemid == 2686 then
 	food = 30
 elseif item.itemid == 2796 then
 	food = 50
 elseif item.itemid == 2672 then
 	food = 130
 elseif item.itemid == 2695 then
 	food = 20
 elseif item.itemid == 2681 then
 	food = 15
 elseif item.itemid == 2671 then
 	food = 90
 elseif item.itemid == 2666 then
 	food = 85
 elseif item.itemid == 2682 then
 	food = 25
 elseif item.itemid == 2792 then
 	food = 50
 elseif item.itemid == 2793 then
 	food = 50
 elseif item.itemid == 2794 then
 	food = 50
 elseif item.itemid == 2795 then
 	food = 50
 elseif item.itemid == 2788 then
 	food = 55
 elseif item.itemid == 2789 then
 	food = 80
 elseif item.itemid == 2790 then
 	food = 70
 elseif item.itemid == 2787 then
 	food = 70
 elseif item.itemid == 2791 then
 	food = 50
 elseif item.itemid == 2675 then
 	food = 40
 elseif item.itemid == 2673 then
 	food = 35
 elseif item.itemid == 2683 then
 	food = 90
 elseif item.itemid == 2690 then
 	food = 60
 elseif item.itemid == 2668 then
 	food = 55
 elseif item.itemid == 2670 then
 	food = 45
 elseif item.itemid == 2680 then
 	food = 10
 elseif item.itemid == 2685 then
 	food = 20
 end
 	
 	if (getPlayerFood(cid) + food > MAX_FOOD) then
 		doPlayerSendCancel(cid,"You are full.")
 		return 1
 	end
 	
 	doPlayerFeed(cid,food)
 	doRemoveItem(item.uid,1)
 	return 1
 end