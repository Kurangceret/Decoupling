#include "CommandQueue.h"

CommandQueue::CommandQueue()
{}

CommandQueue::~CommandQueue()
{}


void CommandQueue::push(Command command)
{
	mCommands.push(command);
	//mCommandsMap[owner] = command;
	//mCommandsMap.push_back(std::make_pair(owner, command));
}

void CommandQueue::push(SceneGraphCommand command)
{
	mSceneGraphCommand.push(command);
}

Command CommandQueue::pop()
{
	Command command = mCommands.front();

	mCommands.pop();

	/*auto iter = mCommandsMap.begin();
	
	Command command = iter->second;
	mCommandsMap.erase(mCommandsMap.begin());*/
	return command;
}

CommandQueue::SceneGraphCommand CommandQueue::popSceneGraphCommand()
{
	SceneGraphCommand command = mSceneGraphCommand.front();
	mSceneGraphCommand.pop();

	return command;
}

/*void CommandQueue::deleteCommand(const SceneNode* owner)
{
	if (mCommandsMap.empty() || !owner)
		return;

	//auto iter = mCommandsMap.find(owner);
	auto iter = std::find_if(mCommandsMap.begin(), mCommandsMap.end(), 
		[&](std::pair<const SceneNode* , Command>& pair) -> bool
	{
		return owner == pair.first;
	});

	if (iter != mCommandsMap.end())
		mCommandsMap.erase(iter);
}*/

bool CommandQueue::isEmpty() const{ return mCommands.empty(); }
bool CommandQueue::isSceneGraphCommandEmpty() const{ return mSceneGraphCommand.empty(); }