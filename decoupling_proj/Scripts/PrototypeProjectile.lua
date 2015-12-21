dofile ("Scripts/ConstantScript.lua")
dofile("Scripts/CategoryScript.lua")


frameSize = {16, 16}
boundingBoxSize = {0, 0, frameSize[1], frameSize[2]}

PrototypeProjectile = {
  initializeData = {
		SpriteComponent = {
			textureLocation = TextureDir .. "prototype_projectile.png",
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
        --thisTransformComp = thisEntity:compTransform()
        
        if(collidedCategoryComp:getCategory() & Category.Projectile ~= 0 or
          collidedCategoryComp:getCategory() & Category.Enemy ~= 0) then
          return
        end
        
        thisEntity:compHealth():damage(1, collidedEntity)
        
      end
		},

		VelocityComponent = {
			speed = 240,
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
            
            
          end,
          damage = 5,
          attackCategory = Category.Player
        },
      }
    },
    --[[FloatableComponent = {
      isFloating = true
      
    },]]--
    EntityExpertiseComponent = {
      ableToFloat = true,
      ableToWalk = true
      
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