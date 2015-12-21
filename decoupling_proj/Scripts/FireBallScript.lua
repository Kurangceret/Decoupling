dofile ("Scripts/ConstantScript.lua")
dofile("Scripts/CategoryScript.lua")


frameSize = {32, 32}
boundingBoxSize = {0, 0, frameSize[1], frameSize[2]}

FireBall = {
  initializeData = {
		SpriteComponent = {
			textureLocation = TextureDir .. "fireball.png",
			textureRect = {0, 0, frameSize[1], frameSize[2]}

		},

		TransformableComponent = {
			positionX = 0,
			positionY = 0,
			centerOriginX = boundingBoxSize[1] + (boundingBoxSize[3] / 2),
			centerOriginY = boundingBoxSize[2] + (boundingBoxSize[4] / 2)
		},

		BoxCollisionComponent = {
			floatRect = {0, 0, boundingBoxSize[3], boundingBoxSize[4]},
      collisionReactor = function(thisEntity, collidedEntity, systemCollision)
        collidedCategoryComp = collidedEntity:compCategory()
        --thisTransformComp = thisEntity:compTransform()
        
        --[[if(collidedCategoryComp:getCategory() & Category.Projectile ~= 0 or
          collidedCategoryComp:getCategory() & Category.Player ~= 0) then
          return
        end]]--
        if(collidedCategoryComp:getCategory() & Category.Tile ~= 0 ) then
          thisEntity:compHealth():damage(1, collidedEntity)
        end
        
      end
		},
    ScriptUpdateComponent = {
      memberVariables = {
        
      },
        
      updateFunc = function(thisEntity, dt, memberVariables)
        timerComp = thisEntity:compTimer()
        timerComp:updateTimer("FireBall_Time", dt)
        
      end
      
    },
		VelocityComponent = {
			speed = 300,
			sprintSpeed = 110

    },
    HealthComponent = {
			maxHealth = 1,
			currentHealth = 1
		},
    TimerComponent = {
      timerList = {
        {
          name = "FireBall_Time",
          elapsedTime = 0.0,
          timeLimit = 1.0
        }
      }
      
    },
		--[[ScriptAIComponent = {
			AIScriptDir = AIScriptDir .. "SpiderAIScript3.lua",
      tableName = "Spider"

    },]]--
    CategoryComponent = {
      category = Category.Projectile
      
    },
    TextDisplayComponent = {
      
    }, 
    HarmfulBoxesComponent = {
      boxesList = {
        {
          name = "Body",
          activated = true,
          floatRect = {0, 0, frameSize[1], frameSize[2]},
          collisionReactor = function(thisEntity, collidedEntity, systemCollision, harmfulBoxData, selfAIState)
            if(collidedEntity:compCategory():getCategory() & harmfulBoxData.mAttackCategory ~= 0) then
              thisEntity:compHealth():damage(1, collidedEntity)
            end
          end,
          damage = 3,
          attackCategory = Category.Enemy
        },
      }
    },
    
    EntityExpertiseComponent = {
      ableToFloat = true,
      ableToWalk = true
      
    },
    DestroyableComponent = {
      isDestroyed = function(thisEntity)
        timerComp = thisEntity:compTimer()
        if(thisEntity:compHealth():getCurrentHealth() <= 0.0 or
          timerComp:isTimerElapsed("FireBall_Time")) then
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