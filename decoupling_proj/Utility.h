#ifndef UTILITY_H
#define UTILITY_H
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <random>
#include <cmath>
#include <ctime>
#include <cassert>
#include <iostream>
#include <set>
#include <cstdint>
#include <string>

struct RotatedRect;
namespace
{
	inline std::default_random_engine createRandomEngine()
	{
		auto seed = static_cast<unsigned long>(std::time(nullptr));
		return std::default_random_engine(seed);
	}

	auto RandomEngine = createRandomEngine();
}

namespace Utility{
	struct CircleGeometry{
		CircleGeometry(const sf::Vector2f& centerPos, float radius);
		sf::Vector2f mCenterPos;
		float mRadius;
	};

	template<class Object>
	inline void centerOrigin(Object& sfObj)
	{
		sf::FloatRect bounds = sfObj.getLocalBounds();
		sfObj.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	}

	template <class Pointer>
	inline std::uintptr_t convertPointerToAddress(Pointer* pointer)
	{
		std::uintptr_t address = reinterpret_cast<uintptr_t>(pointer);
		return address;
	}

	template<class Object>
	inline void centerOrigin(Object& sfObj, const sf::FloatRect& rect)
	{
		sfObj.setOrigin(rect.width / 2.f, rect.height / 2.f);
	}

	inline sf::Vector2f getMidOfRect(const sf::FloatRect& rect)
	{
		return sf::Vector2f(rect.left + rect.width / 2.f, rect.top + rect.height / 2.f);
	}


	inline float toDegree(float radian)
	{
		return 180.f / 3.141592653589793238462643383f * radian;
	}

	/*
	There are π radians in a half circle
	And also 180° in a half circle

	So π radians = 180°

	So 1 radian = 180°/π = 57.2958° (approximately)*/
	inline float toRadian(float degree)
	{
		return 3.141592653589793238462643383f / 180.f * degree;
	}

	inline sf::Vector2f degreeToVector(float degree)
	{
		float radians = toRadian(degree);

		sf::Vector2f v;
		if (std::abs(degree) != 270.f && std::abs(degree) != 90.f)
			v.x = std::cos(radians);
		if (std::abs(degree) != 180.f && std::abs(degree) != 360.f)
			v.y = std::sin(radians);

		//if (std::abs(v.x) > 0.f)v.x = 0.f;
		//if (std::abs(v.y) > 0.f)v.y = 0.f;

		return v;
	}

	inline float returnTheLargest(const float v1, const float v2)
	{
		assert(v1 != v2);
		return (v1 > v2) ? v1 : v2;
	}

	inline float vectorToDegree(const sf::Vector2f& vector, bool fullArcResult = true)
	{
		float initial = toDegree(std::atan2(vector.y, vector.x));
		if (fullArcResult && initial < 0.f)
			initial = 360.f - std::abs(initial);
		return initial;
	}

	inline int randomInt(int exclusiveMax)
	{
		std::uniform_int_distribution<> distr(0, exclusiveMax - 1);
		return distr(RandomEngine);
	}

	//exclusiveMax will be minused 1 in the func
	inline int randomRange(int exclusiveMin, int exclusiveMax)
	{
		if (exclusiveMin == exclusiveMax) return exclusiveMin;
		if (exclusiveMin > exclusiveMax) std::swap(exclusiveMin, exclusiveMax);

		std::uniform_int_distribution<> distr(exclusiveMin, exclusiveMax - 1);
		return distr(RandomEngine);
	}

	inline float getDotProduct(const sf::Vector2f& left, const sf::Vector2f& right)
	{
		return ((left.x * right.x) + (left.y * right.y));
	}

	inline sf::Vector2i absoluteVectori(const sf::Vector2i& vec)
	{
		sf::Vector2i newV;
		newV.x = std::abs(vec.x);
		newV.y = std::abs(vec.y);
		return newV;
	}

	inline sf::Vector2f absoluteVectorf(const sf::Vector2f& vec)
	{
		sf::Vector2f newV;
		newV.x = std::abs(vec.x);
		newV.y = std::abs(vec.y);
		return newV;
	}

	inline float vectorLength(sf::Vector2f vector)
	{
		return std::sqrt(vector.x * vector.x + vector.y * vector.y);
		//return Q_rsqrt(vector.x * vector.x + vector.y * vector.y);
	}
	
	inline sf::Vector2f minusVector(const sf::Vector2f& vector1, 
		const sf::Vector2f& vector2)
	{
		return vector1 - vector2;
	}

	inline sf::Vector2i minusVectorI(const sf::Vector2i& vector1,
		const sf::Vector2i& vector2)
	{
		return vector1 - vector2;
	}

	inline sf::Vector2f plusVector(const sf::Vector2f& vector1,
		const sf::Vector2f& vector2)
	{
		return vector1 + vector2;
	}

	inline sf::Vector2i plusVectorI(const sf::Vector2i& vector1,
		const sf::Vector2i& vector2)
	{
		return vector1 + vector2;
	}

	inline sf::Vector2f multiplyVectorByFloat(const sf::Vector2f& vector1,
		float length)
	{
		return vector1 * length;
	}

	inline float hattanLength(const sf::Vector2f& vector)
	{
		return std::abs(vector.x) + std::abs(vector.y);
	}

	inline void printVector(const sf::Vector2f& vector, bool endLine)
	{
		std::cout << " " << vector.x << " " << vector.y << " ";
		if (endLine)
			printf("\n");
	}

	inline void printVector(const sf::Vector2i& vector, bool endLine)
	{
		std::cout << " " << vector.x << " " << vector.y << " ";
		if (endLine)
			printf("\n");
	}

	inline sf::Vector2f unitVector(const sf::Vector2f& vector)
	{
		//assert(vector != sf::Vector2f(0.f, 0.f));
		if (vector == sf::Vector2f(0.f, 0.f))
			return vector;

		return vector / vectorLength(vector);
	}

	inline sf::Vector2f unitVectorScript(const sf::Vector2f& vector)
	{
		return Utility::unitVector(vector);
	}

	inline sf::Vector2f unitVector(const sf::Vector2i& vector)
	{
		sf::Vector2f vectorF(vector);
		if (vector == sf::Vector2i(0, 0))
			return vectorF;

		return vectorF / vectorLength(vectorF);
	}

	inline sf::Vector2i intRectTopLeftFormat(const sf::IntRect& intRect)
	{
		return sf::Vector2i(intRect.left, intRect.top);
	}

	inline sf::Vector2i intRectSizeFormat(const sf::IntRect& intRect)
	{
		return sf::Vector2i(intRect.width, intRect.height);
	}

	inline void printStringToCmd(const std::string& message, bool endLine = true)
	{
		std::cout << message;
		if (endLine) std::cout << std::endl;
	}

	bool offSetRect1AndCheckRect2(const sf::FloatRect& rect1, 
		const sf::Vector2f& offSet, const sf::FloatRect& rect2);

	/*Exclusvie Script Usage*/
	bool offSetRect1AndCheckRect2Script(const sf::FloatRect& rect1,
		const sf::Vector2f& offSetDir, float valOffSet, const sf::FloatRect& rect2);

	void drawRotatedRect(sf::RenderTarget& target, const RotatedRect* rotatedRect,
		const sf::Color& color = sf::Color());

	bool rotatedCollision(const RotatedRect& lhs, const RotatedRect& rhs);

	void drawFloatToInt(sf::RenderTarget& target, const sf::RenderStates& states,
		const sf::Vector2f& pos, const sf::Drawable& sprite);
	
	bool isPointInsideImaginaryCircle(const sf::Vector2f& circleCenterPos, float circleRadius,
		const sf::Vector2f& pointPos);

	bool isCircleIntersectsWithFloatRect(const CircleGeometry& circle, const sf::FloatRect& floatRect);
}

#endif