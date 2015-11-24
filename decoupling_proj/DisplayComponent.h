#pragma once
#include "Component.h"
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

class DisplayComponent :public Component{
public:
	DisplayComponent(Entity* entity);
	virtual ~DisplayComponent();

	virtual sf::Drawable* getDrawable();
};

