// zipcmp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "zipcmp.h"
#include "tstring.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	if(argv[1] == NULL)
	{
		std::cout << " Please enter the file for compressing." << std::endl;
		return -1;
	}

	std::tstring strFileName(argv[1]);

	return 0;
}

