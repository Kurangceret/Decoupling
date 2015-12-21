#include "PlayerState.h"


PlayerState::PlayerState(Entity* entityPlayer, const luabridge::LuaRef& playerStateTable)
:mPlayer(entityPlayer),
mPlayerStateTable(playerStateTable)
{
}


PlayerState::~PlayerState()
{
}


std::string PlayerState::getLuaTableName() const
{
	return "";
}

bool PlayerState::isStaminaCompEnough(StaminaComponent* staminaComp)
{
	return true;
}

bool PlayerState::isStateAvailable()
{
	return true;
}