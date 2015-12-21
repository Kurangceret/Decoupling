dofile ("Scripts/ConstantScript.lua")
dofile("Scripts/CategoryScript.lua")

frameSize = {52, 64}
boundingBoxSize = {30, 50, 15, 14}


SkeletonArcher = {
  initializeData = {
		SpriteComponent = {
			textureLocation = TextureDir .. "skeleton_archer.png",
			textureRect = {0, 0, frameSize[1], frameSize[2]}

		},

		TransformableComponent = {
			positionX = 350,
			positionY = 150,
			centerOriginX = boundingBoxSize[1] + (boundingBoxSize[3] / 2),
			centerOriginY = boundingBoxSize[2] + (boundingBoxSize[4] / 2)
		},
    SpiritGrabberComponent = {
      
      
    },
    BuffableComponent = {
      
      
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
			maxHealth = 6,
			currentHealth = 6
		},
		AutomaticPathComponent = {

		},
		ScriptAIComponent = {
			AIScriptDir = AIScriptDir .. "SkeletonArcherAIScript.lua",
      tableName = "SkeletonArcher"

    },
    CategoryComponent = {
      category = Category.SkeletonArcher
      
    },
    AvoidanceBoxComponent = {
      avoidCategory = Category.Enemy
      
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


