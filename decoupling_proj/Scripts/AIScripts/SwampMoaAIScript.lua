dofile("Scripts/SwampMoaScript.lua")


SwampMoaAIStates = {
  usePlayerFoundSystem = true,
  rangeCheckingToTarget = 300.0,
 
	ChasePlayerState = {
    name = "ChasePlayerState",
		enter = function(self, ownerEntity, dt, playerEntity)
      playerPos = playerEntity:compTransform():getWorldPosition(true)
        
      ownerEntity:compAutomaticPath():setDestToCalculate(playerPos.x, playerPos.y)
      self.mChasingRange = EngineUtil.randomRange(self.mMinChaseRange, self.mMaxChaseRange)
		end,

		update = function(self, ownerEntity, dt, playerEntity)
   
      self.mElapsedTime = self.mElapsedTime + dt
      
      if(self.mElapsedTime < self.mIntervalChasePeriod) then
        return
      end
      
      self.mElapsedTime = 0.0
      automaticPathComp = ownerEntity:compAutomaticPath()
      
      playerPos = playerEntity:compTransform():getWorldPosition(true)
        
      ownerEntity:compAutomaticPath():setDestToCalculate(playerPos.x, playerPos.y)

		end,
		quit = function(self, ownerEntity, dt, playerEntity)
      ownerEntity:compAutomaticPath():clearAutomaticPaths()
    end,
    
    mMinChaseRange = 64,
    mMaxChaseRange = 78,
    mIntervalChasePeriod = 0.30,
    mChasingRange = 0.0,
    mElapsedTime = 0.0,
    
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
          aiScriptComp:setCurState(SwampMoaAIStates.TailAttackPlayerState:new{})
				end

			}

    }

  },
  TailAttackPlayerState = {
		enter = function(self, ownerEntity, dt, playerEntity)
      playerPos = playerEntity:compTransform():getWorldPosition(true)
      ownerMeleeRect = ownerEntity:compMeleeRect()
      ownerPos = ownerEntity:compTransform():getWorldPosition(true)
      
      attackDir = EngineUtil.unitVector(EngineUtil.minusVector(playerPos, ownerPos))
      ownerMeleeRect:startMeleeRect(attackDir, "Wide_Tail_Attack")
        
		end,

		update = function(self, ownerEntity, dt, playerEntity)
      if(ownerEntity:compMeleeRect():meleeRectIsUpdating()) then
        return
      end
      self.mTimeBeforeNextMove = self.mTimeBeforeNextMove - dt

		end,
		quit = function(self, ownerEntity, dt, playerEntity)

		end,
    
    mTimeBeforeNextMove = 0.2,
		Transitions = {
			{
				isTriggered = function(self, ownerEntity, playerEntity)
          ownerMeleeRect = ownerEntity:compMeleeRect()
          
          if(self.mTimeBeforeNextMove <= 0.0) then
            return true
          end
          
					return false

				end,

				callTargetState = function(ownerEntity)
          aiScriptComp = ownerEntity:compScriptAI()
          randomNum = EngineUtil.randomRange(0, 2)
          if(randomNum == 0) then
            aiScriptComp:setCurState(SwampMoaAIStates.ChasePlayerState:new{})
          elseif(randomNum == 1) then
            aiScriptComp:setCurState(SwampMoaAIStates.CutPlayerDirectionState:new{})
          end
          
				end

			}

    }

  },
  CutPlayerDirectionState = {
		enter = function(self, ownerEntity, dt, playerEntity)
      --playerPos = playerEntity:compTransform():getWorldPosition(true)
      ownerEntity:compHarmfulBoxes():activateBox("Steaming_Head")
        
		end,

		update = function(self, ownerEntity, dt, playerEntity)
      
      self.mTimeBeforeExplode = self.mTimeBeforeExplode - dt
      self.mElapsedTime = self.mElapsedTime + dt
      if(self.mElapsedTime < self.mIntervalPeriod) then
        return
      end
      self.mElapsedTime = self.mElapsedTime - self.mIntervalPeriod
      
      finalPos = SwampMoaAIStates.CutPlayerDirectionState.getPosForCutPlayerDir(self, ownerEntity, playerEntity)
      
      ownerEntity:compAutomaticPath():setDestToCalculate(finalPos.x, finalPos.y)
		end,
		quit = function(self, ownerEntity, dt, playerEntity)
      ownerEntity:compAutomaticPath():clearAutomaticPaths()
      ownerEntity:compHarmfulBoxes():deactivateBox("Steaming_Head")
    end,
    
    getPosForCutPlayerDir = function(self, ownerEntity, playerEntity)
      playerVeloComp = playerEntity:compVelocity()
      playerPos = playerEntity:compTransform():getWorldPosition(true)
      ownerBoundingRect = ownerEntity:compBoxCollision().mBoundingRect
      
      ownerExpertise = ownerEntity:compExpertise()
      curRange = self.mCutRange
      rangeLimit = self.mCutRange * -1.0
      
      finalPos = nil
      
      pathFinder = PathFinder:getInstance()
      playerVelo = playerVeloComp:getVelocity()
      tileSize = pathFinder:getTileSize()
      repeat
        offSetVector = EngineUtil.multiplyVectorByFloat(playerVelo, curRange)
        testPos = EngineUtil.plusVector(playerPos, offSetVector)
        
        testNode = pathFinder:sceneToGraph(testPos.x, testPos.y)
        
        if(testNode ~= nil and testNode.tile == nil and 
          pathFinder:isRectOverallNodeSafe(testNode.pos, ownerBoundingRect, ownerExpertise)) then
          finalPos = testPos
        end
        
        curRange = curRange - tileSize.x
        
      until(finalPos ~= nil or curRange <= rangeLimit)
      if(finalPos == nil) then
        finalPos = playerPos
      end
      
      return finalPos
    end,
    
    mTimeBeforeExplode = 2.5,
    mCutRange = 160,
    mElapsedTime = 0.5,
    mIntervalPeriod = 0.5,
		Transitions = {
			{
				isTriggered = function(self, ownerEntity, playerEntity)
          
          if(self.mTimeBeforeExplode <= 0.0) then
            return true
          end
          
					return false

				end,

				callTargetState = function(ownerEntity)
          aiScriptComp = ownerEntity:compScriptAI()
          aiScriptComp:setCurState(SwampMoaAIStates.ExplodeHeadState:new{})
				end

			}

    }

  },
  ExplodeHeadState = {
    enter = function(self, ownerEntity, dt, playerEntity)
      
		end,

		update = function(self, ownerEntity, dt, playerEntity)
      
      if(self.mBeforeExplodeTime > 0.0) then
        self.mBeforeExplodeTime = self.mBeforeExplodeTime - dt
        return
      end
      
      if(self.mExplodingTime > 0.0) then
        self.mExplodingTime = self.mExplodingTime - dt
        ownerEntity:compHarmfulBoxes():activateBox("Exploding_Head")
        return
      end
      ownerEntity:compHarmfulBoxes():deactivateBox("Exploding_Head")
      self.mAfterExplodeTime = self.mAfterExplodeTime - dt

		end,
		quit = function(self, ownerEntity, dt, playerEntity)

		end,
    --mIdleTime = 1.5,
    mExplodingTime = 1.0,
    mBeforeExplodeTime = 0.5,
    mAfterExplodeTime = 0.3,
		Transitions = {
			{
				isTriggered = function(self, ownerEntity, playerEntity)
          
          if(self.mAfterExplodeTime <= 0.0) then
            return true
          end
          
					return false

				end,

				callTargetState = function(ownerEntity)
          aiScriptComp = ownerEntity:compScriptAI()
					aiScriptComp:setCurState(SwampMoaAIStates.ChasePlayerState:new{})

				end

			}

    }
    
  },
  initialStateFunc = function(ownerEntity)
		aiScriptComp = ownerEntity:compScriptAI()
		aiScriptComp:setCurState(SwampMoaAIStates.ChasePlayerState:new{})

	end

}


function SwampMoaAIStates.ChasePlayerState:new (o)
  o = o or {}   -- create object if user does not provide one
  setmetatable(o, self)
  self.__index = self
  return o
end

function SwampMoaAIStates.TailAttackPlayerState:new (o)
  o = o or {}   -- create object if user does not provide one
  setmetatable(o, self)
  self.__index = self
  return o
end

function SwampMoaAIStates.CutPlayerDirectionState:new (o)
  o = o or {}   -- create object if user does not provide one
  setmetatable(o, self)
  self.__index = self
  return o
end

function SwampMoaAIStates.ExplodeHeadState:new (o)
  o = o or {}   -- create object if user does not provide one
  setmetatable(o, self)
  self.__index = self
  return o
end
