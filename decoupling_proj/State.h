#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "GeneralData.h"
#include "StateIdentifier.h"

class StateStack;

class State{
public:
	struct Context{
		Context(sf::RenderWindow& window, sf::Font& font, GeneralData& generalData)
		:window(&window),font(&font), generalData(&generalData){}

		sf::RenderWindow *window;
		sf::Font *font;
		GeneralData* generalData;
	};

	typedef std::unique_ptr<State> Ptr;
public:
	State(StateStack& stack, Context context);
	virtual ~State();

	virtual void draw() = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event&) = 0;

protected:
	void requestStackClear();
	void requestStackPop();
	void requestStackPush(States::ID);

	Context getContext()  const { return mContext; }
private:
	StateStack *mStack;
	Context mContext;
};

