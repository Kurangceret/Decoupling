#include "AStarNode.h"


bool AStarNode::operator > (const AStarNode& right) const{ return f > right.f; }

bool AStarNode::operator >= (const AStarNode& right)const{ return f >= right.f; }

bool AStarNode::operator < (const AStarNode& right)const{ return f < right.f; }

bool AStarNode::operator <= (const AStarNode& right)const{ return f <= right.f; }