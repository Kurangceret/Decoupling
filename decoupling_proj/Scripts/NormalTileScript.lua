
dofile("Scripts/CategoryScript.lua")


NormalTile = {
  initializeData = {
    BoxCollisionComponent = {
      collisionReactor =  function(thisEntity, collidedEntity, systemCollision)
          collidedCategory = collidedEntity:compCategory()
          collidedTransform = collidedEntity:compTransform()
          
          if(collidedCategory:getCategory() 
            & Category.SolidMovingEntity ~= 0) then
            vector = systemCollision:getCalculatedSafeOffSet(collidedEntity)
            collidedTransform:move(vector.x, vector.y)
          end
        
        end
        
    }
      
  }
    
}
    
  
