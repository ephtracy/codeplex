// MV Importer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "mvox.h"

int _tmain(int argc, _TCHAR* argv[])
{
	MV_Model model;
	model.LoadModel( "data/colors.vox" );
		
	return 0;
}

