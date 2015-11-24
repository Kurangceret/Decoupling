dofile ("Scripts/ConstantScript.lua")
dofile("Scripts/CategoryScript.lua")


frameSize = {48, 12}
boundingBoxSize = {0, 0, frameSize[1], frameSize[2]}

SoulHook = {
  initializeData = {
		SpriteComponent = {
			textureLocation = TextureDir .. "soul_hook.png",
			textureRect = {0, 0, frameSize[1], frameSize[2]}

		},

		TransformableComponent = {
			positionX = 0,
			positionY = 0,
			centerOriginX = boundingBoxSize[1] + (boundingBoxSize[3] / 2),
			centerOriginY = boundingBoxSize[2] + (boundingBoxSize[4] / 2)
		},
    
    RotatedBoxCollisionComponent = {
       --first two is the top left position if aligned like a normal bounding rect
      rotatedRect = {0, 0, frameSize[1], frameSize[2]},
      collisionReactor = function(thisEntity, collidedEntity, systemCollision)
        if(collidedEntity:compCategory:getCategory() & Category.Tile ~= 0)  then
          return
        end
        soulHookStruckEvent = Event.SoulHookStruckEvent()
        soulHookStruckEvent.mHookLatestPos = thisEntity:compTransform():getWorldPosition(true)
        soulHookStruckEvent.mCollidedEntityCategory = collidedEntity:compCategory():getCategory()
        EventManager.getInstance():queueEvent(soulHookStruckEvent)
      end
      
      
    },
    TimerComponent = {
      timerList = {
        {
          name = "Hooking_Time",
          elapsedTime = 0.0,
          timeLimit = 1.0
        }
      }
      
    },
    ScriptUpdateComponent = {
      memberVariables = {
        
      },
        
      updateFunc = function(thisEntity, dt, memberVariables)
        timerComp = thisEntity:compTimer()
        if(timerComp:isTimerElapsed("Hooking_Time")) then
          thisEntity:compVelocity():setVelocity(0.0, 0.0)
          thisEntity:compHealth():damage(1, thisEntity)
          return
        end
        timerComp:updateTimer("Hooking_Time", dt)
        
      end
      
    },

		--[[BoxCollisionComponent = {
			floatRect = {0, 0, boundingBoxSize[3], boundingBoxSize[4]},
      collisionReactor = function(thisEntity, collidedEntity, systemCollision)
        collidedCategoryComp = collidedEntity:compCategory()
        --thisTransformComp = thisEntity:compTransform()
        
       
        
      end
		},]]--

		VelocityComponent = {
			speed = 400,
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
      category = Category.SoulHook
      
    },
    TextDisplayComponent = {
      
    }--[[, 
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
    }]]--

	}
  
  
}