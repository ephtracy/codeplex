// MV Importer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "mv_vox.h"
#include "mv_slab.h"
#include "mv_xraw.h"
#include "mv_qb.h"

int _tmain(int argc, _TCHAR* argv[])
{
	
	printf( "==== load .vox model\n" );
	MV_Model vox;
	vox.LoadModel( "data/colors.vox" );

	printf( "==== load .slab.vox model\n" );
	MV_SLAB slab;
	slab.LoadModel( "data/colors.slab.vox" );

	printf( "==== load .xraw model\n" );
	MV_XRAW xraw;
	xraw.LoadModel( "data/colors.xraw" );

	printf( "==== load .qb model\n" );
	MV_QB qb;
	qb.LoadModel( "data/colors.qb" );

	return 0;
}

