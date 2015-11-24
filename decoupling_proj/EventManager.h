#pragma once
#include <queue>
#include "EventBase.h"
#include <functional>
#include <unordered_map>
#include <cstdint>
#include "Utility.h"
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
};
#include <LuaBridge.h>
#include <RefCountedPtr.h>

class Entity;

struct EventCallBackFunc{
	typedef std::unique_ptr<EventCallBackFunc> Ptr;
	typedef std::function<void(EventBase*)> EventFunction;

	EventCallBackFunc(std::uintptr_t address, const EventFunction& func)
	:callBackFunc(func), ownerAddress(address){}

	EventFunction callBackFunc; //object function which called the function assigned by outsiders(entity/gui/state..etc)
	std::uintptr_t ownerAddress; //holds the owner of this callback func address
};
//exclusive for entity usage
struct LuaEventCallBackFunc{
	typedef std::unique_ptr<LuaEventCallBackFunc> Ptr;
	LuaEventCallBackFunc(Entity* entity, const luabridge::LuaRef& callBack);
	//:mLuaCallBack(std::make_unique<luabridge::LuaRef>(func)), ownerEntity(entity){}

	const luabridge::LuaRef mLuaCallBack;
	Entity* ownerEntity;
};

class EventManager{
public:
	//typedef std::function<void(EventBase*)> EventCallBackFunc;
public:
	EventManager();
	~EventManager();

	void queueScriptEvent(EventBase* eventBase);

	void queueEvent(EventBase::Ptr eventBase);

	//template<class T>
	void addListener(EventType::ID eventId, const EventCallBackFunc&);

	template<class T>
	void deleteListener(EventType::ID eventId, T* ownerPointer);
		
	template<class T>
	void deleteListener(T* ownerPointer);


	void addLuaListener(Entity* thisEntity, const std::string& eventName, 
		lua_State* luaState);

	void processQueuedEvents();

	static EventManager* getInstance();

	void cleanEverything();
private:
	EventBase* getFrontEventQueue() const;
	void popEventQueue();

	void deriveCallToEvent(const luabridge::LuaRef& func, Entity* ownerEntity,
		EventBase* eventBase);
	
	template <class T>
	void registerEventForLuaScript(EventType::ID eventType);
private:
	std::queue<EventBase::Ptr> mQueueEvents;
	//std::queue<RefCountedPtr<EventBase>> mQueueEvents;
	std::unordered_map<EventType::ID, std::vector<EventCallBackFunc::Ptr> > mListeners;

	std::unordered_map<EventType::ID, std::vector<LuaEventCallBackFunc::Ptr>> mLuaListeners;

	static EventManager* mEventManager;
	std::unordered_map<std::string, EventType::ID> mEventsTypeDictionary;
	std::unordered_map<EventType::ID, std::function<EventBase::Ptr(EventBase&)>> mDeriverEventCaller;
};

template <class T>
void EventManager::registerEventForLuaScript(EventType::ID eventType)
{
	mDeriverEventCaller[eventType] = [&](EventBase& eventBase)
	{
		return std::move(EventBase::Ptr(new T(dynamic_cast<T&>(eventBase))));
	};
}

template<class T>
void EventManager::deleteListener(EventType::ID eventId, T* ownerPointer)
{
	auto findIter = mListeners.find(eventId);
	if (findIter != mListeners.end()){
		
		std::vector<EventCallBackFunc>& listOfCallBack = findIter->second;
		std::vector<EventCallBackFunc>::iterator iterC;

		std::uintptr_t address = Utility::convertPointerToAddress(ownerPointer);
		for (iterC = listOfCallBack.begin(); iterC != listOfCallBack.end())
		{
			if (iterC->ownerAddress == address)
			{
				iterC = listOfCallBack.erase(iterC);
				continue;
			}
			iterC++;
		}
	}
	auto luaFindIter = mLuaListeners.find(eventId);
	if (luaFindIter != mLuaListeners.end()){
		std::vector<LuaEventCallBackFunc>& listOfCallBack = luaFindIter->second;
		std::vector<LuaEventCallBackFunc>::iterator iterC;

		for (iterC = listOfCallBack.begin(); iterC != listOfCallBack.end())
		{
			if (iterC->ownerEntity == ownerPointer)
			{
				iterC = listOfCallBack.erase(iterC);
				continue;
			}
			iterC++;
		}
	}
}

template<class T>
void EventManager::deleteListener(T* ownerPointer)
{
	std::uintptr_t address = Utility::convertPointerToAddress(ownerPointer);

	for (auto& mapIter : mListeners){
		std::vector<EventCallBackFunc>& listOfCallBack = mapIter.second;
		std::vector<EventCallBackFunc>::iterator iterC;

		for (iterC = listOfCallBack.begin(); iterC != listOfCallBack.end())
		{
			if (iterC->ownerAddress == address)
			{
				iterC = listOfCallBack.erase(iterC);
				continue;
			}
			iterC++;
		}
	}
}