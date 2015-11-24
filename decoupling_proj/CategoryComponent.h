#pragma once
#include "Component.h"
#include "Category.h"
class CategoryComponent :	public Component{
public:
	CategoryComponent(Entity* entity);
	virtual ~CategoryComponent();
	
	std::size_t getCategory() const;
	void setCategory(std::size_t category);

private:
	std::size_t mCategory;

};

