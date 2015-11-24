#include "TextDisplayComponent.h"


TextDisplayComponent::TextDisplayComponent(Entity* entity)
:Component(entity)
{
	mIdentifier = ComponentIdentifier::TextDisplayComponent;
	mText.setCharacterSize(12);

}


TextDisplayComponent::~TextDisplayComponent()
{
}


sf::Text& TextDisplayComponent::getText()
{
	return mText;
}

void TextDisplayComponent::setString(const std::string& str)
{
	mText.setString(str);
}