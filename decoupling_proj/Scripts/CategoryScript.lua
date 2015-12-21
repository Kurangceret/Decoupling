

--[[Category = {
  None = 0,
  VertexNode = 1 << 1,
  Layer = 1 << 2,
  Player = 1 << 3,
  Tile = 1 << 4,
  Spider = 1 << 5,
  FireLight = 1 << 6,
  Sprite = 1 << 7,
  SwampMoa = 1 << 8,
  SkeletonArcher = 1 << 9,
  Enemy = Category.Spider | Category.SwampMoa | Category.SkeletonArcher,
  SolidMovingEntity = Category.Enemy | Category.Player,
  SolidEntity = Category.Tile | Category.SolidMovingEntity,
  Test = Category.SolidEntity
  
}--]]
Category = {}
Category.None = 0
Category.VertexNode = 1 << 1
Category.Layer = 1 << 2
Category.Player = 1 << 3
Category.Tile = 1 << 4
Category.Spider = 1 << 5
Category.FireLight = 1 << 6
Category.Sprite = 1 << 7
Category.SwampMoa = 1 << 8
Category.SkeletonArcher = 1 << 9
Category.Projectile = 1 << 10
Category.HealthSpirit = 1 << 11
Category.SoulHook = 1 << 12
Category.PlayerElementalShield = 1 << 13
Category.Fireman = 1 << 14
Category.PlayerAlly = Category.Player | Category.PlayerElementalShield
Category.Enemy = Category.Spider | Category.SwampMoa | Category.SkeletonArcher | Category.Fireman
Category.SolidMovingEntity = Category.Enemy | Category.Player | Category.Projectile | Category.HealthSpirit | Category.SoulHook
Category.SolidEntity = Category.Tile | Category.SolidMovingEntity


--print(Category.Enemy & Category.Layer)

--[[CombinedCategory = {
  SolidMovingEntity = Category.Spider | Category.Player |  Category.SwampMoa,
  Enemy = Category.Spider | Category.SwampMoa,
  SolidEntity = Category.Spider | Category.Player | Category.Tile | Category.SwampMoa
  
}]]


--print(CombinedCategory.MoveableEntity)

--print(Category.Player & Category.Spider)
--[[a = Category.None
a = a | Category.VertexNode
print(a)
a = a | Category.Layer
print(a)
a = a | Category.Player
print(a)
print(a & Category.VertexNode)
print(a & Category.Tile)
print(a & Category.Spider)

a = a ~ Category.Layer
print(a)
print(Category.VertexNode | Category.Player | Category.None)

a = a | Category.Sprite
print(a)
print(a & Category.Circle)]]--