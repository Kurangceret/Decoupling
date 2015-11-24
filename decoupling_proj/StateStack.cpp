#include "StateStack.h"


StateStack::StateStack(State::Context context)
:mContext(context)
{
	
}


State::Ptr StateStack::createState(States::ID id)
{
	auto& found = mFactories.find(id);
	assert(found != mFactories.end());

	return found->second();
}

void StateStack::update(sf::Time dt)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); itr++){
		if (!(*itr)->update(dt))
			break;
	}
	applyPendingChanges();
}

void StateStack::handleEvent(const sf::Event& event)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); itr++){
		if (!(*itr)->handleEvent(event))
			break;
	}
	applyPendingChanges();
}

void StateStack::applyPendingChanges()
{
	for (PendingChange& change : mPendingList){
		switch (change.action){
		case Push:
			mStack.push_back(createState(change.stateID));
			break;
		case Pop:
			mStack.pop_back();
			break;
		case Clear:
			clearStackAllocation();
			break;
		}
	}
	mPendingList.clear();
}

void StateStack::clearStackAllocation()
{
	for (auto& ptr : mStack)
		delete ptr.release();
	mStack.clear();
}

void StateStack::draw()
{
	for (State::Ptr& ptr : mStack)
		ptr->draw();
}

void StateStack::pushState(States::ID id)
{
	mPendingList.push_back(PendingChange(Push, id));
}

void StateStack::popState()
{
	mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
	mPendingList.push_back(PendingChange(Clear));
}

StateStack::~StateStack()
{
}
