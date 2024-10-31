#ifndef DYNAMICTREE_H
#define DYNAMICTREE_H

#include <list>
#include <set>
#include <map>

using namespace std;

class Node;
class DynamicTree;
class Vector3;

//.......................................................
class Vector3
{
public:
	Vector3();
	Vector3(const Vector3& vec);
	Vector3(float x, float y, float z);
	Vector3& operator = (const Vector3& vec);
	~Vector3(void){};

	float x,y,z;
};

//.......................................................
class OObject
{
	friend class Node;
public:
	OObject( void );
	OObject( float left, float right, float down, float up, float back, float front );
	~OObject(void);

	void addParent(const Node* node );
	void deleteParent( Node* node );
	void deleteParents();
	float getUp() const;
	float getDown() const;
	float getRight() const;
	float getLeft() const;
	float getFront() const;
	float getBack() const;

	void setUp( const float& up );
	void setDown( const float& down );
	void setLeft( const float& left );
	void setRight( const float& right );
	void setBack( const float& back );
	void setFront( const float& front );

	void deleteUp();
	void deleteDown();
	void deleteLeft();
	void deleteRight();
	void deleteBack();
	void deleteFront();

	bool isUp();
	bool isDown();
	bool isRight();
	bool isLeft();
	bool isBack();
	bool isFront();

	Vector3& getCenter() const;
	void move(const Vector3& vec);
	set<Node*>& getNode();

private:
	float up, down, left, right, front, back;
	bool	isup, isdown, isleft, isright, isfront, isback;
	set<Node*> parents;
	Vector3 center;
};

//.......................................................
class Node
{
public:
	Node();
	Node(DynamicTree* octree);
	Node(DynamicTree* octree, Node* parent);
	~Node(void);
	void addObject( OObject* obj );
	void moveObject( OObject* obj, const Vector3& vec );
private:
	enum Divide { NONE = 0, X_DIVIDE = 1, Y_DIVIDE = 2, Z_DIVIDE = 3 } divide;
	float XDiv, YDiv, ZDiv;
	OObject* BBox;
	bool isLeft, isRight, isUp, isDown, isBack, isFront;

	bool isChild;
	Node *child1, *child2;

	Node* parent;
	DynamicTree* tree;
	
	int concentration;
	set<OObject*> objects;

	struct LineSegment
	{
		float min;
		float max;
	};

	bool isCollide( const OObject* obj1, const OObject* obj2, bool& order );
	bool isXCross( const OObject* obj1, const OObject* obj2, bool& order);
	bool isYCross( const OObject* obj1, const OObject* obj2, bool& order);
	bool isZCross( const OObject* obj1, const OObject* obj2, bool& order);
	bool isDimensionCross( const LineSegment& obj1, const LineSegment& obj2, float& divide, bool& order );
	bool isObjectDimCross( const OObject* obj, float& dim, Node::Divide div );
	bool isObjectDimCross( const OObject* obj, bool& order );
	bool isCollide( const OObject* obj );
	void split(OObject* obj, bool order);
};

//.......................................................
class DynamicTree
{
public:
	DynamicTree(int maxConcentration);
	~DynamicTree(void);

	void addObject(OObject* obj);
	void moveObject(OObject* obj, const Vector3& vec);
	Node* getNode(OObject& obj);
	list<OObject*> getObjects(const Node& node);

	int getMaxConcentration();
private:
	int maxConcentration;
	OObject* razmer;
	Node* child;
};



#endif