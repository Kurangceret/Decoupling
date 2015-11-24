#ifndef CATEGORY_H
#define CATEGORY_H


namespace Category{
	enum ID{
		None = 1 << 0,
		VertexNode = 1 << 1,
		Layer = 1 << 2,
		Player = 1 << 3,
		Tile = 1 << 4,
		Spider = 1 << 5,
	};
}


#endif