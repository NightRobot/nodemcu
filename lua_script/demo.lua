local sda_pin = 6 -- sda pin, GPIO12
local scl_pin = 5 -- scl pin, GPIO14

local oled_addr   = 0x3c
local bh1750_addr = 0x23
local htu21d_addr = 0x40

local cnt   = 1
local state = 0
local lux   = '--.-'
local temp  = '--.-'
local humid = '--.-'

i2c.setup(0, sda_pin, scl_pin, i2c.SLOW)
local disp = u8g.ssd1306_128x64_i2c(oled_addr)

local function read_bh1750() 
  local bh1750 = require("bh1750")
  bh1750.init(sda_pin, scl_pin, bh1750_addr)
  local value = bh1750.read()
  if value == nil then
    lux = '--.-'
  else
    lux = string.format("%.1f", value)
    print(lux)
  end
  bh1750=nil
  package.loaded["bh1750"]=nil
end 




tmr.alarm(1, 1000, 1, function()
  if state == 1 then
    read_bh1750()
  end
  state = (state+1) % 3
  collectgarbage()
end)