--node.compile('bh1750.lua')
--file.remove('bh1750.lua')
for k,v in pairs(file.list()) do 
  print(k)
end
 
dofile("demo.lua")