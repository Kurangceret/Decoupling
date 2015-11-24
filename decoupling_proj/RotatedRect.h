#ifndef ROTATEDRECT_H
#define ROTATEDRECT_H
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <array>

struct RotatedRect{
	enum Position{
		TopLeft,
		TopRight,
		BottomRight,
		BottomLeft,
		PositionCount
	};
	typedef std::array<sf::Vector2f, RotatedRect::PositionCount> FourPoints;

	RotatedRect();

	RotatedRect(const sf::FloatRect& rect);

	bool rectIsAllZeroes() const;

	void transformFloatRect(const sf::FloatRect& rect);

	std::array<sf::Vector2f, PositionCount> mPoints;
};



#endif