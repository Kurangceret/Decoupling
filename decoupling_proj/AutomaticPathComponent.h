#pragma once
#include "Component.h"
#include "MovingNode.h"
//#include "PathFinder.h"
#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>

class AutomaticPathComponent :	public Component{
public:
	
	//System has to iterate this type from the front
	typedef std::vector<MovingNode> AutomaticPathList;
public:
	AutomaticPathComponent(Entity* entity);
	virtual ~AutomaticPathComponent();

	void drawAutomaticPaths(sf::RenderTarget& target);

	void setDestinationToCalculate(AStarNode* destNode);
	void setDestinationToCalculate(const sf::Vector2f& pos);
	void setDestinationToCalculate(float posX, float posY);

	/*exclusive for script usage*/
	void setDestToCalculate(float posX, float posY);
	/**/

	bool isAutomaticPathsEmpty() const;

	void clearAutomaticPaths();
	void setAutomaticPaths(const std::vector<MovingNode>& automaticPaths, 
		AStarNode* finalDestNode);

	AStarNode* getCurrentDestNode() const;
	//if returns
	sf::Vector2f getUncalculatedDest() const;

	AutomaticPathList& getAutomaticPaths();


	bool isUncalculatedPosValid() const;

private:
	//value that indicates the returned getUncalculatedDest() equals to this return
	//means that it does not need to be calculated
	sf::Vector2f getNoNeedCalculationVal() const;

private:
	AutomaticPathList mAutomaticPaths;
	sf::Vector2f mUncalculatedDest;
	AStarNode* mCurrentDestNode;


};

