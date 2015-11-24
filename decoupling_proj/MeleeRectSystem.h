#pragma once
#include "System.h"
#include <SFML/Graphics/RenderTarget.hpp>
struct RotatedRect;
class MeleeRectSystem :	public System{
public:
	typedef std::unique_ptr<MeleeRectSystem> Ptr;
public:
	MeleeRectSystem();
	virtual ~MeleeRectSystem();


	void drawCollideableMeleeRect(sf::RenderTarget& target);
protected:
	virtual void processEntity(sf::Time dt, Entity* entity);
private:
	std::vector<const RotatedRect*> mRotatedRectList;
};

