dofile("Scripts/SpiderScript.lua")
dofile("Scripts/UtilityScript.lua")

SpiderAIStates = {
  usePlayerFoundSystem = true,
  rangeCheckingToTarget = 300.0,
  
	ChasePlayerState = {
    name = "ChasePlayerState",
		enter = function(self, ownerEntity, dt, playerEntity)
      playerPos = playerEntity:compTransform():getWorldPosition(true)
        
      ownerEntity:compAutomaticPath():setDestToCalculate(playerPos.x, playerPos.y)
      self.mChasingRange = EngineUtil.randomRange(64, 100)
		end,

		update = function(self, ownerEntity, dt, playerEntity)
      self.mElapsedTime = self.mElapsedTime + dt
      
      if(self.mElapsedTime < 0.30) then
        return
      end
      
      self.mElapsedTime = 0.0
      automaticPathComp = ownerEntity:compAutomaticPath()
      --if(automaticPathComp:isAutomaticPathsEmpty()) then
      playerPos = playerEntity:compTransform():getWorldPosition(true)
        
      ownerEntity:compAutomaticPath():setDestToCalculate(playerPos.x, playerPos.y)
      --end
		end,
		quit = function(self, ownerEntity, dt, playerEntity)

    end,
    
    mChasingRange = 0.0,
    mElapsedTime = 0.0,

		Transitions = {
			{
				isTriggered = function(self, ownerEntity, playerEntity)
          playerPos = playerEntity:compTransform():getWorldPosition(true)
          ownerPos = ownerEntity:compTransform():getWorldPosition(true)
          ownerRect = ownerEntity:compBoxCollision():getTransformedRect()
          playerRect = playerEntity:compBoxCollision():getTransformedRect()
          
          offSetDir = EngineUtil.unitVector(EngineUtil.minusVector(playerPos, ownerPos))
          
          --print(self.mChasingRange)
          if(EngineUtil.vectorLength(EngineUtil.minusVector(playerPos, ownerPos)) <= self.mChasingRange) then
          --[[if(EngineUtil.offSetRect1AndCheckRect2(ownerRect, offSetDir, 
              self.mChasingRange, playerRect)) then]]--
            return true
          end
                    
          return false

				end,

				callTargetState = function(ownerEntity)
          aiScriptComp = ownerEntity:compScriptAI()
          aiScriptComp:setCurState(SpiderAIStates.ChargePlayerState:new{})
          
            
				end

			}

    }

  },
  ChargePlayerState = {
    name = "ChargePlayerState",
    enter = function(self, ownerEntity, dt, playerEntity)
      ownerEntity:compAutomaticPath():clearAutomaticPaths()
      
    end,
    update = function(self, ownerEntity, dt, playerEntity)
      ownerVelo = ownerEntity:compVelocity()
      
      if(self.mChargingTime <= 0.0) then
        self.mRecoveryTime = self.mRecoveryTime - dt
        ownerVelo:setVelocity(0, 0)
        return
      end
      
      playerPos = playerEntity:compTransform():getWorldPosition(true)
      ownerPos = ownerEntity:compTransform():getWorldPosition(true)
      
      self.mTrackPlayerTime = self.mTrackPlayerTime - dt
      
      if(self.mTrackPlayerTime > 0.0) then
        self.mChargingDir = EngineUtil.unitVector(EngineUtil.minusVector(playerPos, ownerPos))
      end
      ownerEntity:compTextDisplay():setString("Charging")
      self.mIdleTime = self.mIdleTime - dt
      if(self.mIdleTime > 0.0) then
        return
      end
      
      if(not self.mHasStartedMeleeRect) then
        newVector = sfVector2f()
        newVector.x = 0
        newVector.y = -1.0
        ownerEntity:compMeleeRect():startMeleeRect(newVector, "Charge_Attack")
        self.mHasStartedMeleeRect = true
      end
      
      ownerEntity:compTextDisplay():setString("")
      self.mChargingTime = self.mChargingTime - dt
      
      --[[if(self.mChargingTime <= 0.0) then
        return
      end]]--
        
      
      
      ownerVelo:setVelocityX(self.mChargingDir.x)
      ownerVelo:setVelocityY(self.mChargingDir.y)
      ownerVelo:setSpeedIdentifier(4.0)
      
    end,
    quit = function(self, ownerEntity, dt, playerEntity)
      ownerVelo = ownerEntity:compVelocity()
      ownerVelo:setVelocityX(0.0)
      ownerVelo:setVelocityY(0.0)
      ownerVelo:setSpeedIdentifier(1.0)
      
    end,
    mTrackPlayerTime = 1.00,
    mIdleTime = 1.25,
    mChargingTime = 0.3,
    mRecoveryTime = 0.4,
    mChargingDir,
    mHasStartedMeleeRect = false,
    
    Transitions = {
			{
        isTriggered = function(self, ownerEntity, playerEntity)
          if(self.mRecoveryTime > 0.0) then
            return false
          end
          return true
        end,
        callTargetState = function(ownerEntity)
          aiScriptComp = ownerEntity:compScriptAI()
          aiScriptComp:setCurState(SpiderAIStates.WanderingState:new{})
          
        end
        
      }
      
    }
    
  },
  
  WanderingState = {
    name = "WanderingState",
		enter = function(self, ownerEntity, dt, playerEntity)
      ownerEntity:compAutomaticPath():clearAutomaticPaths()
      
		end,

		update = function(self, ownerEntity, dt, playerEntity)
      ownerPathComp = ownerEntity:compAutomaticPath()
      self.mWanderingTime = self.mWanderingTime - dt
      if(not ownerPathComp:isAutomaticPathsEmpty()) then
        return
      end
      playerPos = playerEntity:compTransform():getWorldPosition(true)
      ownerPos = ownerEntity:compTransform():getWorldPosition(true)
      
      
      playerVeloComp = playerEntity:compVelocity()
      
      playerVelo = playerVeloComp:getVelocity()
      playerToOwnerDir = EngineUtil.unitVector(EngineUtil.minusVector(ownerPos, playerPos))
      
      dotProd = EngineUtil.getDotProduct(playerVelo, playerToOwnerDir)
      wanderVelo = nil
      if(dotProd > 0) then
        wanderVelo = playerVelo
      elseif(dotProd <= 0) then
        wanderVelo = EngineUtil.multiplyVectorByFloat(playerVelo, -1)
      end
      
      if(wanderVelo.x == 0 and wanderVelo.y == 0) then
        wanderVelo.x = playerToOwnerDir.x
        wanderVelo.y = playerToOwnerDir.y
      end
      
      wanderTable = SpiderAIStates.WanderingState.getWanderPosition(ownerEntity:compBoxCollision(), wanderVelo, playerPos)
      if(not wanderTable[3]) then
        self.mNoWanderPos = true
        return
      end
      ownerPathComp:setDestToCalculate(wanderTable[1], wanderTable[2])
      
		end,
		quit = function(self, ownerEntity, dt, playerEntity)

    end,
    getWanderPosition = function(ownerBoxCol, wanderVelo, playerPos)
      finalTable = {0, 0, false}
      
      lowestRange = 120
      highestRange = 360
      
      pathFinder = PathFinder:getInstance()
      rangeFromPlayer = EngineUtil.randomRange(lowestRange, highestRange)
            
      boundingRect = ownerBoxCol.mBoundingRect
      foundFinalPos = false
      
      repeat
        plusDir = EngineUtil.degreeToVector(EngineUtil.vectorToDegree(wanderVelo, false) + 45.0)
        minusDir = EngineUtil.degreeToVector(EngineUtil.vectorToDegree(wanderVelo, false) - 45.0)
        
        plusPos = EngineUtil.plusVector(EngineUtil.multiplyVectorByFloat(plusDir, rangeFromPlayer), playerPos)
        minusPos = EngineUtil.plusVector(EngineUtil.multiplyVectorByFloat(minusDir, rangeFromPlayer), playerPos)
        
        midPos = EngineUtil.plusVector(EngineUtil.multiplyVectorByFloat(wanderVelo, rangeFromPlayer), playerPos)
        
        randomizerNum = EngineUtil.randomInt(2)
        --curLimitPos = (randomizerNum == 0) ? plusPos : minusPos
        curLimitPos = nil
        if(randomizerNum == 0) then
          curLimitPos = plusPos
        else
          curLimitPos = minusPos
        end
        
        midCoord = pathFinder:sceneToCoordinate(midPos.x, midPos.y)
        
        --[[curLimitCoord = pathFinder:sceneToCoordinate(curLimitPos.x, curLimitPos.y)
        curCoord = midCoord
        
        diffCoord = EngineUtil.minusVectorI(curLimitCoord, curCoord)]]--
               
        
        
        
        indexCount = 0
        repeat
          indexCount = indexCount + 1
          curLimitCoord = pathFinder:sceneToCoordinate(curLimitPos.x, curLimitPos.y)
          curCoord = midCoord
          diffCoord = EngineUtil.minusVectorI(curLimitCoord, curCoord)
          
          xAdder = 0.0
          yAdder = 0.0
        
          if(diffCoord.x ~= 0) then
            --xAdder = diffCoord.x > 0 ? 1 : -1
            if(diffCoord.x > 0) then
              xAdder = 1
            else
              xAdder = -1
            end
          end
          if(diffCoord.y ~= 0) then
            --yAdder = diffCoord.y > 0 ? 1 : -1
            if(diffCoord.y > 0) then
              yAdder = 1
            else
              yAdder = -1
            end
          end
          
          repeat 
            
            repeat
              
              node = pathFinder:At(curCoord.x, curCoord.y)
              if(node ~= nil and node.tile == nil and pathFinder:isRectOverallNodeSafe(node.pos, boundingRect)) then
                finalTable[1] = node.pos.x
                finalTable[2] = node.pos.y
                foundFinalPos = true
              end
              if(curCoord.x ~= curLimitCoord.x) then
                curCoord.x = curCoord.x + xAdder
              end
            until (curCoord.x == curLimitCoord.x or foundFinalPos)
            if(curCoord.y ~= curLimitCoord.y)then
              curCoord.y = curCoord.y + yAdder
            end
            curCoord.x = midCoord.x
          until (curCoord.y == curLimitCoord.y or foundFinalPos)
          
          --curLimitPos = curLimitPos == minusPos ? plusPos : minusPos
          if(curLimitPos == minusPos) then
            curLimitPos = plusPos
          else
            curLimitPos = minusPos
          end
        until(foundFinalPos or indexCount >= 2)
        
        rangeFromPlayer = rangeFromPlayer - 32
        
      until(foundFinalPos or rangeFromPlayer < lowestRange)
      
      --finalTable[1] = finalPos.x
      --finalTable[2] = finalPos.y
      finalTable[3] = foundFinalPos
      
      
      return finalTable
    end,
      
    mWanderingTime = 3.5,
    mNoWanderPos = false,
    
		Transitions = {
			{
				isTriggered = function(self, ownerEntity, playerEntity)
          if(self.mWanderingTime <= 0.0 or self.mNoWanderPos) then
            return true
          end
          
          
          return false

				end,
				callTargetState = function(ownerEntity)
          aiScriptComp = ownerEntity:compScriptAI()
          aiScriptComp:setCurState(SpiderAIStates.ChasePlayerState:new{})
				end

			}

    }

  },

  initialStateFunc = function(ownerEntity)
		aiScriptComp = ownerEntity:compScriptAI()
		aiScriptComp:setCurState(SpiderAIStates.ChasePlayerState:new{})

	end

}



function SpiderAIStates.ChasePlayerState:new (o)
  o = o or {}   -- create object if user does not provide one
  setmetatable(o, self)
  self.__index = self
  return o
end

function SpiderAIStates.ChargePlayerState:new(o)
  o = o or {}
  setmetatable(o, self)
  self.__index = self
  return o
end

function SpiderAIStates.WanderingState:new(o)
  o = o or {}
  setmetatable(o, self)
  self.__index = self
  return o
  
end


