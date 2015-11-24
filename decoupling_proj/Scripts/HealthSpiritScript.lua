dofile ("Scripts/ConstantScript.lua")
dofile("Scripts/CategoryScript.lua")


frameSize = {8, 8}
boundingBoxSize = {0, 0, frameSize[1], frameSize[2]}

HealthSpirit = {
  initializeData = {
		SpriteComponent = {
			textureLocation = TextureDir .. "health_spirit.png",
			textureRect = {0, 0, frameSize[1], frameSize[2]}

		},

		TransformableComponent = {
			positionX = 0,
			positionY = 0,
			centerOriginX = boundingBoxSize[1] + (boundingBoxSize[3] / 2),
			centerOriginY = boundingBoxSize[2] + (boundingBoxSize[4] / 2)
		},
    ScriptUpdateComponent = {
      memberVariables = {
        mChargingTime = 0.5,
        mElapsedTime = 0.0
      },
      updateFunc = function(thisEntity, dt, memberVariables)
        --print(thisEntity:compScriptUpdate():getMemberVariables().mElapsedTime)
        if(memberVariables.mChargingTime <= memberVariables.mElapsedTime) then
            thisEntity:compVelocity():setVelocity(0, 0)
            return
        end
        
        memberVariables.mElapsedTime = memberVariables.mElapsedTime + dt
        
        
        
      end
      
    },
		BoxCollisionComponent = {
			floatRect = {0, 0, boundingBoxSize[3], boundingBoxSize[4]},
      collisionReactor = function(thisEntity, collidedEntity, systemCollision)
        collidedCategoryComp = collidedEntity:compCategory()
        --thisTransformComp = thisEntity:compTransform()
        
        
        if(thisEntity:compHealth():getCurrentHealth() <= 0.0) then
          return
        end
        
        if(collidedCategoryComp:getCategory() & Category.Projectile ~= 0 or
          collidedCategoryComp:getCategory() & Category.Enemy ~= 0) then
          return
        end
        
        if(collidedCategoryComp:getCategory() & Category.Tile ~= 0) then
          thisEntity:compVelocity():setVelocity(0.0, 0.0)
        end
        
        if(collidedCategoryComp:getCategory() & Category.Player ~= 0 and
          not collidedEntity:compSpiritForm():isOnSpiritForm()) then
          thisEntity:compHealth():damage(1, collidedEntity)
          collidedEntity:compHealth():increaseHealth(1)
        end
        
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
		--[[ScriptAIComponent = {
			AIScriptDir = AIScriptDir .. "SpiderAIScript3.lua",
      tableName = "Spider"

    },]]--
    CategoryComponent = {
      category = Category.HealthSpirit
      
    },
    TextDisplayComponent = {
      
    },
    DestroyableComponent = {
      isDestroyed = function(thisEntity) 
        
        
      end,
      isRemoveable = function(thisEntity)
        
      end
      
    }

	}
  
  
}