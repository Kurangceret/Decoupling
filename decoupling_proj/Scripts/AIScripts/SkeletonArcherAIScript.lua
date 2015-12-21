


SkeletonArcherAIStates = {
  usePlayerFoundSystem = true,
  rangeCheckingToTarget = 400.0,
  rayCastTileChecker = function(curNode)
    if(curNode == nil or curNode.tile ~= nil) then
      return false
    end
    return true
  end,
	ShootPlayerState = {
		enter = function(self, ownerEntity, dt, playerEntity)
      self.mCurDelay = self.mInitialDelay
		end,

		update = function(self, ownerEntity, dt, playerEntity)
      if(self.mCurDelay > 0.0) then
        self.mCurDelay = self.mCurDelay - dt
        return
      end
			ownerPos = ownerEntity:compTransform():getWorldPosition(true)
      playerPos = playerEntity:compTransform():getWorldPosition(true)
      
      if(self.mCurProjectileSpawned >= self.mNumberOfProjectile) then
        if(EngineUtil.vectorLength(EngineUtil.minusVector(playerPos, ownerPos)) <= SkeletonArcherAIStates.rangeCheckingToTarget and
          RayCast.castRayLine(ownerPos.x, ownerPos.y, playerPos.x, playerPos.y, PathFinder.getInstance()
            , SkeletonArcherAIStates.rayCastTileChecker) == true) then
            self.mCurDelay = self.mNormalDelay
            self.mCurProjectileSpawned = 0
        end
        return
      end
      
      if(self.mCurProjectileSpawned > 0 and self.mElapsedTimeForProj < self.mTimingForEachProjectile) then
        self.mElapsedTimeForProj = self.mElapsedTimeForProj + dt
                
        return
      end
      self.mElapsedTimeForProj = 0.0
      
      createEntityEvent = Event.CreateNewEntityEvent()
      dirToPlayer = EngineUtil.unitVector(EngineUtil.minusVector(playerPos, ownerPos))
      createEntityEvent:queueEntityScript("Sky", "Scripts/PrototypeProjectile.lua", "PrototypeProjectile",
        {dir = dirToPlayer, initialPos = ownerPos},
        function (createdEntity, luaData) 
          createdEntity:compVelocity():setVelocity(luaData.dir.x, luaData.dir.y)
          createdEntity:compTransform():setPosition(luaData.initialPos.x, luaData.initialPos.y)
        end)
      
      EventManager.getInstance():queueEvent(createEntityEvent)
      
      self.mCurProjectileSpawned = self.mCurProjectileSpawned + 1
      
		end,
		quit = function(self, ownerEntity, dt, playerEntity)

		end,
    
    mInitialDelay = 1.0,
    mNormalDelay = 2.25,
    mCurDelay = 0.0,
    mTimingForEachProjectile = 0.5,
    mElapsedTimeForProj = 0.0,
    mNumberOfProjectile = 3,
    mCurProjectileSpawned = 0,
    
		Transitions = {
			{
				isTriggered = function(self, ownerEntity, playerEntity)
         

					return false

				end,

				callTargetState = function(ownerEntity)
          aiScriptComp = ownerEntity:compScriptAI()
					--aiScriptComp:setCurState(AIStates.VerticalState:new{})

				end

			}

    }

  },

  
  initialStateFunc = function(ownerEntity)
		aiScriptComp = ownerEntity:compScriptAI()
		aiScriptComp:setCurState(SkeletonArcherAIStates.ShootPlayerState:new{})

	end

}


function SkeletonArcherAIStates.ShootPlayerState:new (o)
  o = o or {}   -- create object if user does not provide one
  setmetatable(o, self)
  self.__index = self
  return o
end



