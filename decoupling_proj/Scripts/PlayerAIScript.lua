



AIStates = {
	FirstState = {
		enter = function(self, ownerEntity, dt)
			print("First State" .. self.curNum)
		end,

		update = function(self, ownerEntity, dt)
			self.curNum = self.curNum + 1
			print("First State" .. self.curNum)
		end,

		quit = function(self, ownerEntity, dt)

			print("quitting First State")
		end,

		Transitions = {
			{
				isTriggered = function(self, ownerEntity)
					if(self.curNum == 10) then return true end
					return false

				end,

				callTargetState = function(ownerEntity)
					aiScriptComp = ownerEntity:compScriptAI()
					aiScriptComp:setCurState(AIStates.SecondState:new{})
				end
			}

		},
		curNum = 0

	},


	SecondState = {
		enter = function(self, ownerEntity, dt)
			print("Second State" .. self.curNum)

		end,

		update = function(self, ownerEntity, dt)
			self.curNum = self.curNum + 1
			print("Second State" ..self.curNum)

		end,

		quit = function(self, ownerEntity, dt)

			print("quitting Second State")
		end,

		Transitions = {
			{
				isTriggered = function(self, ownerEntity)
					if(self.curNum == 10) then return true end
					return false
				end,

				callTargetState = function(ownerEntity)
					aiScriptComp = ownerEntity:compScriptAI()
					aiScriptComp:setCurState(AIStates.FirstState:new{})
				end
			}

		},
		curNum = 0


	},

	initialStateFunc = function(ownerEntity)
		aiScriptComp = ownerEntity:compScriptAI()
		aiScriptComp:setCurState(AIStates.SecondState:new{})

	end
}


function AIStates.FirstState:new(o)
	o = o or {}
	setmetatable(o, self)

	self.__index = self
	return o
end

function AIStates.SecondState:new(o)
	o = o or {}
	setmetatable(o, self)

	self.__index = self
	return o
end
