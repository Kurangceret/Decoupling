#pragma once
#include <SFML/System/Time.hpp>
#include <vector>
#include <bitset>
#include <memory>


class Entity;

class System{
public:
	typedef std::unique_ptr<System> Ptr;
public:
	System();
	virtual ~System();

	//virtual void init();
	virtual void update(sf::Time dt, Entity* entity);

	virtual bool checkForEntity(Entity* entity) const;
	void addEntity(Entity* entity);

	void setRequiredComponents(const 
		std::vector<std::size_t>& requiredComponents);
protected:
	virtual void processEntity(sf::Time dt, Entity* entity) = 0;
	void pushRequiredComponent(std::size_t componentIdentifier);
private:
	std::vector<Entity*> mEntities;
	//components the system wanted for it to fit inside
	std::vector<std::size_t> mRequiredComponents;
};

