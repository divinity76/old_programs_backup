--Tapetries by Zapatron and leandro_benedetti_--
 
 function onUse(cid, item, frompos, item2, topos)
 
 cortina1 = {x=topos.x-1, y=topos.y, z=topos.z} 
 cortina2 = {x=topos.x+1, y=topos.y-1, z=topos.z} 
 cortina3 = {x=topos.x, y=topos.y+1, z=topos.z} 
 
 if item.itemid == 1857 or item.itemid == 1860 or item.itemid == 1863 or item.itemid == 1866 or item.itemid == 1869 or item.itemid == 1872 then 
 doRemoveItem(item.uid,1)
 doCreateItem(item.itemid-1,1,cortina1)
 return 1
 
 elseif item.itemid == 1856 or item.itemid == 1859 or item.itemid == 1862 or item.itemid == 1865 or item.itemid == 1868 or item.itemid == 1871 then
 doRemoveItem(item.uid,1)
 doCreateItem(item.itemid-1,1,cortina2)
 return 1
 
 elseif item.itemid == 1855 or item.itemid == 1858 or item.itemid == 1861 or item.itemid == 1864 or item.itemid == 1867 or item.itemid == 1870 then
 doRemoveItem(item.uid,1)
 doCreateItem(item.itemid+2,1,cortina3)
 
 
 return 1
 end
 end