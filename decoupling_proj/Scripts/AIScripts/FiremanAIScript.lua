dofile("Scripts/UtilityScript.lua")


FiremanAIStates = {
  usePlayerFoundSystem = true,
  rangeCheckingToTarget = 300.0,
  rayCastTileChecker = function(curNode)
    if(curNode == nil or curNode.tile ~= nil) then
      return false
    end
    return true
  end,
	ApproachPlayerState = {
		enter = function(self, ownerEntity, dt, playerEntity)
      self.mChasingRange = EngineUtil.randomRange(self.mMinChasingRange, self.mMaxChasingRange)
      self.mElapsedTime = self.mTimeForEachRefreshPath
		end,

		update = function(self, ownerEntity, dt, playerEntity)
      veloComp = ownerEntity:compVelocity()
      veloComp:setSpeedIdentifier(1)
      if(self.mElapsedSlowSpeedTime < self.mTimeForSlowSpeed) then
        veloComp:setSpeedIdentifier(0.5)
        self.mElapsedSlowSpeedTime = self.mElapsedSlowSpeedTime + dt
      end
      
      self.mElapsedTime = self.mElapsedTime + dt
      if(self.mElapsedTime < self.mTimeForEachRefreshPath) then
        return
      end
      self.mElapsedTime = self.mElapsedTime - self.mTimeForEachRefreshPath
      playerPos = playerEntity:compTransform():getWorldPosition(true)
     
      ownerEntity:compAutomaticPath():setDestToCalculate(playerPos.x, playerPos.y)
		end,
		quit = function(self, ownerEntity, dt, playerEntity)
      ownerEntity:compAutomaticPath():clearAutomaticPaths()
		end,
    Transitions = {
			{
				isTriggered = function(self, ownerEntity, playerEntity)
          playerPos = playerEntity:compTransform():getWorldPosition(true)
          ownerPos = ownerEntity:compTransform():getWorldPosition(true)
                    
          if(EngineUtil.vectorLength(EngineUtil.minusVector(playerPos, ownerPos)) <= self.mChasingRange) then
            return true
          end
          return false
				end,

				callTargetState = function(ownerEntity)
          aiScriptComp = ownerEntity:compScriptAI()
					--aiScriptComp:setCurState(AIStates.VerticalState:new{})
          aiScriptComp:setCurState(FiremanAIStates.FistPlayerState:new{})
				end

      }--[[,
      {
				isTriggered = function(self, ownerEntity, playerEntity)
          if(ownerEntity:compHealth():getCurrentHealth() <= 0) then
            return true
          end
          return false
				end,

				callTargetState = function(ownerEntity)
          aiScriptComp = ownerEntity:compScriptAI()
					--aiScriptComp:setCurState(AIStates.VerticalState:new{})
          aiScriptComp:setCurState(FiremanAIStates.ExplodeBeforeRemovedState:new{})
				end

			}--]]

    },
    mMinChasingRange = 70,
    mMaxChasingRange = 85,
    mChasingRange = 0.0,
    mTimeForSlowSpeed = 2.0,
    mElapsedSlowSpeedTime = 0.0,
    mTimeForEachRefreshPath = 0.3,
    mElapsedTime = 0.0,
    

  },
  FistPlayerState = {
		enter = function(self, ownerEntity, dt, playerEntity)
      meleeRectComp = ownerEntity:compMeleeRect()
      playerPos = playerEntity:compTransform():getWorldPosition(true)
      ownerPos = ownerEntity:compTransform():getWorldPosition(true)
      attackDir = EngineUtil.unitVector(EngineUtil.minusVector(playerPos, ownerPos))
      
      meleeRectComp:startMeleeRect(attackDir, "First_Fist_Attack")
		end,

		update = function(self, ownerEntity, dt, playerEntity)
      meleeRectComp = ownerEntity:compMeleeRect()
      if(self.mHasFirstFistAttackOver ~= true and (meleeRectComp:meleeRectIsUpdating() 
          or meleeRectComp:isVulnerable())) then
        return
      end
      self.mHasFirstFistAttackOver = true
      
      
      --self.mTimeForChasingTarget = self.mTimeForChasingTarget - dt
      self.mElapsedChasingTargetTime = self.mElapsedChasingTargetTime + dt
      
      veloComp = ownerEntity:compVelocity()
      
      playerPos = playerEntity:compTransform():getWorldPosition(true)
      ownerPos = ownerEntity:compTransform():getWorldPosition(true)
      
      if(self.mTimeForChasingTarget > self.mElapsedChasingTargetTime or
        (EngineUtil.vectorLength(EngineUtil.minusVector(playerPos, ownerPos)) > self.mSecondChasingRange
          and self.mTimeForChasingTarget / 2.0 > self.mElapsedChasingTargetTime)) then
        
        if(self.mFinalDirToTarget == nil) then
          playerPos = playerEntity:compTransform():getWorldPosition(true)
          ownerPos = ownerEntity:compTransform():getWorldPosition(true)
          self.mFinalDirToTarget = EngineUtil.unitVector(EngineUtil.minusVector(playerPos, ownerPos))
        end
        
        veloComp:setSpeedIdentifier(1.3)
        veloComp:setVelocity(self.mFinalDirToTarget.x, self.mFinalDirToTarget.y)
        return
      end
      
      if(self.mHasStartedSecondFistAttack ~= true) then
        playerPos = playerEntity:compTransform():getWorldPosition(true)
        ownerPos = ownerEntity:compTransform():getWorldPosition(true)
        attackDir = EngineUtil.unitVector(EngineUtil.minusVector(playerPos, ownerPos))
        meleeRectComp:startMeleeRect(attackDir, "Second_Fist_Attack")
        veloComp:setVelocity(0.0, 0.0)
        veloComp:setSpeedIdentifier(1.0)
        self.mHasStartedSecondFistAttack = true
      end
      
      
		end,
		quit = function(self, ownerEntity, dt, playerEntity)
      
		end,
    
    mHasFirstFistAttackOver = false,
    mHasStartedSecondFistAttack = false,
    --mHasSecondFistAttackOver = false,
    --mHasTrackTarget = false,
    mFinalDirToTarget = nil,
    mElapsedChasingTargetTime = 0.0,
    mTimeForChasingTarget = 1.0,
    mSecondChasingRange = 60,
		Transitions = {
			{
				isTriggered = function(self, ownerEntity, playerEntity)
          meleeRectComp = ownerEntity:compMeleeRect()
          
          if(self.mHasStartedSecondFistAttack and meleeRectComp:meleeRectIsUpdating() ~= true and
            meleeRectComp:isVulnerable() ~= true) then
            return true
          end

					return false

				end,

				callTargetState = function(ownerEntity)
          aiScriptComp = ownerEntity:compScriptAI()
					--aiScriptComp:setCurState(AIStates.VerticalState:new{})
          aiScriptComp:setCurState(FiremanAIStates.RunFromPlayerState:new{})
				end

			}

    }

  },
  RunFromPlayerState = {
		enter = function(self, ownerEntity, dt, playerEntity)
      eightDirections = Utility.getRandomizedEightDirections()
      playerPos = playerEntity:compTransform():getWorldPosition(true)
      
      currentRange = self.mMaxEscapeRange
      --finalPos = nil
      pathFinder = PathFinder:getInstance()
      tileSize = pathFinder:getTileSize()
      
      ownerRect = ownerEntity:compBoxCollision().mBoundingRect
      ownerExpertise = ownerEntity:compExpertise()
      repeat
        for i,dir in ipairs(eightDirections) do
          --print(dir)
          testPos = EngineUtil.plusVector(playerPos, EngineUtil.multiplyVectorByFloat(dir, currentRange))
          curNode = pathFinder:sceneToGraph(testPos.x, testPos.y)
          
          if(curNode ~= nil and curNode.tile == nil and 
            pathFinder:isRectOverallNodeSafe(testPos, ownerRect, ownerExpertise)) then
              self.mFinalEscapePos = testPos
              break
          end
        end
        currentRange = currentRange - tileSize.x
      until(self.mFinalEscapePos ~= nil or currentRange < self.mMinEscapeRange)
      --print(self.mFinalEscapePos)
      if(self.mFinalEscapePos ~= nil) then
        ownerEntity:compAutomaticPath():setDestToCalculate(self.mFinalEscapePos.x, self.mFinalEscapePos.y)
      end
      
		end,

		update = function(self, ownerEntity, dt, playerEntity)
      if(self.mFinalEscapePos == nil) then
        return
      end
      
      self.mElapsedTime = self.mElapsedTime + dt
		end,
		quit = function(self, ownerEntity, dt, playerEntity)

		end,
    mMaxEscapeRange = 400.0,
    mMinEscapeRange = 300.0,
    mFinalEscapePos = nil,
    mEscapeTime = 2.5,
    mElapsedTime = 0.0,
    
		Transitions = {
			{
				isTriggered = function(self, ownerEntity, playerEntity)
          if(self.mFinalEscapePos == nil or self.mElapsedTime >= self.mEscapeTime or
            ownerEntity:compAutomaticPath():isAutomaticPathsEmpty()) then
            return true
          end

					return false

				end,

				callTargetState = function(ownerEntity)
          aiScriptComp = ownerEntity:compScriptAI()
					--aiScriptComp:setCurState(AIStates.VerticalState:new{})
          aiScriptComp:setCurState(FiremanAIStates.ApproachPlayerState:new{})
				end

			}

    }

  },
  ExplodeBeforeRemovedState = {
		enter = function(self, ownerEntity, dt, playerEntity)
      
		end,

		update = function(self, ownerEntity, dt, playerEntity)
      
      
		end,
		quit = function(self, ownerEntity, dt, playerEntity)

		end,
    
   
    
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
		aiScriptComp:setCurState(FiremanAIStates.ApproachPlayerState:new{})

	end
  
}

function FiremanAIStates.ApproachPlayerState:new (o)
  o = o or {}   -- create object if user does not provide one
  setmetatable(o, self)
  self.__index = self
  return o
end

function FiremanAIStates.FistPlayerState:new (o)
  o = o or {}   -- create object if user does not provide one
  setmetatable(o, self)
  self.__index = self
  return o
end

function FiremanAIStates.RunFromPlayerState:new (o)
  o = o or {}   -- create object if user does not provide one
  setmetatable(o, self)
  self.__index = self
  return o
end

function FiremanAIStates.ExplodeBeforeRemovedState:new (o)
  o = o or {}   -- create object if user does not provide one
  setmetatable(o, self)
  self.__index = self
  return o
end