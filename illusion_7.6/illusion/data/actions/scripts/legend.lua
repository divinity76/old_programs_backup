function onUse(cid, item, frompos, item2, topos)
 	
 		doSetItemText(item.uid,"Oh, a Book!")
 
 doShowTextWindow(item.uid,0,0)
 doSetItemText(item.uid,"")
 return 1
 end