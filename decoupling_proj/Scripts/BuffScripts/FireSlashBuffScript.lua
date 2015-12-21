dofile ("Scripts/ConstantScript.lua")
dofile("Scripts/CategoryScript.lua")


FireSlashBuff = {
  name = "FireSlashBuff",
  mUseBuffTimer = true,
  mBuffTimer = 3.0,
  mHasSpawnedFire = false,
  enter = function(self, ownerEntity, dt)
    --[[EventManager.getInstance():addLuaListener(ownerEntity, "EntityGotDamagedEvent", 
      SpiritShieldBuff.entityGotDamagedReactor)]]--
  end,
  
  update = function(self, ownerEntity, dt)
    meleeRectComp = ownerEntity:compMeleeRect()
    if(meleeRectComp:meleeRectIsUpdating() == false or self.mHasSpawnedFire) then
      return
    end
    self.mHasSpawnedFire = true
    
    createEntityEvent = Event.CreateNewEntityEvent()
    attackDir = meleeRectComp:getCurrentAttackDir()
    
    ownerPos = ownerEntity:compTransform():getWorldPosition(true)
    incrementalPos = EngineUtil.multiplyVectorByFloat(attackDir, 15.0)
    
    finalPos = EngineUtil.plusVector(ownerPos, incrementalPos)
    
    createEntityEvent:queueEntityScript("Sky", "Scripts/FireBallScript.lua", "FireBall", 
      {dir = attackDir, initialPos = finalPos},
      function(createdEntity, luaData)
        createdEntity:compVelocity():setVelocity(luaData.dir.x, luaData.dir.y)
        createdEntity:compTransform():setPosition(luaData.initialPos.x, luaData.initialPos.y)
      end)
    EventManager.getInstance():queueEvent(createEntityEvent)
  end,
  
  exit = function(self, ownerEntity, dt)
       
  end,
  
  isBuffOver = function(self, ownerEntity)
    return self.mHasSpawnedFire
  end,
  
  newBuffFunc = function(ownerEntity)
    ownerEntity:compBuffable():insertNewBuff(FireSlashBuff:new{})
  end
   
}

function FireSlashBuff:new(o)
  o = o or {}   -- create object if user does not provide one
  setmetatable(o, self)
  self.__index = self
  return o
end
    