dofile ("Scripts/ConstantScript.lua")
dofile ("Scripts/CategoryScript.lua")

frameSize = {32, 32}
boundingBoxSize = {0, 0, frameSize[1], frameSize[2]}
FireSpirit = {
  initializeData = {
    
    SpriteComponent = {
			textureLocation = TextureDir .. "fireballtest.png",
			textureRect = {0, 0, frameSize[1], frameSize[2]}

		},

		TransformableComponent = {
			--positionX = 350,
			--positionY = 150,
			centerOriginX = boundingBoxSize[1] + (boundingBoxSize[3] / 2),
			centerOriginY = boundingBoxSize[2] + (boundingBoxSize[4] / 2)
		},
    
    ScriptUpdateComponent = {
			updateFunc = function(thisEntity, dt)
        lightPointComp = thisEntity:compLightPoint()
        if(lightPointComp.mNoLightForOneFrame == true) then
          lightPointComp:setCurLightRadius(0.0)
          lightPointComp.mNoLightForOneFrame = false
          return
        end
        lightPointComp:setCurLightRadius(50.0)
      end
    },

		BoxCollisionComponent = {
			floatRect = boundingBoxSize,
      collisionReactor = function(thisEntity, collidedEntity, systemCollision)
        
        collidedCategoryComp = collidedEntity:compCategory()
        thisTransformComp = thisEntity:compTransform()
        
        if(collidedCategoryComp:getCategory() & Category.Tile ~= 0) then
                    
          event = Event.FireSpiritCollidedEvent()
          event.mIsClockwise = not thisEntity:compCircularPath().mIsClockwise
          event.mOwnerEntity = thisEntity
          EventManager.getInstance():queueEvent(event)
          event = nil
          collectgarbage()
        end
        
        
      end
    },

		VelocityComponent = {
			speed = 140,
			sprintSpeed = 110

    },
    CircularPathComponent = {
      isRotatingAroundParentPos = true,
      curRadius = 150.0,
      minRadius = 32.0,
      maxRadius = 150.0,
      speed = 150.0,
      clockWise = false
      
    },
    CategoryComponent = {
      category = Category.FireLight
      
    },
    LightPointComponent = {
      intensity = 3.0,
      radius = 50.0,
      size = 10.0,
      spreadAngle = PI * 2.0,
      softSpreadAngle = 0.0,
      bleed = 0.0,
      linearizeFactor = 1.0,
      alwaysUpdate = true,
      maxRadius = 50.0,
      minRadius = 0.0
        
    }
    
  },
  
  fireSpiritCollidedReactor = function(thisEntity, event) 
    --[[circularComp = thisEntity:compCircularPath()
    circularComp.mIsClockwise = thisEvent.mIsClockwise
    circularComp:decreaseCurRadius(10.0)]]--
    if(event.mOwnerEntity.mId == thisEntity.mId) then
      lightPointComp = thisEntity:compLightPoint()
      lightPointComp.mNoLightForOneFrame = true
    end
   
  end,
  
  initializeFunction = function(thisEntity)
    EventManager.getInstance():addLuaListener(thisEntity, 
      "FireSpiritCollidedEvent", FireSpirit.fireSpiritCollidedReactor)
  end
  
}


