#include "CollisionComponent.h"
#include "ScriptAIComponent.h"
#include "Entity.h"
#include <iostream>

CollisionComponent::CollisionComponent(Entity* entity, CollisionReactor collisionReactor)
:Component(entity),
mCollisionReactor(collisionReactor),
mLuaCollisionReactor(nullptr)
{
	mIdentifier = ComponentIdentifier::CollisionComponent;
	mCollisionReactor = [&](Entity*, Entity* entity, CollisionHandlerSystem* system){};
}


CollisionComponent::~CollisionComponent()
{
}


void CollisionComponent::callCollisionReactor(Entity* thisEntity,
	Entity* collidedEntity, CollisionHandlerSystem* system)
{
	mCollisionReactor(thisEntity, collidedEntity, system);
	if (mLuaCollisionReactor.get()){
		try{
			ScriptAIComponent* ownerAIComp = mOwnerEntity->nonCreateComp<ScriptAIComponent>();
			if (ownerAIComp && ownerAIComp->getCurAIState()){
				(*mLuaCollisionReactor)(thisEntity, collidedEntity, system,
					ownerAIComp->getCurAIState()->getLuaReferenceToState());
			}
			else{
				(*mLuaCollisionReactor)(thisEntity, collidedEntity, system);
			}
		}
		catch (luabridge::LuaException const& e) {
			std::cerr << e.what() << std::endl;
		}
	}
}


void CollisionComponent::setCollisionReactor(const CollisionReactor& collisionReactor)
{
	mCollisionReactor = collisionReactor;
}

void CollisionComponent::setScriptCollisionReactor(const luabridge::LuaRef& luaFunction)
{
	mLuaCollisionReactor = std::make_unique<luabridge::LuaRef>(luaFunction);
}