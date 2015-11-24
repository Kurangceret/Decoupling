#include "EventManager.h"
#include "FireSpiritCollidedEvent.h"
#include "CreateNewEntityEvent.h"
#include "SoulHookStruckEvent.h"
#include <RefCountedPtr.h>

EventManager* EventManager::mEventManager = nullptr;

template<class T>
static void callFuncCastEventBase(const luabridge::LuaRef& func, Entity* ownerEntity, 
	EventBase* eventBase)
{
	func(ownerEntity, dynamic_cast<T*>(eventBase));
}

//static std::unordered_map<std::string, EventType::ID> eventsTypeDictionary;


//static std::

EventManager::EventManager()
{

	mEventsTypeDictionary.insert(std::make_pair("PlayerHPChangedEvent", EventType::PlayerHPChanged));
	mEventsTypeDictionary.insert(std::make_pair("PlayerStaminaChangedEvent", EventType::PlayerStaminaChanged));
	mEventsTypeDictionary.insert(std::make_pair("FireSpiritCollidedEvent", EventType::FireSpiritCollided));
	mEventsTypeDictionary.insert(std::make_pair("CreateNewEntityEvent", EventType::CreateNewEntity));
	mEventsTypeDictionary.insert(std::make_pair("SoulHookStruckEvent", EventType::SoulHookStruck));

	registerEventForLuaScript<FireSpiritCollidedEvent>(EventType::FireSpiritCollided);
	registerEventForLuaScript<CreateNewEntityEvent>(EventType::CreateNewEntity);
	registerEventForLuaScript<SoulHookStruckEvent>(EventType::SoulHookStruck);
}


EventManager::~EventManager()
{
}

EventManager* EventManager::getInstance()
{
	if (!mEventManager)
		mEventManager = new EventManager();

	return mEventManager;
}

void EventManager::queueScriptEvent(EventBase* eventBase)
{
	auto& iter = mDeriverEventCaller.find(eventBase->getEventType());
	if (iter == mDeriverEventCaller.end())
		return;
		
	mQueueEvents.push(std::move(iter->second(*eventBase)));
}

void EventManager::queueEvent(EventBase::Ptr eventBase)
{
	mQueueEvents.push(std::move(eventBase));
}

void EventManager::cleanEverything()
{
	std::queue < EventBase::Ptr > emptyQ;
	mQueueEvents = std::move(emptyQ);
	

	mListeners.clear();
	
	mLuaListeners.clear();
}

void EventManager::addListener(EventType::ID eventId, const EventCallBackFunc& eventCallBackFunc)
{
	if (mListeners.find(eventId) == mListeners.end())
		mListeners.insert(std::make_pair(eventId, std::vector<EventCallBackFunc::Ptr>()));
		
	mListeners[eventId].push_back(std::make_unique<EventCallBackFunc>(eventCallBackFunc));
}


void EventManager::processQueuedEvents()
{
	while (!mQueueEvents.empty()){
		EventBase* eventBase = getFrontEventQueue();
		auto listenerIter = mListeners.find(eventBase->getEventType());
		auto luaListenerIter = mLuaListeners.find(eventBase->getEventType());

	

		if (listenerIter != mListeners.end()){
			for (auto& eventStruct : listenerIter->second)
				eventStruct->callBackFunc(eventBase);
		}

		if (luaListenerIter != mLuaListeners.end()){
			for (auto& eventStruct : luaListenerIter->second)
			{
				const luabridge::LuaRef& func = eventStruct->mLuaCallBack;
				try{
					deriveCallToEvent(func, eventStruct->ownerEntity, eventBase);
				}
				catch (luabridge::LuaException const& e) {
					std::cerr << e.what() << std::endl;
				}
			}
			
		}
		mQueueEvents.pop();
	}
}

EventBase* EventManager::getFrontEventQueue() const
{
	return mQueueEvents.front().get();
}

void EventManager::popEventQueue()
{
	mQueueEvents.pop();
}


void EventManager::addLuaListener(Entity* thisEntity, const std::string& eventName,
	lua_State* luaState)
{
	auto dicIter = mEventsTypeDictionary.find(eventName);
	if (dicIter == mEventsTypeDictionary.end())
		return;

	luabridge::LuaRef luaFunc = luabridge::LuaRef::fromStack(luaState, 4);

	EventType::ID eventId = dicIter->second;

	/*if (mLuaListeners.find(eventId) == mLuaListeners.end())
		mLuaListeners.insert(std::make_pair(eventId, std::vector<LuaEventCallBackFunc::Ptr>()));*/

	//std::unique_ptr<luabridge::LuaRef> luaRef(&luaFunc);
	//this is the problem
	mLuaListeners[eventId].push_back(std::move(LuaEventCallBackFunc::Ptr(new LuaEventCallBackFunc(thisEntity, luaFunc))));
	//mLuaListeners[eventId].back().mLuaCallBack = std::move(std::make_unique<luabridge::LuaRef>(luaFunc));
}

void EventManager::deriveCallToEvent(const luabridge::LuaRef& func, Entity* ownerEntity,
	EventBase* eventBase)
{
	switch (eventBase->getEventType()){
	case EventType::FireSpiritCollided:
		callFuncCastEventBase<FireSpiritCollidedEvent>(func, ownerEntity, eventBase);
		break;
	case EventType::CreateNewEntity:
		callFuncCastEventBase<CreateNewEntityEvent>(func, ownerEntity, eventBase);
		break;
	case EventType::SoulHookStruck:
		callFuncCastEventBase<SoulHookStruckEvent>(func, ownerEntity, eventBase);
		break;
	default:
		break;
	}
}

LuaEventCallBackFunc::LuaEventCallBackFunc(Entity* entity,  const luabridge::LuaRef& luaRef)
:mLuaCallBack(luaRef), ownerEntity(entity)
{

}