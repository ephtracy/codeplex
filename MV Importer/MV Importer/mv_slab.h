#ifndef __MV_SLAB__
#define __MV_SLAB__

#include <vector>
#include "mv_lib.h"
#include "mv_file.h"

//================
// SLAB
//================
class MV_SLAB {
public :
	MV_Vec3i						size;
	std::vector< unsigned char >	volume;
	MV_RGBA							palette[ 256 ];

public :
	void Free( void ) {
		size.Zero();
		volume.clear();
	}

	bool LoadModel( const char *path ) {
		Free();
		
		// file
		MV_ReadFile file;
		if ( !file.Open( path ) ) {
			printf( "error : cannot open file \"%s\"\n", path );
			return false;
		}

		// size
		MV_Vec3i size;
		file.ReadData( &size, 1, sizeof( size ) );
		if ( size.x <= 0 || size.y <= 0 || size.z <= 0 ) {
			printf( "error : invalid size\n" );
			return false;
		}

		// color index volume
		// NOTICE : color 255 is reserved for empty voxels
		// NOTICE : the voxels are stored in "z, y, x" order
		// for ( int x = 0; x < size.x; x++ ) {
		//		for ( int y = 0; y < size.y; y++ ) {
		//			for ( int z = 0; z < size.z; z++ ) {
		//			}
		//		}
		//	}
		{
			int num = size.x * size.y * size.z;
			volume.resize( num );
			file.ReadData( volume.data(), sizeof( unsigned char ), num );
		}
		
		// palette
		// NOTICE : colors are stored as "r/4, g/4, b/4"
		for ( int i = 0; i < 256; i++ ) {
			unsigned char color[ 3 ];
			file.ReadData( color, sizeof( unsigned char ), 3 );

			palette[ i ].r = ( color[ 0 ] << 2 );
			palette[ i ].g = ( color[ 1 ] << 2 );
			palette[ i ].b = ( color[ 2 ] << 2 );
			palette[ i ].a = 255;
		}
		
		// print info
		printf( "log : %d %d %d\n", size.x, size.y, size.z );
		return true;
	}
};

#endif // __MV_SLAB__