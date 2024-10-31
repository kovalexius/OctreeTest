#include "DynamicTree.h"

//..............................................................................................
Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3()
{
	x=0;
	y=0;
	z=0;
}

Vector3::Vector3(const Vector3& vec)
{
	x=vec.x;
	y=vec.y;
	z=vec.z;
}

Vector3& Vector3::operator=(const Vector3& vec)
{
	x=vec.x;
	y=vec.y;
	z=vec.z;
	return *this;
}

//.............................................................................................
OObject::OObject()
{
	isup		=	false;
	isdown	= false;
	isleft	= false;
	isright = false;
	isfront = false; 
	isback	= false;
}

OObject::OObject(float left, float right, float down, float up, float back, float front)
{
	this->left = left;
	this->right = right;
	this->up = up;
	this->down = down;
	this->front = front;
	this->back = back;

	center.x = (left + right)/2;
	center.y = (up + down)/2;
	center.z = (front + back)/2;

	isup		=	true;
	isdown	=	true;
	isleft	=	true;
	isright = true;
	isfront = true;
	isback	= true;
}

OObject::~OObject()
{
}

void OObject::addParent(const Node* node)
{
	parents.insert(const_cast<Node*>(node));
}

void OObject::deleteParents()
{
	parents.clear();
}

void OObject::deleteParent( Node* node )
{
	parents.erase( node );
}

void OObject::setDown( const float& down )
{
	this->down = down;
	isdown = true;
}

void OObject::setUp( const float& up )
{
	this->up = up;
	isup = true;
}

void OObject::setLeft( const float& left )
{
	this->left = left;
	isleft = true;
}

void OObject::setRight( const float& right )
{
	this->right = right;
	isright = true;
}

void OObject::setBack( const float& back )
{
	this->back = back;
	isback = true;
}

void OObject::setFront( const float& front )
{
	this->front = front;
	isfront = true;
}

void OObject::deleteUp()
{
	isup = false;
}

void OObject::deleteDown()
{
	isdown = false;
}

void OObject::deleteLeft()
{
	isleft = false;
}

void OObject::deleteRight()
{
	isright = false;
}

void OObject::deleteBack()
{
	isback = false;
}

void OObject::deleteFront()
{
	isfront = false;
}

bool OObject::isUp()
{
	return isup;
}

bool OObject::isDown()
{
	return isdown;
}

bool OObject::isRight()
{
	return isright;
}

bool OObject::isLeft()
{
	return isleft;
}

bool OObject::isBack()
{
	return isback;
}

bool OObject::isFront()
{
	return isfront;
}

float OObject::getUp() const
{
	return up;
}

float OObject::getDown() const
{
	return down;
}

float OObject::getRight() const
{
	return right;
}

float OObject::getLeft() const
{
	return left;
}

float OObject::getFront() const
{
	return front;
}

float OObject::getBack() const
{
	return back;
}

Vector3& OObject::getCenter() const
{
	return (Vector3&)center;
}

void OObject::move(const Vector3& vec)
{
	up += vec.y;
	down += vec.y;
	center.y += vec.y;

	left += vec.x;
	right += vec.x;
	center.x += vec.x;

	back += vec.z;
	front += vec.z;
	center.z += vec.z;
}

set<Node*>& OObject::getNode()
{
	return parents;
}

//.............................................................................................
Node::Node()
{
	concentration = 0;
	parent = 0;
	tree = 0;

	isChild = 0;

	isLeft = false;
	isRight = false;
	isDown = false;
	isUp = false;
	isBack = false;
	isFront = false;
	
	divide = NONE;
}

Node::Node(DynamicTree* dtree)
{
	concentration = 0;
	tree = dtree;
	parent = 0;

	isChild = 0;

	isLeft = false;
	isRight = false;
	isDown = false;
	isUp = false;
	isBack = false;
	isFront = false;
	
	divide = NONE;
}

Node::Node(DynamicTree* dtree, Node* node)
{
	concentration = 0;
	tree = dtree;
	parent = node;

	isChild = 0;

	isLeft = false;
	isRight = false;
	isDown = false;
	isUp = false;
	isBack = false;
	isFront = false;
	
	divide = NONE;
}

void Node::addObject(OObject* obj)
{
	//...............Если нет потомков..................
	if( !isChild )
	{
		if( objects.empty())
		{
			concentration++;
			obj->addParent(this);
			objects.insert(obj);
			return;
		}
		else
		{
			bool order;
			for( auto curObj = objects.begin(); curObj != objects.end(); curObj++ )
			{
				if( isCollide(obj, *curObj, order) )
				{
					concentration++;
					objects.insert(obj);
					return;
				}
			}

			//....разделить узел на дочерние....
			isChild = true;
			child1 = new Node( tree, this );
			child2 = new Node( tree, this );
			if( order )
			{
				obj->deleteParents();
				child1->addObject( obj );
				for( auto curObj = objects.begin(); curObj != objects.end(); curObj++ )
				{
					child2->addObject(*curObj);
					(*curObj)->deleteParents();
				}
			}
			else
			{
				for( auto curObj = objects.begin(); curObj != objects.end(); curObj++ )
				{
					child1->addObject(*curObj);
					(*curObj)->deleteParents();
				}
				obj->deleteParents();
				child2->addObject( obj );
			}
			objects.clear();

			return;
			//..................................
		}
	}
	//..................................................

	//...............Если есть потомки..................
	if( isChild )
	{
		bool order;
		if( isObjectDimCross( obj, order ) )
			objects.insert(obj);
		else
		{
			if(order)
				child1->addObject(obj);
			else
				child2->addObject(obj);
		}
	}
	//..................................................
}

void Node::moveObject(OObject* obj, const Vector3& vec)
{
	obj->move(vec);
	bool sw = false;
	bool order;
	if( isObjectDimCross( obj, order ) )
	{
		//parent->moveObject
	}
	
}

bool Node::isObjectDimCross( const OObject* obj, bool& order )
{
	switch(divide)
	{
		case X_DIVIDE:
			if( obj->getLeft() <= XDiv )
			{
				if( obj->getRight() >= XDiv )
					return true;
				else
					order = true;
			}
			else
				order = false;
			break;
		case Y_DIVIDE:
			if( obj->getDown() <= YDiv )
			{
				if( obj->getUp() >= YDiv )
					return true;
				else
					order = true;
			}
			else
				order = false;
			break;
		case Z_DIVIDE:
			if( obj->getBack() <= ZDiv )
			{
				if( obj->getFront() >= ZDiv )
					return true;
				else
					order = true;
			}
			else
				order = false;
			break;
	}
	return false;
}

bool Node::isObjectDimCross( const OObject* obj, float& dim, Node::Divide div )
{
	if ( div == Node::Divide::X_DIVIDE )
	{
		if( obj->getLeft() < dim && obj->getRight() > dim )
			return true;
		return false;
	}

	if ( div == Node::Divide::Y_DIVIDE )
	{
		if( obj->getDown() < dim && obj->getUp() > dim )
			return true;
		return false;
	}

	if ( div == Node::Divide::Z_DIVIDE )
	{
		if( obj->getBack() < dim && obj->getFront() > dim )
			return true;
		return false;
	}
	return false;
}

bool Node::isCollide(const OObject* obj1, const OObject* obj2, bool& order)
{
	if( isXCross(obj1, obj2, order) && isYCross(obj1, obj2, order) && isZCross(obj1, obj2, order) )
		return true;
	return false;
}

bool Node::isXCross( const OObject* obj1, const OObject* obj2, bool& order )
{
	LineSegment o1, o2;
	o1.min = obj1->getLeft();
	o1.max = obj1->getRight();
	o2.min = obj2->getLeft();
	o2.max = obj2->getRight();

	bool ret;
	float div;
	ret = isDimensionCross(o1, o2, div, order);
	if(!ret)
	{
		XDiv = div;
		divide = X_DIVIDE;
	}

	return ret;
}

bool Node::isYCross( const OObject* obj1, const OObject* obj2, bool& order )
{
	LineSegment o1, o2;
	o1.min = obj1->getDown();
	o1.max = obj1->getUp();
	o2.min = obj2->getDown();
	o2.max = obj2->getUp();

	bool ret;
	float div;
	ret = isDimensionCross(o1, o2, div, order);
	if(!ret)
	{
		YDiv = div;
		divide = Y_DIVIDE;
	}

	return ret;
}

bool Node::isZCross( const OObject* obj1, const OObject* obj2, bool& order )
{
	LineSegment o1, o2;
	o1.min = obj1->getBack();
	o1.max = obj1->getFront();
	o2.min = obj2->getBack();
	o2.max = obj2->getFront();

	bool ret;
	float div;
	ret = isDimensionCross(o1, o2, div, order);
	if(!ret)
	{
		ZDiv = div;
		divide = Z_DIVIDE;
	}

	return ret;
}

bool Node::isDimensionCross( const LineSegment& obj1, const LineSegment& obj2, float& divide, bool& order )
{
	if( obj1.max >= obj2.max )
	{
		if( obj1.min <= obj2.max )
			return true;
	}
	else
	{
		if( obj1.max >= obj2.min )
			return true;
	}

	if( obj1.min <= obj2.min )
	{
		if( obj1.max >= obj2.min )
			return true;
		else
		{
			order = true;
			divide = ( obj1.max + obj2.min ) / 2.0f; // Разделить; obj1 - вначале, obj2 - вконце
		}
	}
	else
	{
		if( obj1.min <= obj2.max )
			return true;
		else
		{
			order = false;
			divide = ( obj1.min + obj2.max ) / 2.0f; // Разделить; obj1 - вконце, obj2 - вначале
		}
	}

	return false;
}

//.............................................................................................
DynamicTree::DynamicTree(int maxConcentration)
{
	this->maxConcentration = maxConcentration;
	child = 0;
}

DynamicTree::~DynamicTree(void)
{
}

int DynamicTree::getMaxConcentration()
{
	return maxConcentration;
}

void DynamicTree::addObject( OObject* obj )
{
	if(!child)
		child = new Node(this);
	child->addObject(obj);
}

void DynamicTree::moveObject( OObject* obj, const Vector3& vec )
{
	if(child)
		child->moveObject(obj, vec);
}