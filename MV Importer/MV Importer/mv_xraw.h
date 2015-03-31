#ifndef __MV_XRAW__
#define __MV_XRAW__

#include "mv_lib.h"
#include "mv_file.h"

//================
// XRAW
//================
class MV_XRAW {
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
		
		MV_ReadFile file;
		if ( !file.Open( path ) ) {
			printf( "error : cannot open file \"%s\"\n", path );
			return false;
		}
		
		// magic
		{
			int magic = file.ReadInt();
			if ( magic != MV_ID( 'X', 'R', 'A', 'W' ) ) {
				printf( "error : magic number does not match : %d\n", magic );
				return false;
			}
		}

		// header
		int elemDataType	= file.ReadByte();
		int numChannels		= file.ReadByte();
		int numBitsPerElem	= file.ReadByte();
		int numBitsPerIndex	= file.ReadByte();

		MV_Vec3i size;
		file.ReadData( &size, sizeof( size ), 1 );
		
		int paletteSize = file.ReadInt();

		if ( elemDataType != 0 || numChannels != 4 || numBitsPerElem != 8 || numBitsPerIndex != 8 ) {
			return false;
		}

		if ( size.x <= 0 || size.y <= 0 || size.z <= 0 || paletteSize != 256 ) {
			return false;
		}
		
		// volume
		{
			int num = size.x * size.y * size.z;
			volume.resize( num );
			file.ReadData( volume.data(), sizeof( unsigned char ), num );
		}
		
		// palette
		file.ReadData( palette, sizeof( unsigned char ), 256 );


		// print info
		printf( "log : %d %d %d\n", size.x, size.y, size.z );
		return true;
	}
};

#endif // __MV_XRAW__