

Utility = {
  
  deltaToSecond = function(delta) 
    return delta / 1000000
  end,
  
  getEightDirections = function()
    --[[directions = {}
    directions[0] = sfVector2f(1.0, 0.0)
    directions[1] = sfVector2f(1.0, -1.0)
    directions[2] = sfVector2f(0.0, -1.0)
    directions[3] = sfVector2f(-1.0, -1.0)
    directions[4] = sfVector2f(-1.0, 0.0)
    directions[5] = sfVector2f(-1.0, 1.0)
    directions[6] = sfVector2f(0.0, 1.0)
    directions[7] = sfVector2f(1.0, 1.0)]]--
    --[[directions = {
      sfVector2f(1.0, 0.0),
      sfVector2f(1.0, -1.0),
      sfVector2f(0.0, -1.0),
      sfVector2f(-1.0, -1.0),
      sfVector2f(-1.0, 0.0),
      sfVector2f(-1.0, 1.0),
      sfVector2f(0.0, 1.0),
      sfVector2f(1.0, 1.0)
    }]]--
    directions = {
      sfVector2f(),
      sfVector2f(),
      sfVector2f(),
      sfVector2f(),
      sfVector2f(),
      sfVector2f(),
      sfVector2f(),
      sfVector2f()
    }
    directions[1].x = 1.0
    directions[1].y =0.0
    directions[2].x = 1.0
    directions[2].y = -1.0
    directions[3].x = 0.0
    directions[3].y = -1.0
    directions[4].x = -1.0
    directions[4].y =  -1.0
    directions[5].x = -1.0
    directions[5].y = 0.0
    directions[6].x = -1.0
    directions[6].y = 1.0
    directions[7].x = 0.0
    directions[7].y = 1.0
    directions[8].x = 1.0
    directions[8].y = 1.0
    
    return directions
  end,
  
  getRandomizedEightDirections = function()
    newEightDirections = Utility.getEightDirections()
    --newEightDirections = {}
    
    for i = 1, 8 do
      nextI = EngineUtil.randomRange(1, 9)
      if(newEightDirections[nextI] ~= nil) then
        temp = newEightDirections[nextI]
        newEightDirections[nextI] = newEightDirections[i]
        newEightDirections[i] = temp
      else
        newEightDirections[i] = newEightDirections[nextI]
      end
    end
    
    return newEightDirections
  end
  
  
}

