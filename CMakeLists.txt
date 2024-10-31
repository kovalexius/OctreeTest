project(OctreeTest)


add_executable(OctreeTest
	OctreeTest/d3dWindow.h
	OctreeTest/DynamicTree.h
	OctreeTest/InitialDirect3D.h

	OctreeTest/d3dWindow.cpp
	OctreeTest/DynamicTree.cpp
	OctreeTest/InitialDirect3D.cpp
	OctreeTest/OctreeTest.cpp
	OctreeTest/openglWindow.cpp
)

target_include_directories(OctreeTest
	PRIVATE
		DynamicStructure
)

target_link_libraries(OctreeTest
	PRIVATE
		d3d9.lib
		d3dx9.lib
		winmm.lib
)