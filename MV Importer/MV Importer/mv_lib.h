#ifndef __MV_LIB__
#define __MV_LIB__

// magic number
int MV_ID( int a, int b, int c, int d ) {
	return ( a ) | ( b << 8 ) | ( c << 16 ) | ( d << 24 );
}

//================
// Vec3i
//================
class MV_Vec3i {
public :
	int x, y, z;

public :
	MV_Vec3i() {
		x = y = z = 0;
	}

	MV_Vec3i &Set( int x, int y, int z ) {
		this->x = x;
		this->y = y;
		this->z = z;
		return *this;
	}

	MV_Vec3i &Zero( void ) {
		x = y = z = 0;
		return *this;
	}
};

//================
// RGBA
//================
class MV_RGBA {
public :
	unsigned char r, g, b, a;

public :
	MV_RGBA() {
		r = g = b = a = 0;
	}
};


//================
// Voxel
//================
class MV_Voxel {
public :
	unsigned char x, y, z, colorIndex;

public :
	MV_Voxel() {
		x = y = z = colorIndex = 0;
	}
};

#endif __MV_LIB__