#include "Utility.h"
#include "RotatedRect.h"
#include <SFML/Graphics/VertexArray.hpp>

void Utility::drawRotatedRect(sf::RenderTarget& target, const RotatedRect* rotatedRect,
	const sf::Color& color)
{
	const RotatedRect::FourPoints& points = rotatedRect->mPoints;
	sf::Color curColor = color;

	if (color == sf::Color())
		curColor = sf::Color(255, 255, 255);

	sf::VertexArray vertexs(sf::LinesStrip);

	vertexs.append(sf::Vertex(points[RotatedRect::TopLeft], curColor));
	vertexs.append(sf::Vertex(points[RotatedRect::TopRight], curColor));
	vertexs.append(sf::Vertex(points[RotatedRect::BottomRight], curColor));
	vertexs.append(sf::Vertex(points[RotatedRect::BottomLeft], curColor));
	vertexs.append(sf::Vertex(points[RotatedRect::TopLeft], curColor));
	target.draw(vertexs);
}

void Utility::drawFloatToInt(sf::RenderTarget& target, const sf::RenderStates& states,
	const sf::Vector2f& pos, const sf::Drawable& sprite)
{
	sf::RenderStates statess = states;

	sf::Vector2i v;

	v.x = static_cast<int> (pos.x);
	v.y = static_cast<int> (pos.y);

	sf::Vector2f newV;
	newV.x = static_cast<float>(v.x) - pos.x;
	//newV.x = 0.f;
	newV.y = static_cast<float>(v.y) - pos.y;
	statess.transform.translate(newV);

	target.draw(sprite, statess);
}

bool Utility::isPointInsideImaginaryCircle(const sf::Vector2f& circlePos, float circleRadius,
	const sf::Vector2f& pointPos)
{
	if (pointPos.x >= circlePos.x - circleRadius
		&& pointPos.x <= circlePos.x + circleRadius
		&&	pointPos.y >= circlePos.y - circleRadius
		&& pointPos.y <= circlePos.y + circleRadius)
	{
		float dx = circlePos.x - pointPos.x;
		float dy = circlePos.y - pointPos.y;

		dx *= dx;
		dy *= dy;

		float distanceSquared = dx + dy;
		float radiusSquared = circleRadius * circleRadius;

		return distanceSquared <= radiusSquared;
	}

	return false;
}
Utility::CircleGeometry::CircleGeometry(const sf::Vector2f& centerPos, float radius)
{
	mCenterPos = centerPos;
	mRadius = radius;
}

bool Utility::isCircleIntersectsWithFloatRect(const Utility::CircleGeometry& circle, 
	const sf::FloatRect& floatRect)
{
	sf::Vector2f midOfRect = Utility::getMidOfRect(floatRect);
	sf::Vector2f circleDistance(std::abs(circle.mCenterPos.x - midOfRect.x) ,
		std::abs(circle.mCenterPos.y - midOfRect.y));

	if (circleDistance.x > (floatRect.width / 2.f + circle.mRadius)) return false;
	if (circleDistance.y > (floatRect.height / 2.f + circle.mRadius)) return false;

	if (circleDistance.x <= (floatRect.width / 2.f)) return true;
	if (circleDistance.y <= (floatRect.height / 2.f)) return true;

	float cornerDistance = std::pow((circleDistance.x - floatRect.width / 2.f), 2.f) +
		std::pow((circleDistance.y - floatRect.height / 2.f), 2.f);

	return (cornerDistance <= std::pow(circle.mRadius, 2));
}


bool Utility::rotatedCollision(const RotatedRect& lhs, const RotatedRect& rhs)
{
	for (int rect = 0; rect < 2; rect++){
		const RotatedRect& cur = rect == 0 ? lhs : rhs;

		for (std::size_t i1 = 0; i1 < RotatedRect::PositionCount; i1++){
			std::size_t i2 = (i1 + 1) % RotatedRect::PositionCount;

			double normalY = cur.mPoints[i2].y - cur.mPoints[i1].y;
			double normalX = cur.mPoints[i2].x - cur.mPoints[i1].x;

			double minA = std::numeric_limits<double>::max();
			double maxA = std::numeric_limits<double>::min();

			for (int lhsI = 0; lhsI < RotatedRect::PositionCount; lhsI++){
				double projected = normalX * lhs.mPoints[lhsI].x +
					normalY * lhs.mPoints[lhsI].y;
				if (projected < minA) minA = projected;
				if (projected > maxA) maxA = projected;
			}

			double minB = std::numeric_limits<double>::max();
			double maxB = std::numeric_limits<double>::min();

			for (int rhsI = 0; rhsI < RotatedRect::PositionCount; rhsI++){
				double projected = normalX * rhs.mPoints[rhsI].x +
					normalY * rhs.mPoints[rhsI].y;
				if (projected < minB) minB = projected;
				if (projected > maxB) maxB = projected;
			}

			if (maxA < minB || maxB < minA)
				return false;
		}
	}
	return true;
}

bool Utility::offSetRect1AndCheckRect2(const sf::FloatRect& rect1,
	const sf::Vector2f& offSet, const sf::FloatRect& rect2)
{
	sf::FloatRect newOffSetRect = rect1;
	newOffSetRect.top += offSet.y;
	newOffSetRect.left += offSet.x;

	return newOffSetRect.intersects(rect2);
}


bool Utility::offSetRect1AndCheckRect2Script(const sf::FloatRect& rect1,
	const sf::Vector2f& offSetDir,
	float valOffSet, const sf::FloatRect& rect2)
{
	return offSetRect1AndCheckRect2(rect1, offSetDir * valOffSet, rect2);
}