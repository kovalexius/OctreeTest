#include "DynamicStruct.h"


//........................................................................
Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3(float x, float y, float z, const string& name )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->name = name;
}

Vector3::Vector3()
{
	x=0;
	y=0;
	z=0;
}

/*
Vector3::Vector3(const Vector3& vec)
{
	x=vec.x;
	y=vec.y;
	z=vec.z;
}

Vector3& Vector3::operator=(const Vector3& v)
{
	x=v.x;
	y=v.y;
	z=v.z;
	return *this;
}
*/

Vector3 Vector3::operator + (const Vector3& v) const
{
	return Vector3( x + v.x, y + v.y, z + v.z );
}

Vector3& Vector3::operator += (const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

void Vector3::setCell( Cell* c )
{
	parent = c;
}

Cell* Vector3::getCell() const
{
	return parent;
}

string& Vector3::getName()
{
	return name;
}

void Vector3::setIterator( const map<Vector3*, void*>::iterator& i )
{
	m_iter = i;
}

map< Vector3*, void* >::iterator& Vector3::getIterator()
{
	return m_iter;
}
//........................................................................


//......................................................................
Cell::Cell( Address* addr )
{
	address = addr;
}

Cell::~Cell()
{
	for( auto it = objPoint.begin(); it != objPoint.end(); it ++ )
		delete it->second;
}

void Cell::AddPoint( Vector3& v, void* obj )
{
	pointObj.insert ( pair<Vector3*, void*>(&v, obj) );
	v.setIterator( pointObj.find( &v ) );
	v.setCell( this );

	auto it = objPoint.find( obj );
	if( it == objPoint.end() )
	{
		set<Vector3*> *curSet = new set<Vector3*>;
		curSet->insert( &v );
		objPoint.insert( pair< void*, set< Vector3* >* >( obj, curSet ) );
	}
	else
	{
		set<Vector3*> *curSet = it->second;
		curSet->insert( &v );
	}
}

void Cell::DeletePoint ( Vector3& v )
{
	void *curObj = v.getIterator()->second;
	set<Vector3*> *curSet = objPoint.at(curObj);
	curSet->erase( &v );
	if( curSet->empty() )
	{
		delete curSet;
		objPoint.erase( curObj );
	}
	pointObj.erase( v.getIterator() );
}

map<Vector3*, void*>* Cell::getPoints() const
{
	return const_cast<map<Vector3*, void*>*>(&pointObj);
}


Address* Cell::getAddress() const
{
	return address;
}

map<void*, set<Vector3*>*>* Cell::getObjects() const
{
	return const_cast<map<void*, set<Vector3*>*>*>(&objPoint);
}
//......................................................................


//....................................................................
DynamicStruct::DynamicStruct( float s )
{
	size = s;
}

DynamicStruct::~DynamicStruct()
{
	for( auto it = objPoint.begin(); it != objPoint.end(); it ++ )
		delete it->second;
	for( auto it = objCell.begin(); it != objCell.end(); it++ )
		delete it->second;
}


void DynamicStruct::AddPoint( Vector3& point, void* obj )
{
	pointObj.insert( pair< Vector3*, void* >(&point, obj) );

	Address address = getAddress(point);
	Cell* cell;
	
	// обновление map< Address, Cell* > cells
	{
	auto it = addrCells.find( address );
	if( it == addrCells.end() )
	{
		cell = new Cell( new Address(address) );
		addrCells.insert( std::pair<Address, Cell*>( address, cell ) );
	}
	else
		cell = it->second;
	}
	cell->AddPoint( point, obj );

	// обновление map< void*, set<Vector3*>* > objPoint
	{
	auto it = objPoint.find( obj );
	if( it != objPoint.end() )
		it->second->insert( &point );
	else
	{
		set<Vector3*> *curSet = new set<Vector3*>;
		curSet->insert( &point );
		objPoint.insert( pair< void*, set<Vector3*>* >( obj, curSet ) );
	}
	}
	
	// обновление map< void*, set<Cell*>* > objCell
	{
	auto it = objCell.find(obj);
	if( it != objCell.end() )
		it->second->insert( point.getCell() );
	else
	{
		set<Cell*> *curSet = new set<Cell*>;
		curSet->insert( point.getCell() );
		objCell.insert( pair< void*, set<Cell*>* >( obj, curSet ) );
	}
	}

	// обновление set<Cell*> hotCells
	{
		auto curMap = cell->getObjects();
		if( curMap->size() > 1 )
			hotCells.insert( cell );
	}

}


void DynamicStruct::movePoint( Vector3& point, Vector3& v )
{
	if( pointObj.find( &point ) != pointObj.end() )
	{
		point.x = point.x + v.x;
		point.y = point.y + v.y;
		point.z = point.z + v.z;
		
		Address curAddr = getAddress(point);

		Cell *curCell = point.getCell();
		if( *(curCell->getAddress()) != curAddr )  // Если адрес получается другой то продолжаем
		{
			curCell->DeletePoint( point );
			// обновление set<Cell*> hotCells
			auto curMap = curCell->getObjects();
			if( curMap->size() < 2 )
				hotCells.erase( curCell );

			Cell* cell;
			auto it = addrCells.find( curAddr );
			if( it != addrCells.end() )
			{
				cell = it->second;
			}
			else
			{
				cell = new Cell( new Address( curAddr ) );
				addrCells.insert( std::pair<Address, Cell*>( curAddr, cell ) );
			}
			void *obj = pointObj.at( &point );
			cell->AddPoint( point, obj );

			// обновление map< void*, set<Cell*>* > objCell
			auto it1 = objCell.find (obj );
			set<Cell*>* curSet = it1->second;
			curSet->erase( curCell );
			curSet->insert( cell );

			// обновление set<Cell*> hotCells
			curMap = cell->getObjects();
			if( curMap->size() > 1 )
				hotCells.insert( cell );
		}
	}
}


void DynamicStruct::moveObject( void* obj, Vector3& v )
{
	auto it = objPoint.find( obj );
	if( it != objPoint.end() )
	{
		set<Vector3*> *curSet = it->second;
		for( auto it1 = curSet->begin(); it1 != curSet->end(); it1++ )
			movePoint ( **it1, v );
	}
}


Cell* DynamicStruct::getCell( const Vector3& point ) const
{
	if( pointObj.find( const_cast<Vector3*>(&point) ) != pointObj.end() )
		return point.getCell();
	return 0;
}

set<Cell*>* DynamicStruct::getCells( const void* obj ) const
{
	auto it = objCell.find( const_cast<void*>(obj) );
	if( it != objCell.end() )
		return it->second;
	return 0;
}


map<Vector3*, void*>* DynamicStruct::getPoints ( const Cell* cell ) const
{
	return (const_cast<Cell*>(cell))->getPoints();
}

map< void*, set<Vector3*>* >* DynamicStruct::getObjects ( const Cell* cell ) const
{
	return (const_cast<Cell*>(cell))->getObjects();
}

Cell* DynamicStruct::getCell( const Address& addr ) const
{
	auto it = addrCells.find(addr);
	if( it != addrCells.end() )
		return it->second;
	return 0;
}

Address& DynamicStruct::getAddress( const Cell* cell ) const
{
	return *( cell->getAddress() );
}

Address DynamicStruct::getAddress( const Vector3& vec ) const
{
	int i, j, k;
	i = ( int ) ( vec.x / size );
	j = ( int ) ( vec.y / size );
	k = ( int ) ( vec.z / size );
	Address curAddr( i, j, k );
	return curAddr;
}

Vector3 DynamicStruct::getVector( const Address& addr ) const
{
	return Vector3( ( float )addr.i * size, ( float )addr.j * size, ( float )addr.k * size );
}

Vector3 DynamicStruct::getVector( const Cell* cell ) const
{
	return Vector3( ( float )cell->getAddress()->i * size, ( float )cell->getAddress()->j * size, ( float )cell->getAddress()->k * size );
}

set<Cell*>* DynamicStruct::getHotCells() const
{
	return const_cast<set<Cell*>*>(&hotCells);
}
//....................................................................

//........................................................
Address::Address( int i, int j, int k )
{
	this->i = i;
	this->j = j;
	this->k = k;
}

Address::Address( const Address& other )
{
	this->i = other.i;
	this->j = other.j;
	this->k = other.k;
}


bool Address::operator < ( const Address& r ) const
{
	if( i != r.i )
		return i < r.i;
	if( j != r.j )
		return j<r.j;
	return k < r.k;
}

bool Address:: operator != ( const Address& r) const
{
	return ( ( i != r.i ) || ( j != r.j ) || ( k != r.k ) );
}

bool Address::operator== ( const Address& r ) const
{
	return ( ( i == r.i ) && ( j == r.j ) && ( k == r.k ) );
}
//.......................................................