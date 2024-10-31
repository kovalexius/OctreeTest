// Эта маленькая библиотека создана для быстрого разбиения пространства насыщенного объектами у которых есть любое количество контрольных точек
// на AABB ячейки - кубы, Bouding Box'ы и для быстрого распределения объектов и точек по ячейкам при движении объектов или отдельных точек.
// Она может быть использована для MMO игр, для оптимизации вычисления коллизий трёхмерных объектов, и др. задач. 
// Она использует регулярное разбиение трехмерного пространства на кубы с заданным шагом или размером.
// Однако она кушает много памяти, в самом неблагоприятном случае порядка n^2*m, где n - кол-во объектов, m - кол-во точек в объекте. 
// Пользователь-программист библиотеки должен сам подобрать оптимальное значение размера ячейки при вызове конструктора DynamicStruct.
// Для дальнейшей работы, все что необходимо сделать - это создать объекты типа Vector3, которые соответствуют точкам и вызвать метод AddPoint( Vector3& point, void* obj )
// где obj - это указатель на твой объект (любой) приведенный к типу void*. Точки и объекты можно двигать методами 
// movePoint и moveObject соответственно.							 
// При опустошении ячейки, ячейка не удаляется
// Для более точной оптимизации нужно вставить в каждую ячейку Octree и все операции выполнять над ним

// В дальнейшем нужно реализовать потокобезопасность, возможно неблокирующую синхронизацию или написать свои потокобезопасные 
// set, map, unordered_xxx. Нужна также поддержка подписки на события
// в виде коллбэков

// Возможно есть утечки памяти, надобно бы поменять все указатели на shared_ptr и shared_array для массивов

// Сырая врсия!!! Нужно тестировать!!!

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
	std::map< Vector3*, void* >::iterator m_iter; // указывает на Cell::map<Vector3*, void*> pointObj;
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
	map<Vector3*, void*> pointObj;							// Точка - объект в текущей ячейке
	map<void*, set<Vector3*>* > objPoint;				// Объект - его точки в текущей ячейке 
};
//......................................................


//.....................................................
class DynamicStruct
{
public:
	DynamicStruct(float s);  // s - размер ячейки
	~DynamicStruct();
	// Добавить точку point, ассоциированную с объектом obj; 
	// Указатель как тип хорошо бы убрать, а то путаница, лучше использовать uint32_t для идентификации объектов
	void AddPoint( Vector3& point, void* obj );	
	// Переместить точку point на вектор v
	void movePoint( Vector3& point, Vector3& v );	
	// Переместить объект obj на вектор v
	void moveObject( void* obj, Vector3& v );															

	// Указываешь точку - получаешь ячейку где она находится
	Cell* getCell ( const Vector3& point ) const;	
	// Указываешь объект - получаешь все ячейки где находятся точки объекта
	set<Cell*>* getCells ( const void* obj ) const;	
	// Указываешь ячейку - получаешь все точки в этой ячейке
	map< Vector3*, void* >* getPoints ( const Cell* cell ) const;		
	// Указываешь ячейку - получаешь объекты в этой ячейке
	map< void*, set<Vector3*>* >* getObjects ( const Cell* cell ) const;  
	
	Cell* getCell ( const Address& addr ) const;													// Указываешь адрес - получаешь ячейку
	Address& getAddress( const Cell* cell ) const;												// Указываешь ячейку - получаешь адрес
	Address getAddress( const Vector3& vec ) const;											// Указываешь координату - получаешь адрес
	Vector3 getVector( const Address& addr ) const;											// Указываешь адрес - получаешь координату
	Vector3 getVector( const Cell* cell ) const;													// Указываешь ячейку - получаешь координату
	set<Cell*>* getHotCells() const;																			// Получаешь все ячейки где больше одного объекта, горячие ячейки
	set<Cell*>* segmentCast( const Vector3 &P0, const Vector3 &P1 );			// Получить ячейки через которые проходит отрезок
private:
	float size;
	map< Address, Cell* > addrCells;
	map< Vector3*, void* > pointObj;					// точка - объект
	map< void*, set<Vector3*>* > objPoint;		// объект - его точки к каждому
	map< void*, set<Cell*>* > objCell;				// объект - его ячейки к каждому
	set<Cell*> hotCells;											// ячейки, где есть несколько объектов
};
//.....................................................


#endif