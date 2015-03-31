
#include "mv_lib.h"
#include "mv_file.h"

//================
// QB
//================
class MV_QB_Model {
public :
	MV_Vec3i				size;
	MV_Vec3i				offset;
	char					name[ 256 ];
	std::vector< MV_RGBA >	volume;

public :
	void SetColor( int x, int y, int z, const MV_RGBA &color ) {
		volume[ x + y * size.x + z * size.x * size.y ] = color;
	}
};

class MV_QB {
public :
	int			major;
	int			minor;
	int			release;
	int			build;
	int			colorFormat;
	int			coord;
	int			compression;
	int			visibility;

	std::vector< MV_QB_Model >	models;

public :
	void Free( void ) {
		models.clear();
	}

	// http://www.minddesk.com/wiki/index.php?title=Qubicle_Constructor_1:Data_Exchange_With_Qubicle_Binary
	bool LoadModel( const char *path ) {
		Free();
	
		MV_ReadFile file;
		if ( !file.Open( path ) ) {
			return false;
		}
		
		// version (4)
		major 	= file.ReadByte();
		minor 	= file.ReadByte();
		release = file.ReadByte();
		build 	= file.ReadByte();
		if ( major != 1 || minor != 1 || release != 0 || build != 0 ) {
			printf( "error : version does not match : 1.1.0.0\n");
			return false;
		}
		
		// color format (4)
		colorFormat = file.ReadInt();
		
		// left handed (4)
		coord = file.ReadInt();
		
		// Compression (4):
		// If set to 1 data is compressed using run length encoding (RLE).
		// If set to 0 data is uncompressed.
		compression = file.ReadInt();
		
		// Visibility-Mask encoded (4)
		visibility = file.ReadInt();
		
		printf( "log : colorFomat  : %d\n", colorFormat );
		printf( "log : compression : %d\n", compression );

		// Matrix count
		int numModels = file.ReadInt();
		if ( numModels <= 0 ) {
			printf( "error : no models\n" );
			return false;
		}		
		models.resize( numModels );
		for ( int k = 0; k < numModels; k++ ) {
			LoadSubModel( file, k );
		}

		return true;
	}

	void LoadSubModel( MV_ReadFile &file, int k ) {
		const int CODEFLAG = 2;
		const int NEXTSLICEFLAG = 6;

		MV_QB_Model &sub = models[ k ];

		// name
		{
			int nameLength = file.ReadByte();
			file.ReadData( sub.name, sizeof( char ), nameLength );
			sub.name[ nameLength ] = '\0';
		}

		
		// size
		file.ReadData( &sub.size, sizeof( sub.size ), 1 );
		
		// offset
		file.ReadData( &sub.offset, sizeof( sub.offset ), 1 );
		
		printf( "    log : model : #%d : %s : ( %d %d %d ) : ( %d %d %d )\n",
			k,
			sub.name,
			sub.size.x, sub.size.y, sub.size.z,
			sub.offset.x, sub.offset.y, sub.offset.z
		);
		
		// volume
		int num = sub.size.x * sub.size.y * sub.size.z;
		sub.volume.resize( num  );

		if ( compression == 0 ) {
			 // if uncompressed
			file.ReadData( sub.volume.data(), sizeof( MV_RGBA ), num );
			return;
		}
		
		// if compressed
		for ( int z = 0; z < sub.size.z; z++ ) {
			int index = 0;
		
			while ( 1 ) {
				int flag = file.ReadInt();
			
				if ( flag == NEXTSLICEFLAG ) {
					break;
				}
			
				if ( flag == CODEFLAG ) {
					// count
					int count = file.ReadInt();
				
					// color
					MV_RGBA color;
					file.ReadData( &color, sizeof( color ), 1 );
				
					if ( color.a ) {
						for( int i = 0; i < count; i++ ) {
							int x = index % sub.size.x; // mod = modulo e.g. 12 mod 8 = 4
							int y = index / sub.size.x; // div = integer division e.g. 12 div 8 = 1
							index++;
						
							// improve
							sub.SetColor( x, y, z, color );
						}
					} else {
						index += count;
					}
				} else {
					int x = index % sub.size.x;
					int y = index / sub.size.x;
					index++;
				
					MV_RGBA color = *reinterpret_cast< MV_RGBA * >( &flag );
					if ( color.a ) {
						sub.SetColor( x, y, z, color );
					}
				}
			}
		}
	}
};