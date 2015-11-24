#pragma once
#include "State.h"
#include "StateIdentifier.h"
#include <functional>
#include <vector>
#include <map>
#include <cassert>

class StateStack{
public:
	enum Action{
		Push,
		Pop,
		Clear
	};
public:
	explicit StateStack(State::Context context);
	~StateStack();
	template<typename T>
	void registerState(States::ID stateID)
	{
		mFactories[stateID] = [&]()
		{
			return std::move(State::Ptr(new T(*this, mContext))); // this context will be used in all state under this statestack class
		};
	}


	void update(sf::Time dt);
	void draw();
	void handleEvent(const sf::Event&);

	void pushState(States::ID stateID);
	void popState();
	void clearStates();


	bool isEmpty() const { return mStack.empty(); }

private:
	State::Ptr createState(States::ID stateID);
	void applyPendingChanges();

private:
	struct PendingChange{
		explicit PendingChange(Action action, States::ID stateID = States::None)
		:action(action), stateID(stateID)
		{}
		Action	action;
		States::ID	stateID;
	};

	void clearStackAllocation();
private:
	std::vector<State::Ptr> mStack;	//keeps all the State 
	std::vector<PendingChange> mPendingList;	//keeps all the change/action to be done
	State::Context mContext;
	//mFactories hold all the states represented by States::ID and
	//the value is a function to be called to load the states
	std::map<States::ID, std::function<State::Ptr()>> mFactories;
};

