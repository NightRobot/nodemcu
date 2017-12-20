local sda_pin = 2 
local scl_pin = 1 

local bh1750_addr = 0x23

local cnt   = 1
local state = 0
local lux   = '--.-'
local temp  = '--.-'
local humid = '--.-'
i2c.setup(0, sda_pin, scl_pin, i2c.SLOW)

local function read_bh1750() 
  local bh1750 = require("bh1750")
  bh1750.init(sda_pin, scl_pin, bh1750_addr)
  local value = bh1750.read()
  if value == nil then
    lux = '--.-'
  else
    lux = string.format("%.1f", value)
    print("bh1750 light : ",lux)
  end
  bh1750=nil
  package.loaded["bh1750"]=nil
end 

local function read_am2315()
    local am2315 = require("am2315")
    am2315.init(sda_pin, scl_pin)
    local t,h = am2315.getData()
    if temp == nil and humid == nil then
        temp = '--.-'
        humid = '--.-'
    else
        temp = string.format("%.1f", t)
        humid = string.format("%.1f", h)
        print("tempurature : ",temp)
        print("humidnity : ",humid)
    end
    am2315 = nil
    package.loaded["am2315"]=nil
end

tmr.alarm(1, 1000, 1, function()
  if state == 0 then
    read_am2315()
  end
  if state == 1 then
    read_bh1750()
  end
  if state == 2 then
    --read_bh1750()
    print("smart farm sansara")
  end
  state = (state+1) % 3
  collectgarbage()
end)
