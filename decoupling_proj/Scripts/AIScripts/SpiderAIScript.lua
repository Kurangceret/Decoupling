


AIStates = {
	HorizontalState = {
		enter = function(self, ownerEntity, dt, playerEntity)

		end,

		update = function(self, ownerEntity, dt, playerEntity)
   
			automaticPathComp = ownerEntity:compAutomaticPath()

			if(automaticPathComp:isAutomaticPathsEmpty() == false) then
				return
			end

			if(self.curIteration % 2 == 0) then
				automaticPathComp:setDestToCalculate(150.0, 150.0)

			elseif(self.curIteration % 2 == 1) then
				automaticPathComp:setDestToCalculate(550.0, 150.0)

			end

			self.curIteration = self.curIteration + 1

		end,
		quit = function(self, ownerEntity, dt, playerEntity)

		end,

		Transitions = {
			{
				isTriggered = function(self, ownerEntity, playerEntity)
          if(self.curIteration == 2 
            and ownerEntity:compAutomaticPath()
            :isAutomaticPathsEmpty()) 
          then return true end

					return false

				end,

				callTargetState = function(ownerEntity)
          aiScriptComp = ownerEntity:compScriptAI()
					aiScriptComp:setCurState(AIStates.VerticalState:new{})

				end

			}

    },
    curIteration = 0

  },
  VerticalState = {
		enter = function(self, ownerEntity, dt, playerEntity)

			
		end,

		update = function(self, ownerEntity, dt, playerEntity)
    
			automaticPathComp = ownerEntity:compAutomaticPath()

			if(automaticPathComp:isAutomaticPathsEmpty() == false) then
				return
			end

			if(self.curIteration % 2 == 0) then
				automaticPathComp:setDestToCalculate(350.0, 400.0)

			elseif(self.curIteration % 2 == 1) then
				automaticPathComp:setDestToCalculate(350.0, 250.0)

			end

			self.curIteration = self.curIteration + 1

		end,
		quit = function(self, ownerEntity, dt, playerEntity)

		end,

		Transitions = {
			{
				isTriggered = function(self, ownerEntity, playerEntity)
				 if(self.curIteration == 2 
            and ownerEntity:compAutomaticPath()
            :isAutomaticPathsEmpty()) 
          then return true end

					return false

				end,

				callTargetState = function(ownerEntity)
          aiScriptComp = ownerEntity:compScriptAI()
					aiScriptComp:setCurState(AIStates.HorizontalState:new{})

				end

			}

    },
    curIteration = 0

  },
  initialStateFunc = function(ownerEntity)
		aiScriptComp = ownerEntity:compScriptAI()
		aiScriptComp:setCurState(AIStates.HorizontalState:new{})

	end

}
print(1 << 2)
--States.HorizontalState.enter(t, i)

function AIStates.HorizontalState:new (o)
  o = o or {}   -- create object if user does not provide one
  setmetatable(o, self)
  self.__index = self
  return o
end

function AIStates.VerticalState:new(o)
  o = o or {}   -- create object if user does not provide one
  setmetatable(o, self)
  self.__index = self
  return o
  
end


--[[
enter = function(ownerEntity, dt)

			print("")
		end

		update = function(ownerEntity, dt)
			automaticPathComp = ownerEntity:automaticPathComp()

			if(automaticPathComp:isAutomaticPathsEmpty()) then
				return
			end

			if(curIteration % 2 == 0) then
				automaticPathComp:setDestToCalculate(150.0, 150.0)

			else if(curIteration % 2 == 1) then
				automaticPathComp:setDestToCalculate(550.0, 150.0)

			end
			curIteration = curIteration + 1

		end
		quit = function(ownerEntity, dt)

		end

		Transitions = {
			{
				isTriggered = function(curStateTable, ownerEntity)
					curIteration = curStateTable.index("curIteration"):castInt()

					if(curIteration == 2) then	return true	end

					return false

				end,

				callTargetState = function(ownerEntity)


				end

			}

		}
--]]
