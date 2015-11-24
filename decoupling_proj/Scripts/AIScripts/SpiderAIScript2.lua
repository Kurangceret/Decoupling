dofile("Scripts/SpiderScript.lua")
dofile("Scripts/UtilityScript.lua")

AIStates = {
	ChasePlayerState = {
    name = "ChasePlayerState",
		enter = function(self, ownerEntity, dt, playerEntity)
      playerPos = playerEntity:compTransform():getWorldPosition(true)
        
      ownerEntity:compAutomaticPath():setDestToCalculate(playerPos.x, playerPos.y)
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
  
    mElapsedTime = 0.0,

		Transitions = {
			{
				isTriggered = function(self, ownerEntity, playerEntity)
          playerPos = playerEntity:compTransform():getWorldPosition(true)
          ownerPos = ownerEntity:compTransform():getWorldPosition(true)
          ownerRect = ownerEntity:compBoxCollision():getTransformedRect()
          playerRect = playerEntity:compBoxCollision():getTransformedRect()
          
          offSetDir = EngineUtil.unitVector(EngineUtil.minusVector(playerPos, ownerPos))
          
          --if(EngineUtil.vectorLength(EngineUtil.minusVector(playerPos, ownerPos)) <= quickHeadAttackRange * 1.50) then
          if(EngineUtil.offSetRect1AndCheckRect2(ownerRect, offSetDir, 
              quickHeadAttackRange * 0.8, playerRect)) then
            return true
          end
                    
          return false

				end,

				callTargetState = function(ownerEntity)
          aiScriptComp = ownerEntity:compScriptAI()
          aiScriptComp:setCurState(AIStates.AttackPlayerState:new{})

				end

			}

    }

  },
  AttackPlayerState = {
    name = "AttackPlayerState",
		enter = function(self, ownerEntity, dt, playerEntity)
      ownerEntity:compAutomaticPath():clearAutomaticPaths()
      playerPos = playerEntity:compTransform():getWorldPosition(true)
      ownerPos = ownerEntity:compTransform():getWorldPosition(true)
      
      dirAttack = EngineUtil.unitVector(EngineUtil.minusVector(playerPos, ownerPos))
      ownerEntity:compMeleeRect():startMeleeRect(dirAttack, "Quick_Head_Attack")
		end,

		update = function(self, ownerEntity, dt, playerEntity)
      if(meleeRectComp:meleeRectIsUpdating() or meleeRectComp:isVulnerable()) then
        return
      end
      ownerEntity:compAutomaticPath():clearAutomaticPaths()
      playerPos = playerEntity:compTransform():getWorldPosition(true)
      ownerPos = ownerEntity:compTransform():getWorldPosition(true)
      
      dirAttack = EngineUtil.unitVector(EngineUtil.minusVector(playerPos, ownerPos))
      ownerEntity:compMeleeRect():startMeleeRect(dirAttack, "Quick_Head_Attack")
      
		end,
		quit = function(self, ownerEntity, dt, playerEntity)

		end,

		Transitions = {
			{
				isTriggered = function(self, ownerEntity, playerEntity)
          meleeRectComp = ownerEntity:compMeleeRect()
          
          if(meleeRectComp:meleeRectIsUpdating() or meleeRectComp:isVulnerable()) then
            return false
          end
          
          playerPos = playerEntity:compTransform():getWorldPosition(true)
          ownerPos = ownerEntity:compTransform():getWorldPosition(true)
          ownerRect = ownerEntity:compBoxCollision():getTransformedRect()
          playerRect = playerEntity:compBoxCollision():getTransformedRect()
          
          offSetDir = EngineUtil.unitVector(EngineUtil.minusVector(playerPos, ownerPos))
          if(EngineUtil.offSetRect1AndCheckRect2(ownerRect, offSetDir, 
              quickHeadAttackRange * 0.8, playerRect)) then
            return false
          end
          
                   
          return true

				end,

				callTargetState = function(ownerEntity)
          aiScriptComp = ownerEntity:compScriptAI()
          aiScriptComp:setCurState(AIStates.ChasePlayerState:new{})

				end

			}

    },
    curIteration = 0

  },
  WanderingState = {
    name = "WanderingState",
		enter = function(self, ownerEntity, dt, playerEntity)
      ownerEntity:compAutomaticPath():clearAutomaticPaths()
      
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
          

				end

			}

    }

  },

  initialStateFunc = function(ownerEntity)
		aiScriptComp = ownerEntity:compScriptAI()
		aiScriptComp:setCurState(AIStates.ChasePlayerState:new{})

	end

}



function AIStates.ChasePlayerState:new (o)
  o = o or {}   -- create object if user does not provide one
  setmetatable(o, self)
  self.__index = self
  return o
end

function AIStates.AttackPlayerState:new(o)
  o = o or {}
  setmetatable(o, self)
  self.__index = self
  return o
end

function AIStates.WanderingState:new(o)
  o = o or {}
  setmetatable(o, self)
  self.__index = self
  return o
  
end

t = AIStates.ChasePlayerState:new{}
t2 = AIStates.ChasePlayerState:new{}
print (t == AIStates.ChasePlayerState)



