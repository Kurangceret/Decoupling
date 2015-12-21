dofile ("Scripts/ConstantScript.lua")
dofile("Scripts/CategoryScript.lua")

frameSize = {62, 78}
boundingBoxSize = {0, 54, frameSize[1], frameSize[2] - 54}


Fireman = {
  initializeData = {
		SpriteComponent = {
			textureLocation = TextureDir .. "fireman.png",
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
			speed = 160,
			sprintSpeed = 110

    },
    HealthComponent = {
			maxHealth = 15,
			currentHealth = 15 
		},
    EntityExpertiseComponent = {
      ableToFloat = true,
      ableToWalk = true
      
    },
		AutomaticPathComponent = {

		},
		ScriptAIComponent = {
			AIScriptDir = AIScriptDir .. "FiremanAIScript.lua",
      tableName = "Fireman"

    },
    CategoryComponent = {
      category = Category.Fireman
      
    },
    AvoidanceBoxComponent = {
      avoidCategory = Category.Enemy
      
    },
    
    TextDisplayComponent = {
      
    },
    BuffableComponent = {
      
      
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
      
    },
    MeleeRectComponent = {
      attackCategory = Category.Player,
      movementSetList = {
        {
					name = "First_Fist_Attack",
					slashData = {
						{
							angle = -35.0,
							movingPos = {0, 0},
							swingDuration = 0.45,
							delayDuration = 0.45
						},
            {
							angle = 0.0,
							movingPos = {10, 10},
							swingDuration = 0.13,
							delayDuration = 0.0
						},
            {
							angle = 35.0,
							movingPos = {10, 10},
							swingDuration = 0.13,
							delayDuration = 0.0
						}

          },
          entityMovingData = {
            speedIdentifier = 0.7
                        
          },
					staminaUsage = 0,
					damage = 10,
					range = 35,
					thickness = 30,
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.5,
          recoveryDuration = 0.3

        },
        {
					name = "Second_Fist_Attack",
					slashData = {
						{
							angle = 60.0,
							movingPos = {0, 0},
							swingDuration = 0.35,
							delayDuration = 0.35
						},
            {
							angle = 0.0,
							movingPos = {0, 0},
							swingDuration = 0.18,
							delayDuration = 0.0
						},
            {
							angle = -60.0,
							movingPos = {0, 0},
							swingDuration = 0.18,
							delayDuration = 0.0
						}

          },
          entityMovingData = {
            speedIdentifier = 0.7
                        
          },
					staminaUsage = 0,
					damage = 15,
					range = 40,
					thickness = 30,
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.4,
          recoveryDuration = 0.3

        }
        
      },
      collisionReactor = function(thisEntity, collidedEntity, systemCollision, selfAIState)
                
      end
      
    }

	}
  
  
  
}


