#include "State.h"
#include "StateStack.h"

State::State(StateStack& stack, Context context)
:mStack(&stack), mContext(context)
{
}


State::~State()
{
}

void State::requestStackPush(States::ID id)
{
	mStack->pushState(id);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStackClear()
{
	mStack->clearStates();
}
