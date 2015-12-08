#pragma once

#include <memory>
#include <SFML/System/Time.hpp>
#include <vector>
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>

class Entity;
class BuffScript{
public:
	typedef std::unique_ptr<luabridge::LuaRef> LuaRefPtr;
	typedef std::unique_ptr<BuffScript> Ptr;
public:
	BuffScript(LuaRefPtr luaRef);
	~BuffScript();

	void enterBuff(Entity* ownerEntity, sf::Time dt);
	void updateBuff(Entity* ownerEntity, sf::Time dt);
	void exitBuff(Entity* ownerEntity, sf::Time dt);

	bool isBuffOver(Entity* ownerEntity) const;

	std::string getBuffName() const;

	bool isBuffTimerOver() const;

	void absoluteDestroy();

	luabridge::LuaRef getLuaReferenceToBuff();
private:
	LuaRefPtr mLuaReferenceToBuff;

	sf::Time mBuffTimer;
	//if set to true, this buff will automatically be destroyed if mBuffTimer
	//reaches less or equal than zero second. the isBuffOver lua boolean function
	//can still be uses as an alternative to destroying this buff
	bool mUseBuffTimer;
	std::string mBuffName;
	//if absoluteDestroy is true, this buff will be removed regardless of whatever condition
	bool mAbsoluteDestroy;
};

