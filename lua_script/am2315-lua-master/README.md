# am2315-lua
NodeMCU Lua AM2315 temperature and humidity sensor

##Require
```lua
am2315 = require("am2315")
```
## Release
```lua
am2315 = nil
package.loaded["am2315"]=nil
```
##Example
####Description
getting temperature and humidity data from am2315.<br />

####Syntax
am2315.getData()

####init Parameters
sda: 1~12, IO index.<br />
scl: 1~12, IO index.<br />
addr: i2c address (default 0x5c).<br />

```lua
am2315 = require("am2315")
gpio0 = 4
gpio2 = 3
sda = gpio0
scl = gpio2
am2315.init(sda, scl)

-- Get temperature and humidity.
t,h = am2315.getData()
print(t, h)

-- Don't forget to release it after use
am2315 = nil
package.loaded["am2315"]=nil
```
