
dofile ("Scripts/ConstantScript.lua")
dofile("Scripts/CategoryScript.lua")
--dofile ("ConstantScript.lua")


--test = "haha"


frameSizeMovDown = {26, 49}
frameSizeMovUp = {30, 49}
frameSizeMovSide = {71, 62}
frameSizeHeavyAttackSide = {111, 102}
frameSizeHeavyAttackDown = {96, 119}
frameSizeHeavyAttackUp = {90, 109}


prepAttackHeavy1Dur = 0.4
animPrepAttackHeavy1Dur = prepAttackHeavy1Dur / 5

prepAttackHeavy2Dur = 0.3
animPrepAttackHeavy2Dur = prepAttackHeavy2Dur / 4

prepAttackHeavy3Dur = 0.6
animPrepAttackHeavy3Dur = prepAttackHeavy3Dur / 4

attackHeavy1Dur = 0.6
animAttackHeavy1Dur = attackHeavy1Dur / 5

attackHeavy2Dur = 0.8
animAttackHeavy2Dur = attackHeavy2Dur / 5

attackHeavy3Dur = 0.9
animAttackHeavy3Dur = attackHeavy3Dur / 6

recoveryDur = 0.5
animRecoveryDur = recoveryDur / 8

boundingBoxSize = {6.5, 36.75, 17, 12.25}
originalCenterOrigin = {boundingBoxSize[1] + (boundingBoxSize[3] / 2), 
        boundingBoxSize[2] + (boundingBoxSize[4] / 2)}
Player = {
	initializeData = {
		SpriteComponent = {
			textureLocation = TextureDir .. "human_figure.png",
			textureRect = {0, 0, 26, 49},
			centerOrigin = true

		},

		TransformableComponent = {
			positionX = 96,
			positionY = 1520,
			centerOriginX = boundingBoxSize[1] + (boundingBoxSize[3] / 2),
			centerOriginY = boundingBoxSize[2] + (boundingBoxSize[4] / 2)
		},

		BoxCollisionComponent = {
			floatRect = {0, 0, boundingBoxSize[3], boundingBoxSize[4]},
      collisionReactor = function(thisEntity, collidedEntity, systemCollision)
        collidedCategoryComp = collidedEntity:compCategory()
        thisTransformComp = thisEntity:compTransform()
        
        if(collidedCategoryComp:getCategory() & Category.Enemy ~= 0) then
           vector = systemCollision:getCalculatedSafeOffSet(thisEntity)
           thisTransformComp:move(vector.x, vector.y)
        end
        
        
      end
		},

		VelocityComponent = {
			speed = 120,
			sprintSpeed = 110

    },
    CategoryComponent = {
      category = Category.Player
      
    },
  

		--[[AutomaticPathComponent = {

		},]]--

		StaminaComponent = {
			maxStamina = 100,
			maxStaminaRegen = 25
		},

		HealthComponent = {
			maxHealth = 50,
			currentHealth = 50
		},
    --Entity list right here shall be invoked with SceneGraph parent like component
    --so the entity in the list right here shall follow this entity's world transform
    --[[ChildEntityList = {
      {
        tableName = "FireSpirit",
        scriptDir = "Scripts/FireSpiritScript.lua",
        --Position list number of array also indicate the number of entity with the same type
        positionList = {
          {
            positionX = 200.0,
            positionY = 0.0
          },
          {
            positionX = -200.0,
            positionY = 0.0
          }
                    
        }
      }
      
    },]]--

		AnimationComponent = {
			animScriptDir = AnimScriptDir .. "PlayerAnim3Script.lua"

    },--[[
    LightPointComponent = {
      intensity = 0.0,
      radius = 125.0,
      size = 10.0,
      spreadAngle = PI * 2.0,
      softSpreadAngle = 0.0,
      bleed = 0.0,
      linearizeFactor = 1.0,
      alwaysUpdate = true
        
    },--]]
    AttackModeComponent = {
      attackModeList = {"Spear_Attack_", "Axe_Attack_"}
      
    },
    SpiritFormComponent = {
      passedThroughCategory = Category.Enemy,
      damageToPassedThroughCategory = 2.0,
      spiritSpeed = 500.0,
      normalSpiritLifeTime = 0.3,
      delayTime = 0.3,
      staminaIncrease = 25
      
    },
    MeleeRectComponent = {
			attackCategory = Category.Enemy,
			movementSetList = {
				{
					name = "Heavy_Attack_1",
					slashData = {
						{
							angle = 0.0,
							movingPos = {0, 0},
							swingDuration = 0.3 + prepAttackHeavy1Dur,
							delayDuration = prepAttackHeavy1Dur
						},

						{
							angle = 1.0,
							movingPos = {0, 0},
							swingDuration = 0.01,
							delayDuration = 0.0
						}

					},
					staminaUsage = 20,
					damage = 10,
					range = 50,
					thickness = 20,
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.25,
          recoveryDuration = 0.5

        },
        {
					name = "Heavy_Attack_2",
					slashData = {
						{
							angle = 45.0,
							movingPos = {0, 0},
							swingDuration = prepAttackHeavy2Dur + 0.01,
							delayDuration = prepAttackHeavy2Dur
						},

						{
							angle = 0.0,
							movingPos = {0, 0},
							swingDuration = 0.3,
							delayDuration = 0.0
						},
            {
							angle = -65.0,
							movingPos = {0, 0},
							swingDuration = 0.3,
							delayDuration = 0.0
						}

					},
					staminaUsage = 20,
					damage = 10,
					range = 50,
					thickness = 20,
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.25,
          recoveryDuration = 0.5

        },
        {
					name = "Heavy_Attack_3",
					slashData = {
						{
							angle = 0.0,
							movingPos = {0, 0},
							swingDuration = 1.2,
							delayDuration = prepAttackHeavy3Dur
						},

						{
							angle = 180.0,
							movingPos = {0, 0},
							swingDuration = 0.8,
							delayDuration = 0.3
						}

					},
					staminaUsage = 20,
					damage = 10,
					range = 50,
					thickness = 20,
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.25,
          recoveryDuration = 0.5

				},
        {
					name = "Spear_Attack_1",
					slashData = {
						{
							angle = 0.0,
							movingPos = {-10, -10},
							swingDuration = 0.11, --original 0.2
							delayDuration = 0.1
						},

						{
							angle = 0.0,
							movingPos = {10, 10},
							swingDuration = 0.25,
							delayDuration = 0.00
						}

          },
          entityMovingData = {
            speedIdentifier = 1.0
                        
          },
          
					staminaUsage = 20,
					damage = 3,
					range = 64,
					thickness = 10,
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.1,
          recoveryDuration = 0.5

				},
        {
					name = "Spear_Attack_2",
					slashData = {
						{
							angle = 0.0,
							movingPos = {0, 0},
							swingDuration = 0.11, -- original 0.2
							delayDuration = 0.1
						},

						{
							angle = 0.0,
							movingPos = {10, 10},
							swingDuration = 0.2,
							delayDuration = 0.0
						}

          },
          entityMovingData = {
            speedIdentifier = 1.0
                        
          },
          
					staminaUsage = 20,
					damage = 3,
					range = 64,
					thickness = 10,
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.1,
          recoveryDuration = 0.5

				},
        {
					name = "Spear_Attack_3",
					slashData = {
						{
							angle = 0.0,
							movingPos = {-20, -20},
							swingDuration = 0.4,
							delayDuration = 0.4
						},

						{
							angle = 0.0,
							movingPos = {10, 10},
							swingDuration = 0.35,
							delayDuration = 0.00
						}

          },
          entityMovingData = {
            speedIdentifier = 1.0
                        
          },
          
					staminaUsage = 20,
					damage = 6,
					range = 64,
					thickness = 10,
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.1,
          recoveryDuration = 0.5

				},
        {
					name = "Axe_Attack_1",
					slashData = {
						{
							angle = 90.0,
							movingPos = {0, 0},
							swingDuration = 0.11,
							delayDuration = 0.1
						},

						{
							angle = 0.0,
							movingPos = {0, 0},
							swingDuration = 0.09,
							delayDuration = 0.00
						},

						{
							angle = -90.0,
							movingPos = {0, 0},
							swingDuration = 0.09,
							delayDuration = 0.00
						}

          },
          entityMovingData = {
            speedIdentifier = 1.0
                        
          },
					staminaUsage = 20,
					damage = 3,
					range = 50,
					thickness = 20,
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.1,
          recoveryDuration = 0.5

				},
        {
					name = "Axe_Attack_2",
					slashData = {
						{
							angle = -45.0,
							movingPos = {0, 0},
							swingDuration = 0.11,
							delayDuration = 0.1
						},

						{
							angle = 0.0,
							movingPos = {0, 0},
							swingDuration = 0.09,
							delayDuration = 0.00
						},

						{
							angle = 45.0,
							movingPos = {0, 0},
							swingDuration = 0.09,
							delayDuration = 0.00
						}

          },
          entityMovingData = {
            speedIdentifier = 1.0
                        
          },
					staminaUsage = 20,
					damage = 3,
					range = 50,
					thickness = 20,
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.1,
          recoveryDuration = 0.5

				},
        {
					name = "Axe_Attack_3",
					slashData = {
						{
							angle = 0.0,
							movingPos = {0, 0},
							swingDuration = 0.4,
							delayDuration = 0.25
						},

						{
							angle = 1.0,
							movingPos = {0, 0},
							swingDuration = 0.01,
							delayDuration = 0.0
						}

          },
          entityMovingData = {
            speedIdentifier = 1.0
                        
          },
					staminaUsage = 20,
					damage = 6,
					range = 50,
					thickness = 20,
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.1,
          recoveryDuration = 0.5

				},
        {
					name = "Left_Claw_Attack",
					slashData = {
						{
							angle = -45.0,
							movingPos = {0, 0},
							swingDuration = 0.11,
							delayDuration = 0.1
						},

						{
							angle = 0.0,
							movingPos = {0, 0},
							swingDuration = 0.15,
							delayDuration = 0.0
						},

						{
							angle = 45.0,
							movingPos = {0, 0},
							swingDuration = 0.15,
							delayDuration = 0.0
						}

          },
          entityMovingData = {
            speedIdentifier = 1.0
                        
          },
					staminaUsage = 20,
					damage = 6,
					range = 60,
					thickness = 20,
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.1,
          recoveryDuration = 0.5

				}

			}

		}    
 
	}


}
--[[
this is for instance calling type
 function Account:withdraw (v)
      self.balance = self.balance - v
    end
]]--



Test = {
	printVal = function(self)

		print(self.value)

	end,

	value = "hehe"
}



--[[function Test:printVal()
		print(self.value)
end]]--


function Test:new(o)
	o = o or {}
	setmetatable(o, self)

	self.__index = self
	return o
end

test1 = Test:new{}
--test1.printVal(test1)
--print(test1.value)


LuaRef:testIndex(test1)
--[[Account = {
	States = {
		{
			prototype = {
				new = function (o)


				end,
				test = function ()
					print("test1")
					--flippingPage = 0
					--Account.curState = Account.States[2]
				end,
				num
			}

		},

		{
			test = function()
				print("test2")
				--Account.curState = Account.States[1]
			end,
			num = 1

		}


	},
	TopFunc = function()
		print("Account func")
	end
	,
	curState = 1

}

print(Account.States[1])
--Account.States[1].prototype{ num = 10}
Account.States[1].mt = {}



function Account:new(o)
	o = o or {}
	setmetatable(o, self)

	self.curState = self
	return o
end

test = Account:new{curState = 100}
--test
print(test.curState)

num = Account.curState
print(num)
]]--


--[[Window = {}

Window.prototype = {x = 0, y= 0, width = 100, height = 50}
Window.mt = {}

function Window.new(o)
	setmetatable(o, Window.mt)
	return o
end

Window.mt.__index = function (table, key)
      return Window.prototype[key]
end


w = Window.new{ z = 0}
print(w.k)]]--

--[[Account.curState = Account.States:new()
Account.curState.num = 3
print(Account.curState.num)
Account.curState = Account.States[2]
Account.curState.num = 10
print(Account.curState.num)
Account.curState = Account.States[1]
print(Account.curState.num)
]]--


--print(TextureDir)

