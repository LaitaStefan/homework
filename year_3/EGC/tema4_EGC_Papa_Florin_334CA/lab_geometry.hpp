//Papa Florin, 334CA

#pragma once
#include "dependente\freeglut\freeglut.h"

namespace lab{

	//sfera
	static void drawSolidSphere(float radius,int slices, int stacks) { glutSolidSphere(radius,slices,stacks);}
	static void drawWireSphere(float radius,int slices, int stacks) { glutWireSphere(radius,slices,stacks); }

	//cub
	static void drawSolidCube(float size){glutSolidCube(size);}
	static void drawWireCube(float size){glutWireCube(size);}

	//con
	static void drawSolidCone(float base_radius, float height, int slices, int stacks) { glutSolidCone(base_radius,height,slices,stacks);}
	static void drawWireCone(float base_radius, float height, int slices, int stacks) {glutWireCone(base_radius,height,slices,stacks);}

	//torus
	static void drawSolidTorus(float inner_radius, float outer_radius, int nsides, int rings){glutSolidTorus(inner_radius, outer_radius, nsides, rings);}
	static void drawWireTorus(float inner_radius, float outer_radius, int nsides, int rings){glutWireTorus(inner_radius, outer_radius, nsides, rings);}

	//dodecaedru
	static void drawSolidDodecahedron(){glutSolidDodecahedron();}
	static void drawWireDodecahedron(){glutWireDodecahedron();}

	//octaedru
	static void drawSolidOctahedron(){glutSolidOctahedron();}
	static void drawWireOctahedron(){glutWireOctahedron();}

	//tetraedru
	static void drawSolidTetrahedron(){glutSolidTetrahedron();}
	static void drawWireTetrahedron(){glutWireTetrahedron();}

	//icosaedru
	static void drawSolidIcosahedron(){glutSolidIcosahedron();}
	static void drawWireIcosahedron(){glutWireIcosahedron();}

	//ceainic (poveste: https://en.wikipedia.org/wiki/Utah_teapot)
	static void drawSolidTeapot(float size){glutSolidTeapot(size);}
	static void drawWireTeapot(float size){glutSolidTeapot(size);}
	
}