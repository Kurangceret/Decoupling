dofile ("Scripts/ConstantScript.lua")
dofile("Scripts/CategoryScript.lua")


SpiritShieldBuff = {
  name = "SpiritShieldBuff",
  mUseBuffTimer = true,
  mBuffTimer = 5.0,
  mOwnerHasBeenDamaged = false,
  
  enter = function(self, ownerEntity, dt)
    EventManager.getInstance():addLuaListener(ownerEntity, "EntityGotDamagedEvent", 
      SpiritShieldBuff.entityGotDamagedReactor)
  end,
  
  update = function(self, ownerEntity, dt)
    ownerEntity:compHealth():setIsImmune(true)
  end,
  
  exit = function(self, ownerEntity, dt)
    EventManager.getInstance():deleteSpecificLuaListener(ownerEntity, "EntityGotDamagedEvent")
    ownerEntity:compHealth():setIsImmune(false)
  end,
  
  isBuffOver = function(self, ownerEntity)
    return self.mOwnerHasBeenDamaged
  end,
  
  newBuffFunc = function(ownerEntity)
    ownerEntity:compBuffable():insertNewBuff(SpiritShieldBuff:new{})
  end,
  
  entityGotDamagedReactor = function(ownerEntity, eventBase)
    if(ownerEntity.mId == eventBase:getEntityEventSender().mId) then
      ownerEntity:compBuffable():getFirstBuffByName("SpiritShieldBuff"):getLuaReferenceToBuff().mOwnerHasBeenDamaged = true
    end
  end
   
}

function SpiritShieldBuff:new(o)
  o = o or {}   -- create object if user does not provide one
  setmetatable(o, self)
  self.__index = self
  return o
end
    