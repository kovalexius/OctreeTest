// OctreeTest.cpp : Defines the entry point for the console application.
//

#include "DynamicStruct.h"
#include <conio.h>
#include <iostream>
#include <string>
#include <tchar.h>
#include <ctime>
#include <random>

#include "InitialDirect3D.h"

#define NUM_OBJ 100000
#define NUM_VEC_PER_OBJ 8

void main()
{
	//WinD3DMain();
	WinMainD3D();

	DynamicStruct scene( 10 );

	int *obj;
	obj = new int[NUM_OBJ];
	Vector3 *v;
	v = new Vector3[ NUM_OBJ * NUM_VEC_PER_OBJ ];

	auto begin = clock();

	for( int i = 0; i < NUM_OBJ; i++ )
	{
		int xshift = rand();
		int yshift = rand();
		int zshift = rand();
		char buf[10];
		itoa( i, buf, 10 );
		string name = "v" + string(buf);
		v[ i * NUM_VEC_PER_OBJ + 0 ] = Vector3( 0 + xshift, 0 + yshift, 0 + zshift,name );
		v[ i * NUM_VEC_PER_OBJ + 1 ] = Vector3( 1 + xshift, 0 + yshift, 0 + zshift,name );
		v[ i * NUM_VEC_PER_OBJ + 2 ] = Vector3( 1 + xshift, 1 + yshift, 0 + zshift,name );
		v[ i * NUM_VEC_PER_OBJ + 3 ] = Vector3( 0 + xshift, 1 + yshift, 0 + zshift,name );
		v[ i * NUM_VEC_PER_OBJ + 4 ] = Vector3( 0 + xshift, 0 + yshift, 1 + zshift,name );
		v[ i * NUM_VEC_PER_OBJ + 5 ] = Vector3( 1 + xshift, 0 + yshift, 1 + zshift,name );
		v[ i * NUM_VEC_PER_OBJ + 6 ] = Vector3( 1 + xshift, 1 + yshift, 1 + zshift,name );
		v[ i * NUM_VEC_PER_OBJ + 7 ] = Vector3( 0 + xshift, 1 + yshift, 1 + zshift,name );
		obj[ i ] = i;
	}

	{
		auto end = clock();
		double elapsedMs = double(end - begin) * 1000.0 / CLOCKS_PER_SEC;
		cout << "elapsed time = " << elapsedMs << endl;
		begin = end;
	}
	cout << " Adding " << NUM_OBJ << " objects" << endl;

	for( int i = 0; i < NUM_OBJ; i++ )
	{
		scene.AddPoint( v[ i * NUM_VEC_PER_OBJ + 0 ], &obj[i] );
		scene.AddPoint( v[ i * NUM_VEC_PER_OBJ + 1 ], &obj[i] );
		scene.AddPoint( v[ i * NUM_VEC_PER_OBJ + 2 ], &obj[i] );
		scene.AddPoint( v[ i * NUM_VEC_PER_OBJ + 3 ], &obj[i] );
		scene.AddPoint( v[ i * NUM_VEC_PER_OBJ + 4 ], &obj[i] );
		scene.AddPoint( v[ i * NUM_VEC_PER_OBJ + 5 ], &obj[i] );
		scene.AddPoint( v[ i * NUM_VEC_PER_OBJ + 6 ], &obj[i] );
		scene.AddPoint( v[ i * NUM_VEC_PER_OBJ + 7 ], &obj[i] );
	}

	{
		auto end = clock();
		double elapsedMs = double(end - begin) * 1000.0 / CLOCKS_PER_SEC;
		cout << "elapsed time = " << elapsedMs << endl;
		begin = end;
	}

	cout << " Moving object " << endl;

	for( int i=0; i<100000; i++ )
	{
		set< Cell* > cells;
		cells = *(scene.getHotCells());
		/*
		for( auto it = cells.begin(); it != cells.end(); it++ )
		{
			printf(" Cell address %d,%d,%d \n", (*it)->getAddress()->i, (*it)->getAddress()->j, (*it)->getAddress()->k );
			auto curMap = scene.getObjects( *it );
			for( auto it1 = curMap->begin(); it1 != curMap->end(); it1++ )
			{
				int *num = (int*)( it1->first );
				printf( " Object %d \n", *num );
			}
		}
		*/
		Vector3 move( 4, 0, 0 );
		scene.moveObject( &obj[0], move );
		//printf("\n");
	}

	{
		auto end = clock();
		double elapsedMs = double(end - begin) * 1000.0 / CLOCKS_PER_SEC;
		cout << "elapsed time = " << elapsedMs << endl;
	}

	getch();
}

