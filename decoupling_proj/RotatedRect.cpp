#include "RotatedRect.h"


RotatedRect::RotatedRect()
{
	for (std::size_t i = 0; i < PositionCount; i++) mPoints[i] = sf::Vector2f(0.f, 0.f);
}


RotatedRect::RotatedRect(const sf::FloatRect& rect)
{
	transformFloatRect(rect);
}


void RotatedRect::transformFloatRect(const sf::FloatRect& rect)
{
	mPoints[RotatedRect::TopLeft] = sf::Vector2f(rect.left, rect.top);
	mPoints[RotatedRect::TopRight] = sf::Vector2f(rect.left + rect.width, rect.top);
	mPoints[RotatedRect::BottomLeft] = sf::Vector2f(rect.left, rect.top + rect.height);
	mPoints[RotatedRect::BottomRight] = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
}

bool RotatedRect::rectIsAllZeroes() const
{
	for (std::size_t i = 0; i < PositionCount; i++){
		if (mPoints[i] != sf::Vector2f())
			return false;
	}
	return true;
}