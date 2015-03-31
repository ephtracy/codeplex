#ifndef __MV_FILE__
#define __MV_FILE__

#include <stdio.h>

//================
// ReadFile
//================
class MV_ReadFile {
public :
	FILE *fp;

public :
	~MV_ReadFile() {
		Close();
	}

	MV_ReadFile() : fp( NULL ) {
	}

	bool Open( const char *path ) {
		Close();
		fp = fopen( path, "rb" );
		return ( fp != NULL );
	}

	void Close( void ) {
		if ( fp ) {
			fclose( fp );
			fp = NULL;
		}
	}

	int ReadInt( void ) {
		int v = 0;
		fread( &v, sizeof( int ), 1, fp );
		return v;
	}

	void ReadData( void *data, int elemSize, int count ) {
		fread( data, elemSize, count, fp );
	}

	void Seek( int pos, int origin ) {
		fseek( fp, pos, origin );
	}

	int Position( void ) {
		return ftell( fp );
	}
};

#endif //__MV_FILE__