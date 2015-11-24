#pragma once
#include "Component.h"
#include <functional>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>

class Entity;
class CollisionHandlerSystem;
class ComponentArranger;

class CollisionComponent :	public Component{
public:
	friend CollisionHandlerSystem;
	friend ComponentArranger;
	typedef std::function<void(Entity*, Entity*, CollisionHandlerSystem*)> CollisionReactor;
public:
	CollisionComponent(Entity* ownerEntity, CollisionReactor collisionReactor = CollisionReactor());
	virtual ~CollisionComponent();

	void setScriptCollisionReactor(const luabridge::LuaRef& luaFunction);
	void setCollisionReactor(const CollisionReactor& collisionReactor);
	virtual void callCollisionReactor(Entity* thisEntity, 
		Entity* collidedEntity, CollisionHandlerSystem* system);
private:
	CollisionReactor mCollisionReactor;
	std::unique_ptr<luabridge::LuaRef> mLuaCollisionReactor;
};

