#ifndef COMMAND_H
#define COMMAND_H
#include "Category.h"
#include <functional>
#include <cassert>
#include <SFML/System.hpp>

class SceneNode;

struct Command{
	typedef std::function<void(SceneNode&, sf::Time)> Action;
	Command()
	:targetObject(nullptr), category(Category::None){}

	Category::ID category;
	std::function < void(SceneNode& sceneNode, sf::Time) > action;
	SceneNode *targetObject;
};


//act as a place to cast objects, since we will be working from different class which all inherited from Entity
template<typename GameObject, typename Function>
Command::Action derivedAction(Function fn)
{//[=] means variable referenced from its body are copied from the surrounding scope
	return[=](GameObject& node, sf::Time dt)
	{
		//std::cout << node.getCategory() << std::endl;
		//check if the cast is not null, if not then proceed
		assert(dynamic_cast<GameObject*>(&node) != nullptr);
		//Downcast node and invoke function on it
		fn(static_cast<GameObject&>(node), dt);
	};
}



/*Example of using Command struct
by normal function, just make sure you make it like 'Command c = &nameOfYourFunction' with the same
parameter as Command::action has
by lambda:
c.action = [] (SceneNode& node,  sf::Time dt)
{
node.move(-30.f * dt.asSeconds(), 0.f);
};
*/

#endif