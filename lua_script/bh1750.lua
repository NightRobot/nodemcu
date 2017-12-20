local moduleName = ... 
local M = {}
_G[moduleName] = M
local id = 0 -- i2c device ID
local addr = 0x23 -- default i2c address
local init = false
local lux
local i2c = i2c
function M.init(sda, scl,address)
  i2c.setup(id,sda,scl,i2c.SLOW)
  if (address ~= nil ) then
    addr = address
  end
  i2c.start(id)
  i2c.address(id, addr, i2c.TRANSMITTER)
  i2c.write(id, 0x01) --power-on
  i2c.stop(id)
  i2c.start(id)
  i2c.address(id, addr, i2c.TRANSMITTER)
  i2c.write(id, 0x10) -- set mode
  i2c.stop(id)
  tmr.delay(250000) -- delay for 250 msec
  init = true
end

local function read_data()
  i2c.start(id)
  i2c.address(id, addr, i2c.RECEIVER)
  recv = i2c.read(id, 2) -- read two bytes
  i2c.stop(id)
  return recv
end

function M.read()
  if (not init) then
    M.init()
  end  
  data = read_data()
  lux = (256 * data:byte(1) + data:byte(2))/1.2
  return lux
end

return M    
