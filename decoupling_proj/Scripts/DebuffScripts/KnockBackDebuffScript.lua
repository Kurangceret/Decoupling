dofile ("Scripts/ConstantScript.lua")
dofile("Scripts/CategoryScript.lua")


KnockBackDebuff = {
  name = "KnockBackDebuff",
  mUseBuffTimer = true,
  mBuffTimer = 0.1,
  mKnockBackDirX = 0.0,
  mKnockBackDirY = 0.0,
  enter = function(self, ownerEntity, dt)
    
  end,
  
  update = function(self, ownerEntity, dt)
    ownerEntity:compVelocity():setSpeedIdentifier(1.5)
    ownerEntity:compVelocity():setVelocity(self.mKnockBackDirX, self.mKnockBackDirY)
  end,
  
  exit = function(self, ownerEntity, dt)
    ownerEntity:compVelocity():setVelocity(0.0, 0.0)
    ownerEntity:compVelocity():setSpeedIdentifier(1.0)
  end,
  
  isBuffOver = function(self, ownerEntity)
    return false
  end,
  
  newBuffFunc = function(ownerEntity)
    ownerEntity:compBuffable():insertNewBuff(KnockBackDebuff:new{})
  end
   
}

function KnockBackDebuff:new(o)
  o = o or {}   -- create object if user does not provide one
  setmetatable(o, self)
  self.__index = self
  return o
end
    