dofile ("Scripts/ConstantScript.lua")
dofile("Scripts/CategoryScript.lua")


frameSize = {64, 40}
boundingBoxSize = {0, frameSize[2] * 0.5, frameSize[1], frameSize[2] * 0.5}

tailAttackRange = 52
tailAttackDuration = 1.2
tailAttackDelayDuration = 1.0



SwampMoa = {
  
  initializeData = {
		SpriteComponent = {
			textureLocation = TextureDir .. "SwampMoa.png",
			textureRect = {0, 0, frameSize[1], frameSize[2]}

		},

		TransformableComponent = {
			positionX = 350,
			positionY = 150,
			centerOriginX = boundingBoxSize[1] + (boundingBoxSize[3] / 2),
			centerOriginY = boundingBoxSize[2] + (boundingBoxSize[4] / 2)
		},

		BoxCollisionComponent = {
			floatRect = {0, 0, boundingBoxSize[3], boundingBoxSize[4]},
      collisionReactor = function(thisEntity, collidedEntity, systemCollision)
        collidedCategoryComp = collidedEntity:compCategory()
        thisTransformComp = thisEntity:compTransform()
        
        if(collidedCategoryComp:getCategory() & Category.Player ~= 0 or
          collidedCategoryComp:getCategory() & Category.Enemy ~= 0) then
           vector = systemCollision:getCalculatedSafeOffSet(thisEntity)
           thisTransformComp:move(vector.x, vector.y)
        end
        
        
      end
		},

		VelocityComponent = {
			speed = 120,
			sprintSpeed = 110

    },
    HealthComponent = {
			maxHealth = 20,
			currentHealth = 20
		},
		AutomaticPathComponent = {

		},
		ScriptAIComponent = {
			AIScriptDir = AIScriptDir .. "SwampMoaAIScript.lua",
      tableName = "SwampMoa"

    },
    CategoryComponent = {
      category = Category.SwampMoa
      
    },
    AvoidanceBoxComponent = {
      avoidCategory = Category.Enemy
      
    },
    MeleeRectComponent = {
      attackCategory = Category.Player,
      movementSetList = {
        {
					name = "Tail_Attack",
					slashData = {
						{
							angle = 0.0,
							movingPos = {-10, -10},
							swingDuration = tailAttackDelayDuration,
							delayDuration = tailAttackDelayDuration
						},
            {
							angle = 0.0,
							movingPos = {10, 10},
							swingDuration = tailAttackDuration - tailAttackDelayDuration,
							delayDuration = 0.0
						}

          },
          entityMovingData = {
            speedIdentifier = 0.7
                        
          },
					staminaUsage = 0,
					damage = 15,
					range = tailAttackRange,
					thickness = 30,
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.25,
          recoveryDuration = 0.3

        },
        {
					name = "Wide_Tail_Attack",
					slashData = {
						{
							angle = 55.0,
							movingPos = {-10, -10},
							swingDuration = tailAttackDelayDuration,
							delayDuration = tailAttackDelayDuration
						},
            {
							angle = 0.0,
							movingPos = {0, 0},
							swingDuration = 0.15,
							delayDuration = 0.0
						},
            {
              angle = -55.0,
							movingPos = {10, 10},
							swingDuration = 0.15,
							delayDuration = 0.0
            }

          },
          entityMovingData = {
            speedIdentifier = 0.5
                        
          },
					staminaUsage = 0,
					damage = 15,
					range = tailAttackRange,
					thickness = 30,
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.25,
          recoveryDuration = 0.3

        }
      --[[,
        {
					name = "Steaming_Head",
					slashData = {
						{
							angle = 0.0,
							movingPos = {-10, -10},
							swingDuration = tailAttackDelayDuration,
							delayDuration = tailAttackDelayDuration
						},
            {
							angle = 0.0,
							movingPos = {10, 10},
							swingDuration = tailAttackDuration - tailAttackDelayDuration,
							delayDuration = 0.0
						}

					},
					staminaUsage = 0,
					damage = 10,
					range = 25,
					thickness = 30,
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.25,
          recoveryDuration = 0.3

        }]]--
        
      },
      collisionReactor = function(thisEntity, collidedEntity, systemCollision, selfAIState)
       
        
      end
      
    },
    TextDisplayComponent = {
      
    },
    SpiritGrabberComponent = {
      
      
    },
    HarmfulBoxesComponent = {
      boxesList = {
        {
          name = "Steaming_Head",
          activated = false,
          floatRect = {6, 0, 32, 24},
          collisionReactor = function(thisEntity, collidedEntity, systemCollision, harmfulBoxData, selfAIState)
            
            
          end,
          damage = 5,
          attackCategory = Category.Player
        },
        {
          name = "Exploding_Head",
          activated = false,
          --floatRect = {-10, -16, 64, 62},
          floatRect = {-20, -16, 104, 82},
          collisionReactor = function(thisEntity, collidedEntity, systemCollision, harmfulBoxData, selfAIState)
            
            
          end,
          damage = 10,
          attackCategory = Category.Player
        }
              
      }
      
    },
    DestroyableComponent = {
      isDestroyed = function(thisEntity) 
        if(thisEntity:compHealth():getCurrentHealth() <= 0.0) then
          return true
        end
        return false
        
      end,
      isRemoveable = function(thisEntity)
        return thisEntity:compDestroyable():isDestroyed()
      end
      
    }


	}
  
  
}
