dofile ("Scripts/ConstantScript.lua")
dofile("Scripts/CategoryScript.lua")
--dofile("Scripts/AIScripts/SpiderAIScript2.lua")

frameSize = {47, 30}


quickHeadAttackSwingDur = 0.7
quickHeadAttackDelayDur = 0.3
quickHeadAttackRange = 20.0

boundingBoxSize = {0, frameSize[2] * 0.5, frameSize[1], frameSize[2] * 0.5}
Spider = {
	initializeData = {
		SpriteComponent = {
			textureLocation = TextureDir .. "spider_side.png",
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
			speed = 80,
			sprintSpeed = 110

    },
    HealthComponent = {
			maxHealth = 10,
			currentHealth = 10
		},
		AutomaticPathComponent = {

    },
    BuffableComponent = {
      
      
    },
		ScriptAIComponent = {
			AIScriptDir = AIScriptDir .. "SpiderAIScript3.lua",
      tableName = "Spider"

    },
    CategoryComponent = {
      category = Category.Spider
      
    },
    AvoidanceBoxComponent = {
      avoidCategory = Category.Enemy
      
    },
    EntityExpertiseComponent = {
      ableToFloat = false,
      ableToWalk = true
    },
    MeleeRectComponent = {
      attackCategory = Category.Player,
      movementSetList = {
        {
					name = "Quick_Head_Attack",
					slashData = {
						{
							angle = 0.0,
							movingPos = {-10, -10},
							swingDuration = quickHeadAttackDelayDur + 0.01,
							delayDuration = quickHeadAttackDelayDur
						},
            {
							angle = 0.0,
							movingPos = {10, 10},
							swingDuration = quickHeadAttackSwingDur - quickHeadAttackDelayDur,
							delayDuration = 0.0
						}--[[,

						{
							angle = 1.0,
							movingPos = {0, 0},
							swingDuration = 0.01,
							delayDuration = 0.0
						}]]--

					},
					staminaUsage = 0,
					damage = 10,
					range = 20,
					thickness = 25,
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.25,
          recoveryDuration = 0.3

        },
        {
					name = "Charge_Attack",
					slashData = {
						{
							angle = 0.0,
							movingPos = {0, (-frameSize[2] / 2)},
							swingDuration = 0.6,
							delayDuration = 0.0
						},
            {
							angle = 0.0,
							movingPos = {0, (-frameSize[2] / 2) },
							swingDuration = 0.01,
							delayDuration = 0.0
						}

					},
					staminaUsage = 0,
					damage = 10,
					range = frameSize[2] / 2,
					thickness = frameSize[1],
					wholeDelayDuration = 0,
					wholeSwingDuration = 0.6,
          vulnerableDuration = 0.25,
          recoveryDuration = 0.3

        }
        
      },
      collisionReactor = function(thisEntity, collidedEntity, systemCollision, selfAIState)
        
        
        --scriptAIComp = thisEntity:compScriptAI()
        
        if(selfAIState ~= nil and selfAIState.name == "AttackPlayerState") then
           -- print(scriptAIComp:getCurLuaState():test)
            --t = scriptAIComp:getCurAIState()
            --print(selfAIState.curIteration)
            --print(scriptAIComp:getCurAIState():getLuaStateRef())
            --print(scriptAIComp:getCurAIState():getLuaState(Spider.initializeData.MeleeRectComponent.collisionReactor))
        end
        
      end
      
    },
    TextDisplayComponent = {
      
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

