#pragma once
#include <queue>
#include "Command.h"
#include <unordered_map>
class SceneGraph;

class CommandQueue{
public:
	typedef std::function<void(SceneGraph&, sf::Time)> SceneGraphCommand;
public:
	CommandQueue();
	~CommandQueue();

	//if no owner means this command is supposed to be safe
	//from being removed
	void push(Command command);
	void push(SceneGraphCommand command);

	Command pop();
	SceneGraphCommand popSceneGraphCommand();
	

	//void deleteCommand(const SceneNode* owner);
	
	bool isEmpty() const;

	bool isSceneGraphCommandEmpty() const;

private:
	std::queue<Command> mCommands;
	std::queue<SceneGraphCommand> mSceneGraphCommand;
	//std::vector<std::pair<const SceneNode*, Command>> mCommandsMap;
};

