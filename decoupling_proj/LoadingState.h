#pragma once
#include "StateStack.h"
#include "LoadingTask.h"
class LoadingState : public State{
public:
	LoadingState(StateStack& stack, Context context);
	virtual ~LoadingState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event&);
	void setCompletion(float percent, std::string currrentProcess);
private:
	sf::Text mLoadingText;
	sf::RectangleShape mProgressBarBackground;
	sf::RectangleShape mProgressBar;

	LoadingTask mLoadingTask;

};

