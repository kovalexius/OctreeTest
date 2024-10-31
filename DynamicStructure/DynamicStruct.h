// ��� ��������� ���������� ������� ��� �������� ��������� ������������ ����������� ��������� � ������� ���� ����� ���������� ����������� �����
// �� AABB ������ - ����, Bouding Box'� � ��� �������� ������������� �������� � ����� �� ������� ��� �������� �������� ��� ��������� �����.
// ��� ����� ���� ������������ ��� MMO ���, ��� ����������� ���������� �������� ��������� ��������, � ��. �����. 
// ��� ���������� ���������� ��������� ����������� ������������ �� ���� � �������� ����� ��� ��������.
// ������ ��� ������ ����� ������, � ����� ��������������� ������ ������� n^2*m, ��� n - ���-�� ��������, m - ���-�� ����� � �������. 
// ������������-����������� ���������� ������ ��� ��������� ����������� �������� ������� ������ ��� ������ ������������ DynamicStruct.
// ��� ���������� ������, ��� ��� ���������� ������� - ��� ������� ������� ���� Vector3, ������� ������������� ������ � ������� ����� AddPoint( Vector3& point, void* obj )
// ��� obj - ��� ��������� �� ���� ������ (�����) ����������� � ���� void*. ����� � ������� ����� ������� �������� 
// movePoint � moveObject ��������������.							 
// ��� ����������� ������, ������ �� ���������
// ��� ����� ������ ����������� ����� �������� � ������ ������ Octree � ��� �������� ��������� ��� ���

// � ���������� ����� ����������� ������������������, �������� ������������� ������������� ��� �������� ���� ���������������� 
// set, map, unordered_xxx. ����� ����� ��������� �������� �� �������
// � ���� ���������

// �������� ���� ������ ������, ������� �� �������� ��� ��������� �� shared_ptr � shared_array ��� ��������

// ����� �����!!! ����� �����������!!!

#ifndef DYNAMICSTRUCT_H
#define DYNAMICSTRUCT_H

#include <string>
#include <list>
#include <set>
#include <map>
#include <iterator>

using namespace std;

class Cell;
struct Point;

//.......................................................
struct Vector3
{
	Vector3();
	//Vector3(const Vector3& vec);
	Vector3( float x, float y, float z);
	Vector3( float x, float y, float z, const string& name );
	//Vector3& operator = (const Vector3& v);
	Vector3 operator + (const Vector3& v) const;
	Vector3& operator += (const Vector3& v);
	~Vector3(void){};
	float x,y,z;
	void setCell( Cell* parent );
	Cell* getCell() const;
	void setIterator( const std::map< Vector3*, void* >::iterator& it );
	std::map< Vector3*, void* >::iterator& getIterator();
	std::string& getName();
private:
	Cell* parent;
	std::map< Vector3*, void* >::iterator m_iter; // ��������� �� Cell::map<Vector3*, void*> pointObj;
	std::string name;
};
//.......................................................

//......................................................
struct Address
{
	Address(int i, int j, int k);
	Address( const Address& other );
	int i, j, k;

	bool operator < ( const Address& right ) const;
	bool operator != ( const Address& right ) const;
	bool operator == ( const Address& right ) const;
};
//......................................................


//......................................................
class Cell
{
public:
	Cell( Address* addr);
	~Cell();
	void AddPoint( Vector3& v, void* obj );
	void DeletePoint( Vector3& v);
	map<Vector3*, void*>* getPoints() const;
	Address* getAddress(void) const;
	map< void*, set<Vector3*>* >* getObjects() const;
private:
	Address* address;
	map<Vector3*, void*> pointObj;							// ����� - ������ � ������� ������
	map<void*, set<Vector3*>* > objPoint;				// ������ - ��� ����� � ������� ������ 
};
//......................................................


//.....................................................
class DynamicStruct
{
public:
	DynamicStruct(float s);  // s - ������ ������
	~DynamicStruct();
	// �������� ����� point, ��������������� � �������� obj; 
	// ��������� ��� ��� ������ �� ������, � �� ��������, ����� ������������ uint32_t ��� ������������� ��������
	void AddPoint( Vector3& point, void* obj );	
	// ����������� ����� point �� ������ v
	void movePoint( Vector3& point, Vector3& v );	
	// ����������� ������ obj �� ������ v
	void moveObject( void* obj, Vector3& v );															

	// ���������� ����� - ��������� ������ ��� ��� ���������
	Cell* getCell ( const Vector3& point ) const;	
	// ���������� ������ - ��������� ��� ������ ��� ��������� ����� �������
	set<Cell*>* getCells ( const void* obj ) const;	
	// ���������� ������ - ��������� ��� ����� � ���� ������
	map< Vector3*, void* >* getPoints ( const Cell* cell ) const;		
	// ���������� ������ - ��������� ������� � ���� ������
	map< void*, set<Vector3*>* >* getObjects ( const Cell* cell ) const;  
	
	Cell* getCell ( const Address& addr ) const;													// ���������� ����� - ��������� ������
	Address& getAddress( const Cell* cell ) const;												// ���������� ������ - ��������� �����
	Address getAddress( const Vector3& vec ) const;											// ���������� ���������� - ��������� �����
	Vector3 getVector( const Address& addr ) const;											// ���������� ����� - ��������� ����������
	Vector3 getVector( const Cell* cell ) const;													// ���������� ������ - ��������� ����������
	set<Cell*>* getHotCells() const;																			// ��������� ��� ������ ��� ������ ������ �������, ������� ������
	set<Cell*>* segmentCast( const Vector3 &P0, const Vector3 &P1 );			// �������� ������ ����� ������� �������� �������
private:
	float size;
	map< Address, Cell* > addrCells;
	map< Vector3*, void* > pointObj;					// ����� - ������
	map< void*, set<Vector3*>* > objPoint;		// ������ - ��� ����� � �������
	map< void*, set<Cell*>* > objCell;				// ������ - ��� ������ � �������
	set<Cell*> hotCells;											// ������, ��� ���� ��������� ��������
};
//.....................................................


#endif