#pragma once
#include "StateStack.h"
class MenuState : public State{
public:
	MenuState(StateStack& stack, Context context);
	virtual ~MenuState();

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event&);

	void updateOptionText();
private:
	enum{
		Play,
		Exit
	};
private:
	std::size_t mOptionIndex;
	std::vector<sf::Text> mTexts;
};

